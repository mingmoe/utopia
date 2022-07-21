#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

add_executable(${UTOPIA_SERVER}
    "src/main.cpp"
)

target_include_directories(${UTOPIA_SERVER} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${UTOPIA_SERVER} PUBLIC "${UTOPIA_CORE}")
target_link_libraries(${UTOPIA_CLIENT} PUBLIC zlib)
