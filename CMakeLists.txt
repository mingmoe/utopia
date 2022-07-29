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
u_enable_strict_wanrnings()
u_add_subdirectory_at_root ("utopia-core")
u_add_subdirectory_at_root ("utopia-client")
u_add_subdirectory_at_root ("utopia-server")

# another 
option(U_OPT_PVS_ENABLED "enable PVS check?" OFF)

if(U_OPT_PVS_ENABLED)
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/pvs-studio-cmake-module/PVS-Studio.cmake")

    if(MSVC)
        set(EXT_FLAGS CXX_FLAGS "/utf-8")
    endif()

    pvs_studio_add_target(
        TARGET core.analyze 
        OUTPUT FORMAT json
        ANALYZE "${UTOPIA_CORE}"
        LOG "pvs.core.log"
        ARGS -e "${UTOPIA_LIBRARY}/*"
        ${EXT_FLAGS}
    )
    pvs_studio_add_target(
        TARGET client.analyze 
        OUTPUT FORMAT json
        ANALYZE "${UTOPIA_CLIENT}"
        LOG "pvs.client.log"
        ARGS -e "${UTOPIA_LIBRARY}/*"
        ${EXT_FLAGS}
    )
    pvs_studio_add_target(
        TARGET server.analyze 
        OUTPUT FORMAT json
        ANALYZE "${UTOPIA_SERVER}"
        LOG "pvs.server.log"
        ARGS -e "${UTOPIA_LIBRARY}/*"
        ${EXT_FLAGS}
    )
endif(U_OPT_PVS_ENABLED)
#==============================================
# 添加cmake自定义项目
# 这主要用于IDE管理cmake文件
source_group(TREE ${CMAKE_SOURCE_DIR} PREFIX CMake FILES ${U_TOTAL_CMAKE_FILES})

add_custom_target(
    "cmakefiles"
    SOURCES ${U_TOTAL_CMAKE_FILES}
)
