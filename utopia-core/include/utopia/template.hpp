//===-------------------- template.hpp --------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//
/// \file
/// 一些用于模板的工具类
//===------------------------------------------------------===//

#pragma once

#include <algorithm>
#include <cstddef>

namespace utopia::core {

    /// @brief 字符串字面值
    template<size_t N>
    struct StringLiteral {
        constexpr StringLiteral(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }

        char value[N];
    };


}   // namespace utopia::core
