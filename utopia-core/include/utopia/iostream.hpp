//===------------------------ iostream.hpp ------------------------===//
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
/// 这个文件实现了一个io流。用于向stdout和stderr进行输出。
/// 这个流是线程安全的，并且拥有更多功能。所有输出都应该使用这个流。
//===------------------------------------------------------===//

#pragma once

#include <fmt/format.h>
#include <string_view>
#include <utility>

namespace utopia::core {

    /**
     * @brief 写入字符串到stdout
     * @param str 要写入的字符串
    */
    void write_stdout(std::string_view str);

    /**
     * @brief 写入字符串到stderr
     * @param str 要写入的字符串
    */
    void write_stderr(std::string_view str);

}   // namespace utopia::core
