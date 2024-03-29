//===-------------------------- abort.hpp -------------------------===//
//   Copyright (C) 2021-2022 mingmoe(me@kawayi.moe)(https://kawayi.moe)
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
/// 这个文件负责处理程序的中断，信号，断点等。
//===--------------------------------------------------------------===//

#pragma once

#include <boost/stacktrace.hpp>
#include <exception>
#include <sstream>
#include <string_view>

#if !defined(_MSC_VER)
    #if !__has_builtin(__builtin_debugtrap)
    #include <csignal>
    #endif
#endif

#include "utopia/iostream.hpp"
#include "utopia/source_location.hpp"

namespace utopia::core {

    class Exception;

    /**
     * @brief debug断点
    */
    inline void debug_break() {
#if !defined(UTOPIA_NO_DEBUG_BREAK)
    #if defined(_MSC_VER)
        __debugbreak();
    #elif __has_builtin(__builtin_debugtrap)
        __builtin_debugtrap();
    #elif defined(SIGTRAP)
        raise(SIGTRAP);
    #else
        #error no debug break impl found
    #endif
#endif
        return;
    }

    /**
     * @brief 断言函数。类似于标准库中的assert。
     * @param condition 断言条件
     * @param reason 断言原因
     * @param location 由编译器自动填写。调用位置。
     * @note 断言函数在运行时也将会存在。要消除开销使用u_debug_assert宏
    */
    inline void
        u_assert(bool                   condition,
                 const std::string_view reason,
                 source_location        location = source_location::current()) {

        if(!condition) {
            fmt::print(
                stdout,
                "====> abort <====\nassert failed down:{}\nat {}({}:{}) {}()",
                reason,
                location.file_name(),
                location.line(),
                location.column(),
                location.function_name());
            debug_break();
            std::abort();
        }
    }

    /**
     * @brief 不可到达调用此函数。等价于以false为条件调用断言函数。
     * @param reason 不可到达的原因。
     * @param location 由编译器自动填写。调用位置。
    */
    [[noreturn]] inline void
        unreachable(const std::string_view reason,
                    source_location location = source_location::current()) {
        u_assert(false, reason, location);
        std::abort();
    }

    /**
     * @brief 初始化terminate句柄
    */
    inline void init_terminate_handle() {
        auto handle = []
        {
            std::stringstream ss{};
            ss << "===> terminate <===\n";
            ss << boost::stacktrace::stacktrace();
            write_stderr(ss.str());
        };
        std::set_terminate(handle);
    }

    [[noreturn]] inline void abort_with_exception(
        Exception       err,
        source_location location = source_location::current());

}   // namespace utopia::core

/// @macro debug_u_assert 这个宏在release构建下不会进行断言。
#ifdef NDEBUG
    #define utopia_debug_assert(...)
#else
    #define utopia_debug_assert(...) utopia::core::u_assert(__VA_ARGS__)
#endif
