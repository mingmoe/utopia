//===---------------------- abort.hpp ----------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===-------------------------------------------------------===//
/// \file
/// 这个文件负责处理程序的中断，信号，断点等。
//===-------------------------------------------------------===//

#pragma once

#include <boost/stacktrace/stacktrace.hpp>
#include <exception>
#include <source_location>
#include <sstream>
#include <string_view>

#if !defined(_MSC_VER) && !__has_builtin(__builtin_debugtrap)
    #include <csignal>
#endif

#include "utopia/iostream.hpp"

namespace utopia::core {

    /**
     * @brief debug断点
    */
    inline void debug_break() {
#ifndef NDEBUG
    #ifdef _MSC_VER
        __debugbreak();
    #elif __has_builtin(__builtin_debugtrap)
        __builtin_debugtrap
    #elif defined(SIGTRAP)
        raise(SIGTRAP)
    #else
        unreachable("no debug break impl");
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
    inline constexpr void u_assert(
        bool                   condition,
        const std::string_view reason,
        std::source_location   location = std::source_location::current()) {

        if(!condition) {
            fmt::print(
                stdout,
                "====> abort <====\nassert failed down:{}\nat {}:{}:{} {}()",
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
    [[noreturn]] inline constexpr void unreachable(
        const std::string_view reason,
        std::source_location   location = std::source_location::current()) {
        u_assert(false, reason, location);
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

}   // namespace utopia::core

/// @macro debug_u_assert 这个宏在release构建下不会进行断言。
#ifdef NDEBUG
    #define u_debug_assert(...)
#else
    #define u_debug_assert(...) utopia::core::u_assert(__VA_ARGS__)
#endif
