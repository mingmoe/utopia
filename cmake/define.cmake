#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

set(UTOPIA_VERSION "0.1.0")

set(UTOPIA_CORE "utopia-core")
set(UTOPIA_CLIENT "utopia-client")
set(UTOPIA_SERVER "utopia-server")

set(UTOPIA_LIBRARY "${CMAKE_SOURCE_DIR}/library")

if(NOT DEFINED UTOPIA_SYSROOT)
    set(UTOPIA_SYSROOT "${UTOPIA_LIBRARY}/install-tree")
    message(STATUS "using ${UTOPIA_SYSROOT} as SYSROOT")
endif()

set(CMAKE_PREFIX_PATH "${UTOPIA_SYSROOT}")
list(APPEND CMAKE_PREFIX_PATH "${UTOPIA_SYSROOT}/SDL")
list(APPEND CMAKE_PREFIX_PATH "${UTOPIA_SYSROOT}/freetype")
