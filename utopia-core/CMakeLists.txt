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
    "include/utopia/source_location.hpp"
    "include/utopia/guuid.hpp"
    "include/utopia/entity.hpp"
    "include/utopia/hasher.hpp"
    "src/iostream.cpp")

target_include_directories(${UTOPIA_CORE} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${UTOPIA_CORE} PUBLIC ${UTOPIA_CONFIG})
target_link_libraries(${UTOPIA_CORE} PUBLIC fmt::fmt)
target_link_libraries(${UTOPIA_CORE} PUBLIC eigen)
target_link_libraries(${UTOPIA_CORE} PUBLIC asio)
target_link_libraries(${UTOPIA_CORE} PUBLIC boost)
target_link_libraries(${UTOPIA_CORE} PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${UTOPIA_CORE} PUBLIC icu)
target_link_libraries(${UTOPIA_CORE} PUBLIC xxhash)

add_executable(${UTOPIA_CORE_TEST} "test/guuid.cpp" "test/main.cpp")
target_link_libraries(${UTOPIA_CORE_TEST} PUBLIC doctest ${UTOPIA_CORE})
doctest_discover_tests(${UTOPIA_CORE_TEST})

target_include_directories(${UTOPIA_CORE_TEST} PUBLIC "${UTOPIA_LIBRARY}/doctest/doctest")

# enable strict warning
if(MSVC)
    target_compile_options(${UTOPIA_CORE} PUBLIC "/W4")
else()
    target_compile_options(${UTOPIA_CORE} PUBLIC "-Wall" "-Wextra")
endif()
