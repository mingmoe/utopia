//===---------------------- init.hpp ----------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//
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
