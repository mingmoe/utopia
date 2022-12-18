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
#include <boost/safe_numerics/safe_integer.hpp>
#include <concepts>
#include <cstddef>
#include <optional>
#include <span>
#include <string_view>
#include <type_traits>
#include <vector>

namespace utopia::core {

    enum class OffsetPosition {
        Current,
        Begin,
        End
    };


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
        virtual void set_write_offset(OffsetPosition pos,
                                      std::ptrdiff_t offset) = 0;
        virtual void write_bytes(std::span<std::byte>)       = 0;
    };

    /**
     * @brief 输入流
    */
    struct InputStream : public StreamBase {
        virtual void                   set_read_offset(OffsetPosition pos,
                                                       std::ptrdiff_t offset) = 0;
        virtual std::vector<std::byte> get_bytes(size_t length) = 0;
    };

    struct Stream : public OutputStream, public InputStream {};

    /**
     * @brief 简单流接口。用户只需要实现read和wirte即可使用。
    */
    class SimplyStream : public Stream {
        ::boost::safe_numerics::safe<std::size_t> write_offset, read_offset;

        void modify_offset(OffsetPosition                             pos,
                           std::ptrdiff_t                             offset,
                           ::boost::safe_numerics::safe<std::size_t> &value) {
            auto l = this->get_length();

            if(!l.has_value() && pos == OffsetPosition::End) {
                throw UnrealizedException{
                    "no supported get length of stream"
                };
            }

            ::boost::safe_numerics::safe<std::size_t> v;

            switch(pos) {
                case OffsetPosition::Begin:
                    value = offset;
                    break;
                case OffsetPosition::Current:
                    value += offset;
                    break;
                case OffsetPosition::End:
                    v     = l.value();
                    value = v + offset;
                    break;
            }
        }

        virtual void                         write_to(std::size_t                offset,
                                                      const std::span<std::byte> data) = 0;

        virtual const std::vector<std::byte> read_from(std::size_t offset,
                                                       std::size_t length) = 0;

      public:

        /**
         * @brief 获取流的长度，如果不支持（如未知或无限长度，则返回std::nullopt）
        */
        virtual std::optional<std::size_t> get_length() = 0;

        virtual void                       flush() override {}
        virtual void                       close() override {}

        virtual void InputStream::set_read_offset(OffsetPosition pos,
                                                  std::ptrdiff_t offset) {
            this->modify_offset(pos, offset, this->read_offset);
        }

        virtual void OutputStream::set_write_offset(OffsetPosition pos,
                                                    std::ptrdiff_t offset) {
            this->modify_offset(pos, offset, this->write_offset);
        }

        //========== WRITE ==========//
        virtual void write_bytes(std::span<std::byte> v) override {
            write_to(write_offset, v);
            this->write_offset += v.size();
        }

        template<class T>
        void write(T value) {
            static_assert(std::is_integral_v<T> == true ||
                          std::is_floating_point_v<T> == true ||
                          std::is_same_v<T, bool> == true);

            auto                 ptr = reinterpret_cast<std::byte *>(&value);

            std::span<std::byte> buf(ptr, ptr + sizeof(T));

            write_bytes(buf);
        }


        //========== READ ==========//
        virtual std::vector<std::byte> get_bytes(size_t length) override {
            auto s = read_from(read_offset, length);
            read_offset += length;
            return s;
        }

        template<class T>
        T get() {
            static_assert(std::is_integral_v<T> == true ||
                          std::is_floating_point_v<T> == true ||
                          std::is_same_v<T, bool> == true);

            auto v = get_bytes(sizeof(T));

            return *reinterpret_cast<T *>(v.data());
        }
    };

    /**
     * @brief 内存流
    */
    class MemoryStream : public SimplyStream {
        std::vector<std::byte> datas;

        virtual void           SimplyStream::write_to(std::size_t                offset,
                                            const std::span<std::byte> data) {
            datas.resize(offset + data.size());

            memcpy(datas.data() + offset, data.data(), data.size());
        }

        virtual const std::vector<std::byte>
            SimplyStream::read_from(std::size_t offset, std::size_t length) {
            if(offset + length > datas.size()) {
                throw utopia::core::OutOfRangeException{
                    "MemoryStream:read out of range"
                };
            }

            return std::vector<std::byte>(datas.data() + offset,
                                          datas.data() + offset + length);
        }

      public:

        using SimplyStream::get;
        using SimplyStream::write;

        virtual std::optional<std::size_t> SimplyStream::get_length() {
            return datas.size();
        }
    };

}   // namespace utopia::core
