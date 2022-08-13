//===------------------------ iostream.cpp ------------------------===//
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
/// 这个文件实现了iostream中的两个写入函数。
//===------------------------------------------------------===//

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
