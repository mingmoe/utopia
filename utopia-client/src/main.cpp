//===---------------------- main.cpp ----------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//

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
