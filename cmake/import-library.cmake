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

include("${UTOPIA_SYSROOT}/library-info.cmake")

#===== zlib =====
message(STATUS "using ${zlib_LIBRARY}")

add_library(zlib STATIC IMPORTED)

set_property(TARGET zlib PROPERTY
             IMPORTED_LOCATION "${zlib_LIBRARY}")
target_include_directories(zlib INTERFACE "${zlib_INCLUDE_DIR}")
#================

#===== libpng =====
message(STATUS "using ${libpng_LIBRARY}")

add_library(libpng STATIC IMPORTED)

set_property(TARGET libpng PROPERTY
             IMPORTED_LOCATION "${libpng_LIBRARY}")
target_include_directories(libpng INTERFACE "${libpng_INCLUDE_DIR}")
#==================

#===== harfbuzz =====
set(U_HARFBUZZ_INSTALL_DIR "${UTOPIA_SYSROOT}/harfbuzz")

add_library(harfbuzz STATIC IMPORTED)

set_property(TARGET harfbuzz PROPERTY
             IMPORTED_LOCATION "${U_HARFBUZZ_INSTALL_DIR}/lib/libharfbuzz.a")
target_include_directories(harfbuzz INTERFACE "${U_HARFBUZZ_INSTALL_DIR}/include")
#====================

#===== FLAC =====
message(STATUS "using ${flac_LIBRARY}")

add_library(flac INTERFACE)
target_include_directories(flac INTERFACE "${flac_INCLUDE_DIR}")
target_link_libraries(flac INTERFACE "${flac_LIBRARY}")

#================

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

if(U_UNDER_LINUX OR U_UNDER_APPLE OR U_ANDROID_MODE)
    message(STATUS "link with ${CMAKE_DL_LIBS} for stacktrace")
    target_link_directories(boost INTERFACE ${CMAKE_DL_LIBS})
endif()
#=================
