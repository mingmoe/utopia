//===------------------------- hasher.hpp -------------------------===//
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
/// 这个文件声明了一个哈希器接口和一个hashable接口。
/// 并实现了一个xxHash3的哈希器。
/// 并且为utopia::core::Hashable实现了std::hash。
//===--------------------------------------------------------------===//

#pragma once

#include "utopia/exception.hpp"
#include "utopia/scope_guard.hpp"
#include "utopia/stream.hpp"
#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>
#include <xxhash.h>

namespace utopia::core {

    using hash_t = unsigned long long;

    struct Hasher : public OutputStream {
        Hasher()                               = default;
        virtual ~Hasher()                      = default;

        Hasher(const Hasher &)                 = delete;
        Hasher(Hasher &&)                      = delete;

        Hasher      &operator=(const Hasher &) = delete;
        Hasher      &operator=(Hasher &&)      = delete;

        virtual void reset()                   = 0;

        virtual void flush() override {}

        virtual void close() override {}

        /**
         *  @brief get the hash result
         * @note workout won't reset
        */
        virtual hash_t workout() = 0;

        /**
         * @brief 用于遍历容器的write
         * @tparam B 开头迭代器类型
         * @tparam E 末尾迭代器类型
         * @param begin 开头迭代器
         * @param end 末尾迭代器
        */
        template<class B, class E>
        void write_range(B begin, E end) {
            for(; begin != end; begin++) {
                write(*begin);
            }
        }
    };

    /**
     * @brief 表示一个可以进行hash的对象
    */
    struct Hashable {
        virtual void get_hash(Hasher &) const = 0;
    };

    /**
     * @brief 使用XXHash3的Hasher实现。
     * 这个Hasher应该是足够通用，并且是最快的。
     * @note 不会有哪个傻宝拿他加密吧？
    */
    class XXHashHasher : public Hasher {
        XXH3_state_t *state = XXH3_createState();

        template<class T>
        void write_to(T v) {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }

      public:

        XXHashHasher() {
            if(state == nullptr) {
                throw RuntimeException{ "failed to init xxhash3" };
            }

            reset();
        }

        virtual ~XXHashHasher() override {
            auto err = XXH3_freeState(state);

            if(err == XXH_ERROR) {
                throw RuntimeException{ "failed to free xxhash3" };
            }
        }

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
        virtual void write(std::span<std::byte> bytes) override {
            for(auto byte : bytes) {
                write_to(byte);
            }
        }

        virtual void reset() override {
            if(XXH3_64bits_reset(state) == XXH_ERROR) {
                throw RuntimeException{ "failed to reset xxhash3" };
            }
        }

        virtual hash_t workout() override {
            auto result = XXH3_64bits_digest(state);

            return result;
        }
    };

}   // namespace utopia::core

template<>
struct std::hash<utopia::core::Hashable> {
    std::size_t operator()(utopia::core::Hashable const &s) const noexcept {
        thread_local std::unique_ptr<utopia::core::XXHashHasher> l_hasher{
            nullptr
        };

        if(l_hasher.get() == nullptr) {
            l_hasher =
                std::move(std::make_unique<utopia::core::XXHashHasher>());
        }

        l_hasher->reset();

        s.get_hash(*l_hasher.get());
        return l_hasher->workout();
    }
};
