// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
//===---------------------- main.cpp -----------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===-------------------------------------------------------===//

#include "utopia/exception.hpp"
#include "utopia/iostream.hpp"

auto main() -> int {
    auto exception = utopia::core::IOException("unknown exception");
    utopia::core::write_stderr(exception.what());

    return 0;
}
