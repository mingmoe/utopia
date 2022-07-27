// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
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

#include "utopia/exception.hpp"
#include <cstdio>
#include <mutex>

namespace {

    inline void write_to(std::string_view str,
                         std::mutex      &lock,
                         decltype(stdout) stream) {
        std::lock_guard locker{ lock };

        auto ret = fwrite(str.data(), sizeof(char), str.size(), stream);

        if(ret != str.size()) {
            throw utopia::core::IOException{
                "failed to write to stdout or stderr"
            };
        }
    }

}   // namespace

void utopia::core::write_stdout(std::string_view str) {
    static std::mutex stdout_locker{};

    write_to(str, stdout_locker, stdout);
}

void utopia::core::write_stderr(std::string_view str) {
    static std::mutex stderr_locker{};

    write_to(str, stderr_locker, stderr);
}
