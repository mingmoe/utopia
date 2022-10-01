//===------------------------- stream.hpp -------------------------===//
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
/// 实现了一个输入，输出流接口。
/// 并且实现了一个内存流。
//===--------------------------------------------------------------===//

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
     * @brief 简单流接口。用户只需要实现read和wirte即可使用。
    */
    class SimplyStream : public Stream {
        std::size_t read_offset{ 0 };

      public:

        virtual void write_to(const std::span<std::byte> data)           = 0;

        virtual const std::span<std::byte> read_from(std::size_t offset,
                                                     std::size_t length) = 0;

        virtual void                       flush() override {}
        virtual void                       close() override {}

        //========== WRITE ==========//
        virtual void write(char v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(unsigned char v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(short v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(unsigned short v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(signed v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(unsigned v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(long v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(unsigned long v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(long long v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(unsigned long long v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(float v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(double v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(long double v) override {
            write_to(std::span<std::byte>(reinterpret_cast<std::byte *>(&v),
                                          sizeof(v)));
        }
        virtual void write(std::span<std::byte> v) override {
            for(auto c : v) {
                write(std::to_integer<unsigned char>(c));
            }
        }

        //========== READ ==========//
        virtual std::span<std::byte> get_bytes(size_t length) override {
            auto s = read_from(read_offset, length);
            read_offset += length;
            return s;
        }
        virtual char get_char() override {
            const auto length = sizeof(this->get_char());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return std::to_integer<char>(s[0]);
        }
        virtual unsigned char get_uchar() override {
            const auto length = sizeof(this->get_uchar());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return std::to_integer<unsigned char>(s[0]);
        }
        virtual short get_short() override {
            const auto length = sizeof(this->get_short());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<short *>((s.data()));
        }
        virtual unsigned short get_ushort() override {
            const auto length = sizeof(this->get_ushort());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<unsigned short *>((s.data()));
        }
        virtual signed get_int() override {
            const auto length = sizeof(this->get_int());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<signed *>((s.data()));
        }
        virtual unsigned get_uint() override {
            const auto length = sizeof(this->get_uint());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<unsigned *>((s.data()));
        }
        virtual long get_long() override {
            const auto length = sizeof(this->get_long());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<long *>((s.data()));
        }
        virtual unsigned long get_ulong() override {
            const auto length = sizeof(this->get_ulong());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<unsigned long *>((s.data()));
        }
        virtual long long get_llong() override {
            const auto length = sizeof(this->get_llong());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<long long *>((s.data()));
        }
        virtual unsigned long long get_ullong() override {
            const auto length = sizeof(this->get_ullong());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<unsigned long long *>((s.data()));
        }
        virtual float get_float() override {
            const auto length = sizeof(this->get_float());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<float *>((s.data()));
        }
        virtual double get_double() override {
            const auto length = sizeof(this->get_double());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<double *>((s.data()));
        }
        virtual long double get_ldouble() override {
            const auto length = sizeof(this->get_ldouble());
            auto       s      = read_from(read_offset, length);
            read_offset += length;
            return *reinterpret_cast<long double *>((s.data()));
        }

        void clear() {
            this->read_offset = 0;
        }
    };

    /**
     * @brief 内存流
    */
    class MemoryStream : public SimplyStream {
        std::vector<std::byte> datas;

      public:

        virtual void write_to(const std::span<std::byte> data) {
            datas.reserve(datas.size() + data.size());

            for(auto &b : data) {
                datas.push_back(b);
            }
        }

        virtual const std::span<std::byte> read_from(std::size_t offset,
                                                     std::size_t length) {
            if(offset + length > datas.size()) {
                throw utopia::core::OutOfRangeException{
                    "MemoryStream:read out of range"
                };
            }

            return std::span<std::byte>(datas.data() + offset, length);
        }
    };

}   // namespace utopia::core
