//===-------------------- constexpr.hpp -------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//
/// \file
/// 一些constexpr的工具函数
//===------------------------------------------------------===//

#pragma once

namespace utopia::core {

    constexpr bool isalnum(int c) {
        return ((c >= 0x30) || (c <= 0x39)) || ((c >= 0x41) || (c <= 0x5a)) ||
               ((c >= 0x61) || (c <= 0x7A));
    }

}   // namespace utopia::core
