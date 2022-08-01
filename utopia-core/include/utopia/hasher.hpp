//===--------------------- hasher.hpp ---------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//
/// \file
/// 这个文件声明了一个哈希器接口和一个hashable接口。
/// 并实现了一个xxHash3的哈希器。
//===------------------------------------------------------===//

#pragma once

#include "utopia/exception.hpp"
#include <cstdint>
#include <string_view>
#include <vector>
#include <xxhash.h>

namespace utopia::core {

    struct Hasher {
        Hasher()                               = default;
        virtual ~Hasher()                      = default;

        Hasher(const Hasher &)                 = delete;
        Hasher(Hasher &&)                      = delete;

        Hasher      &operator=(const Hasher &) = delete;
        Hasher      &operator=(Hasher &&)      = delete;

        virtual void reset()                   = 0;
        virtual void write(char)               = 0;
        virtual void write(unsigned char)      = 0;
        virtual void write(short)              = 0;
        virtual void write(unsigned short)     = 0;
        virtual void write(signed)             = 0;
        virtual void write(unsigned)           = 0;
        virtual void write(long)               = 0;
        virtual void write(unsigned long)      = 0;
        virtual void write(long long)          = 0;
        virtual void write(unsigned long long) = 0;
        virtual void write(float)              = 0;
        virtual void write(double)             = 0;
        virtual void write(long double)        = 0;
        virtual void write(std::string_view)   = 0;

        /**
         *  @brief get the hash result
         * @note workout won't reset
        */
        virtual std::uint64_t workout() = 0;

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
        virtual void get_hash(Hasher &) = 0;
    };

    /**
     * @brief 使用XXHash3的Hasher实现。
     * 这个Hasher应该是足够通用，并且是最快的。
     * @note 不会有哪个傻宝拿他加密吧？
    */
    class XXHashHasher : public Hasher {
        XXH3_state_t *state = XXH3_createState();

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
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(unsigned char v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(short v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(unsigned short v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(signed v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(unsigned v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(long v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(unsigned long v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(long long v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(unsigned long long v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(float v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(double v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(long double v) override {
            if(XXH3_64bits_update(state, &v, sizeof(v)) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }
        virtual void write(std::string_view v) override {
            if(XXH3_64bits_update(state, v.data(), v.size()) == XXH_ERROR) {
                throw RuntimeException{ "failed to update xxhash3" };
            }
        }

        virtual void reset() override {
            if(XXH3_64bits_reset(state) == XXH_ERROR) {
                throw RuntimeException{ "failed to reset xxhash3" };
            }
        }

        virtual std::uint64_t workout() override {
            auto result = XXH3_64bits_digest(state);

            return result;
        }
    };

}   // namespace utopia::core
