//===---------------------- guuid.hpp ---------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//

#include "utopia/guuid.hpp"

#include <doctest.h>

TEST_CASE("utopia::core::Guuid Constructor Test") {
    utopia::core::Guuid guuid{
        "root",
        {"1", "2"}
    };

    CHECK_EQ(guuid.get_namespace_ref().size(), 2);
    {
        auto func = []
        {
            utopia::core::Guuid guuid{ "nosp", {} };
        };

        CHECK_NOTHROW(func);
    }
    {
        auto func = []
        {
            utopia::core::Guuid guuid{ "I'm a illegal name!", {} };
        };

        CHECK_THROWS(func);
    }
    {
        auto func = []
        {
            utopia::core::Guuid guuid{ "root", { "I'm a illegal name!" } };
        };

        CHECK_THROWS(func);
    }
}
