#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

if(U_DEBUG_MODE)
    set(U_LIBRARY_DEBUG_END "d")
else()
    set(U_LIBRARY_DEBUG_END "")
endif()

if(U_UNDER_WINDOWS)
    set(U_LIBRARY_FILE_EXT ".lib")
elseif(U_UNDER_LINUX)
    set(U_LIBRARY_FILE_EXT ".a")
else()
    message(FATAL_ERROR "unknown platform")
endif()

set(U_LIBRARY_END "${U_LIBRARY_DEBUG_END}${U_LIBRARY_FILE_EXT}")
unset(U_LIBRARY_DEBUG_END)
unset(U_LIBRARY_FILE_EXT)

#===== zlib =====
add_library(zlib STATIC IMPORTED)

set(ZLIB_INCLUDE_DIR "${UTOPIA_SYSROOT}/zlib/include")
set(ZLIB_LIBRARY "${UTOPIA_SYSROOT}/zlib/lib/zlibstatic${U_LIBRARY_END}")

message(STATUS "using ${ZLIB_LIBRARY}")

set_property(TARGET zlib PROPERTY
             IMPORTED_LOCATION "${ZLIB_LIBRARY}")
target_include_directories(zlib INTERFACE "${ZLIB_INCLUDE_DIR}")
#================

#===== libpng =====
add_library(libpng STATIC IMPORTED)

set(PNG_PNG_INCLUDE_DIR "${UTOPIA_SYSROOT}/libpng/include")
set(PNG_LIBRARY "${UTOPIA_SYSROOT}/libpng/lib/libpng16_static${U_LIBRARY_END}")

message(STATUS "using ${PNG_LIBRARY}")

set_property(TARGET libpng PROPERTY
             IMPORTED_LOCATION "${PNG_LIBRARY}")
target_include_directories(libpng INTERFACE "${PNG_PNG_INCLUDE_DIR}")
#==================

#===== harfbuzz =====
set(U_HARFBUZZ_INSTALL_DIR "${UTOPIA_SYSROOT}/harfbuzz")

add_library(harfbuzz STATIC IMPORTED)

set_property(TARGET harfbuzz PROPERTY
             IMPORTED_LOCATION "${U_HARFBUZZ_INSTALL_DIR}/lib/libharfbuzz.a")
target_include_directories(harfbuzz INTERFACE "${U_HARFBUZZ_INSTALL_DIR}/include")
#====================

#===== boost =====
add_library("boost" INTERFACE)
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/config/include")
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/core/include")
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/winapi/include")
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/type_traits/include")
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/predef/include")
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/array/include")
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/static_assert/include")
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/assert/include")
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/container_hash/include")
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/throw_exception/include")
target_include_directories(boost SYSTEM INTERFACE "${UTOPIA_LIBRARY}/boost/libs/stacktrace/include")

if(U_UNDER_LINUX OR U_UNDER_APPLE)
    message(STATUS "link with lib-${CMAKE_DL_LIBS} for stacktrace")
    target_link_directories(boost INTERFACE ${CMAKE_DL_LIBS})
endif()
#=================

unset(U_LIBRARY_END)
