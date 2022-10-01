//===----------------------- scope_guard.hpp ----------------------===//
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
/// 实现了一个guard scope
//===--------------------------------------------------------------===//

#pragma once

#include <functional>
#include <mutex>
#include <utility>

namespace utopia::core {

    template<class T>
    class SourceGuard {
      private:

        std::once_flag            flag;
        std::function<void(T &&)> func;
        T                         src;

      public:

        template<class FUNC>
        SourceGuard(FUNC &&t, T &&source) :
            src(std::forward(source)), func(std::forward(t)) {}

        ~SourceGuard() {
            std::call_once(flag, func);
        }

        T &get_source() {
            return this->src;
        }

        SourceGuard(const SourceGuard &)            = delete;
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
