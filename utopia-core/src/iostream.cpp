//===-------------------- u_assert.cpp ---------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===-------------------------------------------------------===//
/// \file
/// 这个文件实现了iostream中的两个写入函数。
//===-------------------------------------------------------===//

#include "utopia/iostream.hpp"

#include <cstdio>
#include <mutex>

namespace {
    std::mutex stdout_locker{};

    std::mutex stderr_locker{};
}   // namespace


void utopia::core::write_stdout(std::string_view str) {
    std::lock_guard locker{ stdout_locker };

    fwrite(str.data(), sizeof(char), str.size(), stdout);
}

void utopia::core::write_stderr(std::string_view str) {
    std::lock_guard locker{ stderr_locker };

    fwrite(str.data(), sizeof(char), str.size(), stderr);
}
