//===------------------- scope_guard.hpp ------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//
/// \file
/// 实现了一个guard scope
//===------------------------------------------------------===//

#pragma once

#include <functional>
#include <mutex>
#include <utility>

namespace utopia::core {

    class ScopeGuard {

        bool                  called{ false };
        std::function<void()> call;

      public:

        template<class T>
        ScopeGuard(std::function<void()> &&t) : call(std::forward(t)) {}

        ~ScopeGuard() {
            if(!called) {
                called = true;
                this->call.operator()();
            }
        }

        ScopeGuard(const ScopeGuard &) = delete;
        ScopeGuard(ScopeGuard &&)      = delete;
        ScopeGuard &operator=(const ScopeGuard &) = delete;
        ScopeGuard &operator=(ScopeGuard &&) = delete;
    };

    template<class T>
    class SourceGuard {
      private:

        bool                      called{ false };
        std::function<void(T &&)> func;
        T                         src;

      public:

        template<class FUNC>
        SourceGuard(FUNC &&t, T &&source) : src(source) {
            this->func = std::forward(t);
        }

        ~SourceGuard() {
            if(!called) {
                called = true;
                func(std::move(src));
            }
        }

        T &get_source() {
            return this->src;
        }

        SourceGuard(const SourceGuard &) = delete;
        SourceGuard &operator=(const SourceGuard &) = delete;
        SourceGuard(SourceGuard &&o) noexcept {
            *this = std::move(0);
        }
        SourceGuard &operator=(SourceGuard &&other) noexcept {
            if(this == other) {
                return *this;
            }

            this->~SourceGuard();

            this->called = other.called;
            this->src    = std::move(other.src);
            this->func   = std::move(other.func);

            other.called = true;

            return *this;
        }
    };


}   // namespace utopia::core
