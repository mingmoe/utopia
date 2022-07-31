//===-------------------- iostream.hpp --------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//
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
