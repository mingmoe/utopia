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

#include "utopia/guuid.hpp"
#include "utopia/init.hpp"
#include "utopia/iostream.hpp"

int main() {
    utopia::core::setup_icu_data();

    for(auto i = 0; i != 10; i++) {
        utopia::core::write_stdout(
            utopia::core::Guuid::new_unique().to_string() + "\n");
    }


    return 0;
}
