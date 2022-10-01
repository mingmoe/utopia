//===-------------------------- guuid.cpp -------------------------===//
//   Copyright (C) 2021-2022 mingmoe(me@kawayi.moe)(https://kawayi.moe)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU Affero General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License
//===--------------------------------------------------------------===//

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
