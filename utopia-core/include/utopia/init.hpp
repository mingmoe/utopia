//===-------------------------- init.hpp --------------------------===//
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
/// 声明一些程序初始化所需要的函数和数据等。
//===------------------------------------------------------===//

#pragma once

#include <unicode/putil.h>
#include <filesystem>

namespace utopia::core {

    /**
     * @brief 设置ICU数据
    */
    inline void setup_icu_data() {
        auto path = std::filesystem::current_path().string();
        u_setDataDirectory(path.c_str());
    }


}
