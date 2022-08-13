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

#=====================
# get compiler version
#=====================
string(REPLACE "." ";" VERSION_LIST ${CMAKE_CXX_COMPILER_VERSION})
# for gcc\clang: x.y.z
list(GET VERSION_LIST 0 UC_CXXCOMPILER_VERSION_MAJOR)
list(GET VERSION_LIST 1 UC_CXXCOMPILER_VERSION_MINOR)
list(GET VERSION_LIST 2 UC_CXXCOMPILER_VERSION_PATCH)

if(U_USE_MSVC)
    # for msvc: x.y.z.w
    list(GET VERSION_LIST 3 UC_CXXCOMPILER_VERSION_BUILD)
else()
    # set w to 0 for gcc\clang
    set(UC_CXXCOMPILER_VERSION_BUILD 0)
endif()

set(UC_CXXCOMPILER_VERSION_MAJOR "@UC_CXXCOMPILER_VERSION_MAJOR@\n${UC_CXXCOMPILER_VERSION_MAJOR}")
set(UC_CXXCOMPILER_VERSION_MINOR "@UC_CXXCOMPILER_VERSION_MINOR@\n${UC_CXXCOMPILER_VERSION_MINOR}")
set(UC_CXXCOMPILER_VERSION_PATCH "@UC_CXXCOMPILER_VERSION_PATCH@\n${UC_CXXCOMPILER_VERSION_PATCH}")
set(UC_CXXCOMPILER_VERSION_BUILD "@UC_CXXCOMPILER_VERSION_BUILD@\n${UC_CXXCOMPILER_VERSION_BUILD}")

unset(VERSION_LIST)

#=====================
# get project version
#=====================
set(UC_PROJECT_VERSION_MAJOR "@UC_PROJECT_VERSION_MAJOR@\n${U_PROJECT_VERSION_MAJOR}")
set(UC_PROJECT_VERSION_MINOR "@UC_PROJECT_VERSION_MINOR@\n${U_PROJECT_VERSION_MINOR}")
set(UC_PROJECT_VERSION_PATCH "@UC_PROJECT_VERSION_PATCH@\n${U_PROJECT_VERSION_PATCH}")
set(UC_PROJECT_VERSION_BUILD "@UC_PROJECT_VERSION_BUILD@\n${U_PROJECT_VERSION_BUILD}")

#=====================
# configure file
#=====================
set(this_file_is_source "${CMAKE_CURRENT_SOURCE_DIR}/config.hpp")

configure_file(
    "${this_file_is_source}"
    "${CMAKE_CURRENT_BINARY_DIR}/utopia-config/include/utopia/config.hpp"
    NEWLINE_STYLE "LF"
    @ONLY
)

add_library(${UTOPIA_CONFIG} INTERFACE)
target_include_directories(${UTOPIA_CONFIG} INTERFACE 
"${CMAKE_CURRENT_BINARY_DIR}/utopia-config/include")

unset(this_file_is_source)
#====================================
unset(UC_CXXCOMPILER_VERSION_MAJOR)
unset(UC_CXXCOMPILER_VERSION_MINOR)
unset(UC_CXXCOMPILER_VERSION_PATCH)
unset(UC_CXXCOMPILER_VERSION_BUILD)
unset(UC_PROJECT_VERSION_MAJOR)
unset(UC_PROJECT_VERSION_MINOR)
unset(UC_PROJECT_VERSION_PATCH)
unset(UC_PROJECT_VERSION_BUILD)
