//===------------------------ constexpr.hpp -----------------------===//
//   Copyright (C) 2021-2022 mingmoe(me@kawayi.moe)(https://blog.kawayi.moe)
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
/// \file
/// 一些constexpr的工具函数
//===------------------------------------------------------===//

#pragma once

namespace utopia::core {

    constexpr bool isalnum(int c) {
        return ((c >= 0x30) && (c <= 0x39)) || ((c >= 0x41) && (c <= 0x5a)) ||
               ((c >= 0x61) && (c <= 0x7A));
    }

}   // namespace utopia::core
