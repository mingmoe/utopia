//===----------------- source_location.hpp ----------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//
/// \file
/// copy from "source_location" in Windows
/// with little modify for clang
//===------------------------------------------------------===//
// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <cstdint>

// clang has no __builtin_COLUMN
#if __has_builtin(__builtin_COLUMN)
    #define UTOPIA_BUILT_IN_COLUMN __builtin_COLUMN()
#else
    #define UTOPIA_BUILT_IN_COLUMN 0
#endif

namespace utopia::core {
    struct source_location {
        // ------------------- because clang not supported
        // ------------------↓ so change this from consteval to constexpr
        [[nodiscard]] static constexpr source_location current(
            const uint_least32_t _Line_     = __builtin_LINE(),
            const uint_least32_t _Column_   = UTOPIA_BUILT_IN_COLUMN,
            const char *const    _File_     = __builtin_FILE(),
            const char *const    _Function_ = __builtin_FUNCTION()) noexcept {
            source_location _Result;
            _Result._Line     = _Line_;
            _Result._Column   = _Column_;
            _Result._File     = _File_;
            _Result._Function = _Function_;
            return _Result;
        }

        constexpr source_location() noexcept = default;

        [[nodiscard]] constexpr uint_least32_t line() const noexcept {
            return _Line;
        }
        [[nodiscard]] constexpr uint_least32_t column() const noexcept {
            return _Column;
        }
        [[nodiscard]] constexpr const char *file_name() const noexcept {
            return _File;
        }
        [[nodiscard]] constexpr const char *function_name() const noexcept {
            return _Function;
        }

      private:

        uint_least32_t _Line{};
        uint_least32_t _Column{};
        const char    *_File     = "";
        const char    *_Function = "";
    };
}   // namespace utopia::core

#undef UTOPIA_BUILT_IN_COLUMN
