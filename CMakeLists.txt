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
cmake_minimum_required (VERSION 3.23)

message(STATUS 
"Cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

# init
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/base.cmake")
list(APPEND U_TOTAL_CMAKE_FILES "${CMAKE_CURRENT_LIST_FILE}")

u_include_at_root("${CMAKE_CURRENT_SOURCE_DIR}/cmake/define.cmake")

#==============================================

project ("utopia" VERSION ${UTOPIA_VERSION} LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#==============================================

u_include_at_root("${CMAKE_CURRENT_SOURCE_DIR}/cmake/checker.cmake")
u_include_at_root("${CMAKE_CURRENT_SOURCE_DIR}/cmake/options.cmake")
u_include_at_root("${CMAKE_CURRENT_SOURCE_DIR}/cmake/library.cmake")
u_include_at_root("${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake")

u_open_clang_tidy()
u_add_subdirectory_at_root ("utopia-core")
u_add_subdirectory_at_root ("utopia-client")
u_add_subdirectory_at_root ("utopia-server")

u_include_at_root("${CMAKE_CURRENT_SOURCE_DIR}/cmake/install.cmake")

#==============================================
# 添加cmake自定义项目
# 这主要用于IDE管理cmake文件
source_group(TREE ${CMAKE_SOURCE_DIR} PREFIX CMake FILES ${U_TOTAL_CMAKE_FILES})

add_custom_target(
    "cmakefiles"
    SOURCES ${U_TOTAL_CMAKE_FILES}
)
