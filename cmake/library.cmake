#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#   Copyright (C) 2021-2022 mingmoe(me@kawayi.moe)(https://kawayi.moe)
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

set(SKIP_INSTALL_ALL ON)

#===== fmtlib =====
add_subdirectory("${UTOPIA_LIBRARY}/fmt" EXCLUDE_FROM_ALL)
#==================

#===== asio =====
add_library("asio" INTERFACE)
target_include_directories(asio SYSTEM INTERFACE "${UTOPIA_LIBRARY}/asio/asio/include")
#================

#===== nlohmann/json =====
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory("${UTOPIA_LIBRARY}/json" EXCLUDE_FROM_ALL)
u_add_define(
    "JSON_DIAGNOSTICS=1"
    "JSON_HAS_CPP_20=1"
    "JSON_HAS_FILESYSTEM=1"
    "JSON_USE_IMPLICIT_CONVERSIONS=1")
#=========================

#===== eigen =====
add_library("eigen" INTERFACE)
target_include_directories(eigen SYSTEM INTERFACE "${UTOPIA_LIBRARY}/eigen")
#=================

#===== SDL =====
find_package("SDL2" REQUIRED CONFIG)
#===============

#===== FreeType =====
find_package("freetype" REQUIRED CONFIG)
#====================

#===== libjpeg-turbo =====
find_package("libjpeg-turbo" REQUIRED CONFIG)
#=========================

#===== Ogg =====
find_package("Ogg" REQUIRED CONFIG)
#===============

#===== Vorbis =====
find_package("Vorbis" REQUIRED CONFIG)
#==================

#===== zstd =====
find_package("zstd" REQUIRED CONFIG)
#================

#===== OpenSSL =====
find_package("OpenSSL" 3.0.0 REQUIRED COMPONENTS Crypto SSL)
#===================

#===== CURL =====
find_package("CURL" REQUIRED)
#================

#======= ICU =======
# this library is compiled by ourselves
u_add_subdirectory_at_root("${UTOPIA_LIBRARY}/icu/" EXCLUDE_FROM_ALL)
#===================

#===== imgui ======
file(GLOB IMGUI_SOURCE CONFIGURE_DEPENDS "${UTOPIA_LIBRARY}/imgui/*.cpp")
add_library(imgui STATIC ${IMGUI_SOURCE})
unset(IMGUI_SOURCE)

target_include_directories(imgui PUBLIC "${UTOPIA_LIBRARY}/imgui")
target_include_directories(imgui PUBLIC "${UTOPIA_LIBRARY}/imgui/backends")

# backend
target_sources(imgui PUBLIC "${UTOPIA_LIBRARY}/imgui/backends/imgui_impl_sdl.cpp")
target_sources(imgui PUBLIC "${UTOPIA_LIBRARY}/imgui/backends/imgui_impl_sdlrenderer.cpp")

target_link_libraries(imgui PUBLIC SDL2::SDL2-static)
#==================

#===== doctest ======
enable_testing()
add_subdirectory("${UTOPIA_LIBRARY}/doctest" EXCLUDE_FROM_ALL)
include("${UTOPIA_LIBRARY}/doctest/scripts/cmake/doctest.cmake")
#====================

#===== xxHash ======
set(XXHASH_BUILD_ENABLE_INLINE_API ON)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory("${UTOPIA_LIBRARY}/xxHash/cmake_unofficial" EXCLUDE_FROM_ALL)
#===================

#===== openal-soft ======
set(ALSOFT_INSTALL OFF)
set(ALSOFT_INSTALL_CONFIG OFF)
set(ALSOFT_INSTALL_HRTF_DATA OFF)
set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF)
set(ALSOFT_INSTALL_EXAMPLES OFF)
set(ALSOFT_INSTALL_UTILS OFF)
add_subdirectory("${UTOPIA_LIBRARY}/openal-soft" EXCLUDE_FROM_ALL)
#========================

u_include_at_root("${CMAKE_CURRENT_SOURCE_DIR}/cmake/import-library.cmake")
