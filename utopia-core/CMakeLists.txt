#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

add_library(${UTOPIA_CORE} STATIC
    "include/utopia/abort.hpp"
    "include/utopia/iostream.hpp"
    "include/utopia/template.hpp"
    "include/utopia/exception.hpp"
    "src/iostream.cpp")

target_include_directories(${UTOPIA_CORE} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${UTOPIA_CORE} PUBLIC fmt::fmt)
target_link_libraries(${UTOPIA_CORE} PUBLIC eigen)
target_link_libraries(${UTOPIA_CORE} PUBLIC boost)
target_link_libraries(${UTOPIA_CORE} PUBLIC nlohmann_json::nlohmann_json)
