#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
cmake_minimum_required (VERSION 3.22)

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

#==============================================
# 添加cmake自定义项目
# 这主要用于IDE管理cmake文件
source_group(TREE ${CMAKE_SOURCE_DIR} PREFIX CMake FILES ${U_TOTAL_CMAKE_FILES})

add_custom_target(
    "cmakefiles"
    SOURCES ${U_TOTAL_CMAKE_FILES}
)
