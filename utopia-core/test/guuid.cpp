//===---------------------- guuid.cpp ---------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//

#include "utopia/guuid.hpp"

#include <doctest.h>

using namespace std;


TEST_CASE("utopia::core::Guuid Constructor Test") {
    utopia::core::Guuid guuid{
        "root"sv,
        {"1"sv, "2"sv}
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
            utopia::core::Guuid guuid{ "", {} };
        };

        CHECK_THROWS(func());
    }
    {
        auto func = []
        {
            utopia::core::Guuid guuid{ "I'm a illegal name!", {} };
        };

        CHECK_THROWS(func());
    }
    {
        auto func = []
        {
            utopia::core::Guuid guuid{ "root", { "I'm a illegal name!" } };
        };

        CHECK_THROWS(func());
    }
}

TEST_CASE("utopia::core::Guuid Compare Test") {
    utopia::core::Guuid guuid1{
        "root",
        {"1", "2"}
    };

    utopia::core::Guuid guuid2{
        "root",
        {"1", "2"}
    };

    utopia::core::Guuid guuid3{ "root", { "1" } };
    utopia::core::Guuid guuid4{ "root2", { "1" } };
    utopia::core::Guuid guuid5{ "root", { "2" } };
    utopia::core::Guuid guuid6{
        "root",
        {"2", "1"}
    };

    CHECK_EQ(guuid1, guuid2);
    CHECK_NE(guuid1, guuid3);
    CHECK_NE(guuid2, guuid4);
    CHECK_NE(guuid3, guuid4);
    CHECK_NE(guuid3, guuid5);
    CHECK_NE(guuid1, guuid6);
}
