//===------------------------- stream.cpp -------------------------===//
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

#include "utopia/stream.hpp"

#include <array>
#include <doctest.h>

using namespace std;
using namespace utopia;

TEST_CASE("MemoryStream") {
    core::MemoryStream stream{};

    // write
    stream.write(static_cast<char>('a'));
    stream.write(static_cast<unsigned char>(0));
    stream.write(static_cast<short>(1));
    stream.write(static_cast<unsigned short>(2));
    stream.write(static_cast<signed>(3));
    stream.write(static_cast<unsigned>(4));
    stream.write(static_cast<long>(5));
    stream.write(static_cast<unsigned long>(6));
    stream.write(static_cast<long long>(7));
    stream.write(static_cast<unsigned long long>(8));
    stream.write(9.0f);
    stream.write(10.0);
    stream.write(11.0l);

    std::array<std::byte, 5> data{};
    data[0] = static_cast<std::byte>(20);
    data[1] = static_cast<std::byte>(21);
    data[2] = static_cast<std::byte>(22);
    data[3] = static_cast<std::byte>(23);
    data[4] = static_cast<std::byte>(24);
    span<std::byte, 5> sp{ data };

    stream.write(sp);

    // then check
    CHECK_EQ(stream.get_char(), 'a');
    CHECK_EQ(stream.get_uchar(), 0);
    CHECK_EQ(stream.get_short(), 1);
    CHECK_EQ(stream.get_ushort(), 2);
    CHECK_EQ(stream.get_int(), 3);
    CHECK_EQ(stream.get_uint(), 4);
    CHECK_EQ(stream.get_long(), 5);
    CHECK_EQ(stream.get_ulong(), 6);
    CHECK_EQ(stream.get_llong(), 7);
    CHECK_EQ(stream.get_ullong(), 8);
    CHECK_EQ(stream.get_float(), 9.0f);
    CHECK_EQ(stream.get_double(), 10.0);
    CHECK_EQ(stream.get_ldouble(), 11.0l);

    auto read = stream.get_bytes(sp.size_bytes());
    CHECK_EQ(std::to_integer<int>(read[0]), 20);
    CHECK_EQ(std::to_integer<int>(read[1]), 21);
    CHECK_EQ(std::to_integer<int>(read[2]), 22);
    CHECK_EQ(std::to_integer<int>(read[3]), 23);
    CHECK_EQ(std::to_integer<int>(read[4]), 24);
}
