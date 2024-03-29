//===------------------------ exception.hpp -----------------------===//
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
/// 这个文件声明了utopia::core::Exception和一些常用异常。
/// 程序中所有抛出的异常都应该是派生于utopia::core::Exception的。
//===--------------------------------------------------------------===//

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

                std::string temp;

                try {
                    std::rethrow_exception(inner);
                }
                catch(const Exception &e) {
                    temp = e.what();
                }
                catch(const std::exception &e) {
                    temp = e.what();
                }
                catch(const std::string &e) {
                    temp = e;
                }
                catch(const char *e) {
                    temp = std::string_view{ e };
                }
                catch(...) {
                    temp = "unknown inner exception";
                }
                
                std::stringstream tempbuf(temp);
                while(std::getline(tempbuf, temp, '\n')) {
                    ss << '\t' << temp << '\n';
                }
            }

            this->msg = ss.str();

            debug_break();
        }
    };

    using RuntimeException     = UniversalException<"RuntimeException">;

    using IOException          = UniversalException<"IOException">;

    using NullPointerException = UniversalException<"NullPointerException">;

    using IllegalArgumentException =
        UniversalException<"IllegalArgumentException">;

    using RuntimeException    = UniversalException<"RuntimeException">;

    using OutOfRangeException = UniversalException<"OutOfRangeException">;
    
    using UnrealizedException = UniversalException<"UnrealizedException">;

}   // namespace utopia::core
