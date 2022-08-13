//===--------------------- stream.hpp ---------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//
/// \file
/// 实现了一个输入，输出流接口。
/// 并且实现了一个内存流。
//===------------------------------------------------------===//

#pragma once

#include "utopia/exception.hpp"
#include <cstddef>
#include <span>
#include <string_view>
#include <vector>

namespace utopia::core {

    /**
     * @brief 流的基础操作
    */
    struct StreamBase {
        virtual void flush() = 0;
        virtual void close() = 0;
    };

    /**
     * @brief 输出流
    */
    struct OutputStream : public StreamBase {
        virtual void write(char)                 = 0;
        virtual void write(unsigned char)        = 0;
        virtual void write(short)                = 0;
        virtual void write(unsigned short)       = 0;
        virtual void write(signed)               = 0;
        virtual void write(unsigned)             = 0;
        virtual void write(long)                 = 0;
        virtual void write(unsigned long)        = 0;
        virtual void write(long long)            = 0;
        virtual void write(unsigned long long)   = 0;
        virtual void write(float)                = 0;
        virtual void write(double)               = 0;
        virtual void write(long double)          = 0;
        virtual void write(std::string_view)     = 0;
        virtual void write(std::span<std::byte>) = 0;
    };

    /**
     * @brief 输入流
    */
    struct InputStream : public StreamBase {
        virtual char                 get_char()               = 0;
        virtual unsigned char        get_uchar()              = 0;
        virtual short                get_short()              = 0;
        virtual unsigned short       get_ushort()             = 0;
        virtual signed               get_int()                = 0;
        virtual unsigned             get_uint()               = 0;
        virtual long                 get_long()               = 0;
        virtual unsigned long        get_ulong()              = 0;
        virtual long long            get_llong()              = 0;
        virtual unsigned long long   get_ullong()             = 0;
        virtual float                get_float()              = 0;
        virtual double               get_double()             = 0;
        virtual long double          get_ldouble()            = 0;
        virtual std::span<std::byte> get_bytes(size_t length) = 0;
    };

    struct Stream : public OutputStream, public InputStream {};

    /**
     * @brief 内存流
    */
    class MemoryStream : public Stream {
        std::vector<char> buffer{};
        std::size_t       read_offset{ 0 };

        template<class T>
        void write_to(T t) {
            const auto size  = sizeof(t);
            const auto begin = buffer.size();

            for(auto it = size; it != 0; it--) {
                buffer.push_back(0);
            }

            memcpy(buffer.data() + begin, &t, size);
        }

        template<class T>
        T read_from() {
            const auto size         = sizeof(T);

            const auto final_offset = size + read_offset;

            if(final_offset >= buffer.size()) {
                throw OutOfRangeException{
                    "read from MemoryStream out of range"
                };
            }
            const auto buffer = this->buffer.data();

            char       read[sizeof(T)]{ 0 };

            for(auto it = 0; it != size; it++) {
                read[it] = buffer[read_offset + it];
            }

            read_offset = final_offset;

            T result    = std::bit_cast<T>(read);
            return result;
        }

      public:

        virtual void flush() override {}
        virtual void close() override {}

        virtual void write(char v) override {
            write_to(v);
        }
        virtual void write(unsigned char v) override {
            write_to(v);
        }
        virtual void write(short v) override {
            write_to(v);
        }
        virtual void write(unsigned short v) override {
            write_to(v);
        }
        virtual void write(signed v) override {
            write_to(v);
        }
        virtual void write(unsigned v) override {
            write_to(v);
        }
        virtual void write(long v) override {
            write_to(v);
        }
        virtual void write(unsigned long v) override {
            write_to(v);
        }
        virtual void write(long long v) override {
            write_to(v);
        }
        virtual void write(unsigned long long v) override {
            write_to(v);
        }
        virtual void write(float v) override {
            write_to(v);
        }
        virtual void write(double v) override {
            write_to(v);
        }
        virtual void write(long double v) override {
            write_to(v);
        }
        virtual void write(std::string_view v) override {
            for(auto c : v) {
                write_to(c);
            }
        }
        virtual void write(std::span<std::byte> v) override {
            for(auto c : v) {
                write_to(std::to_integer<char>(c));
            }
        }

        virtual std::span<std::byte> get_bytes(size_t length) override {
            const auto final_offset = length + read_offset;

            if(final_offset >= buffer.size()) {
                throw OutOfRangeException{
                    "read from MemoryStream out of range"
                };
            }
            const auto buffer =
                reinterpret_cast<std::byte *>(this->buffer.data());

            std::span<std::byte> result(buffer, buffer + length);

            return result;
        }
        virtual char get_char() override {
            return read_from<char>();
        }
        virtual unsigned char get_uchar() override {
            return read_from<unsigned char>();
        }
        virtual short get_short() override {
            return read_from<short>();
        }
        virtual unsigned short get_ushort() override {
            return read_from<unsigned short>();
        }
        virtual signed get_int() override {
            return read_from<signed>();
        }
        virtual unsigned get_uint() override {
            return read_from<unsigned>();
        }
        virtual long get_long() override {
            return read_from<long>();
        }
        virtual unsigned long get_ulong() override {
            return read_from<unsigned long>();
        }
        virtual long long get_llong() override {
            return read_from<long long>();
        }
        virtual unsigned long long get_ullong() override {
            return read_from<unsigned long long>();
        }
        virtual float get_float() override {
            return read_from<float>();
        }
        virtual double get_double() override {
            return read_from<double>();
        }
        virtual long double get_ldouble() override {
            return read_from<long double>();
        }

        void clear() {
            this->buffer.clear();
            this->read_offset = 0;
        }

        /**
         * @return 返回只读，不拥有所有权的缓冲区。
         * 在下次调用非const成员函数之后失效。
        */
        std::span<const std::byte> get() const {
            const auto buffer =
                reinterpret_cast<const std::byte *>(this->buffer.data());
            const auto size = this->buffer.size();

            return std::span{ buffer, buffer + size };
        }
    };

}   // namespace utopia::core
