#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#   Copyright (C) 2021-2022 mingmoe(me@kawayi.moe)(https://blog.kawayi.moe)
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Affero General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Affero General Public License for more details.
#
#   You should have received a copy of the GNU Affero General Public License
#   along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
    "include/utopia/world.hpp"
    "include/utopia/serializable.hpp"
    "include/utopia/stream.hpp"
    "include/utopia/scope_guard.hpp"
    "include/utopia/init.hpp"
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

# ref:https://cmake.org/cmake/help/latest/module/FindOpenSSL.html
if(TARGET OpenSSL::applink)
    target_link_libraries(${UTOPIA_CORE} PRIVATE OpenSSL::applink)
endif()
target_link_libraries(${UTOPIA_CORE} PUBLIC OpenSSL::SSL)
target_link_libraries(${UTOPIA_CORE} PUBLIC OpenSSL::Crypto)


# testing
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
