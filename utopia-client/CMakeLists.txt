#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

add_executable(${UTOPIA_CLIENT} 
    "src/main.cpp"
)

target_include_directories(${UTOPIA_CLIENT} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${UTOPIA_CLIENT} PUBLIC "${UTOPIA_CORE}")
target_link_libraries(${UTOPIA_CLIENT} PUBLIC zlib)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC libpng)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC SDL2::SDL2-static)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC freetype)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC icu)
#copy_icu_library_for(${UTOPIA_CLIENT})
