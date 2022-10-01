//===------------------------ constexpr.hpp -----------------------===//
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
/// \file
/// 提供一些常量函数
//===--------------------------------------------------------------===//

namespace utopia::core {

    /**
     * 如果是ASCII中的字母或者数字则返回true
     */
    constexpr bool isalnum(char c) {
        if((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'Z')) {
            return true;
        }
        return false;
    }


}   // namespace utopia::core
