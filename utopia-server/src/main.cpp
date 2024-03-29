//===-------------------------- main.cpp --------------------------===//
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

#include "utopia/exception.hpp"
#include "utopia/init.hpp"
#include "utopia/iostream.hpp"

auto main() -> int {
    utopia::core::setup_icu_data();
    auto root      = utopia::core::IOException("ROOT EXCEPTION");
    auto children  = utopia::core::IOException("CHILDREN EXCEPTION",
                                              std::make_exception_ptr(root));
    auto exception =
        utopia::core::IOException("exception", std::make_exception_ptr(children));


    utopia::core::write_stderr(exception.what());

    return 0;
}
