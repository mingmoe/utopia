//===------------------------ -abort.cpp -------------------------===//
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

#include "utopia/abort.hpp"

#include "utopia/exception.hpp"
#include <string_view>

using namespace utopia::core;


[[noreturn]] inline void utopia::core::abort_with_exception(
    Exception       err,
    source_location location) {
    auto message = std::string_view{ err.what() };

    u_assert(false, message, location);
    unreachable("should unreachable");
}
