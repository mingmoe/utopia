#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
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

# close all warnings
target_compile_options(imgui PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W1>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-W>
)
#==================

#===== doctest ======
enable_testing()
add_subdirectory("${UTOPIA_LIBRARY}/doctest" EXCLUDE_FROM_ALL)
include("${UTOPIA_LIBRARY}/doctest/scripts/cmake/doctest.cmake")
#====================

u_include_at_root("${CMAKE_CURRENT_SOURCE_DIR}/cmake/import-library.cmake")
