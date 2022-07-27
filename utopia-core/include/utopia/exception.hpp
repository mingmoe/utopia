// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
//===-------------------- exception.hpp --------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
//===-------------------------------------------------------===//
/// \file
/// 这个文件声明了utopia::core::Exception和一些常用异常。
/// 程序中所有抛出的异常都应该是派生于utopia::core::Exception的。
//===-------------------------------------------------------===//

#pragma once

#include "utopia/abort.hpp"
#include "utopia/template.hpp"
#include <boost/stacktrace.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace utopia::core {

    /**
     * @brief 所有异常的基类
    */
    class Exception : public std::exception {
      protected:

        std::string                   msg{};

        boost::stacktrace::stacktrace stack{};

        std::exception_ptr            inner_exception{};

        Exception(std::exception_ptr exception) :
            stack(boost::stacktrace::stacktrace()), inner_exception(exception) {
        }

      public:

        /**
         * @brief 构造一个新异常
         * @param msg 异常信息
         * @param exception 引发此异常的异常。可为空。
        */
        Exception(std::string_view   msg,
                  std::exception_ptr exception = std::exception_ptr{}) :
            Exception(exception) {
            std::stringstream ss{};

            //::" << Name.value << "
            ss << "===> Exception <===\n" << msg << "\n" << stack;

            this->msg = ss.str();

            debug_break();
        }

        virtual char const *what() const noexcept override {
            return msg.c_str();
        }

        /**
         * @brief 获取异常构造时的堆栈
        */
        virtual boost::stacktrace::stacktrace get_stacktrace() const {
            return stack;
        }
    };

    /**
     * @brief 使用模板特例化的通用异常。
    */
    template<StringLiteral Name>
    class UniversalException : public Exception {
      public:

        /**
         * @brief 构造一个新异常
         * @param msg 异常信息
         * @param exception 引发此异常的异常。可为空。
        */
        UniversalException(std::string_view   msg,
                           std::exception_ptr inner = std::exception_ptr{}) :
            Exception(inner) {

            this->inner_exception = inner;
            std::stringstream ss{};

            ss << "===> Exception::" << Name.value << " <===\n"
               << msg << "\n"
               << stack;

            if(inner) {
                ss << "\n===> With Inner Exception:";

                try {
                    std::rethrow_exception(inner);
                }
                catch(const Exception &e) {
                    ss << e.what();
                }
                catch(const std::exception &e) {
                    ss << e.what();
                }
                catch(const std::string &e) {
                    ss << e;
                }
                catch(const char *e) {
                    ss << std::string_view{ e };
                }
                catch(...) {
                    ss << "unknown inner exception";
                }
            }

            this->msg = ss.str();

            debug_break();
        }
    };

    using RuntimeException     = UniversalException<"RuntimeException">;

    using IOException          = UniversalException<"IOException">;

    using NullPointerException = UniversalException<"NullPointerException">;

    using IllegalParameterException =
        UniversalException<"IllegalParameterException">;

}   // namespace utopia::core
