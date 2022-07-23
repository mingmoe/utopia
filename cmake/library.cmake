#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

set(SKIP_INSTALL_ALL ON)

#===== fmtlib =====
add_subdirectory("${UTOPIA_LIBRARY}/fmt" EXCLUDE_FROM_ALL)
#==================

#===== nlohmann/json =====
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
u_add_define(
    "JSON_DIAGNOSTICS=1"
    "JSON_HAS_CPP_20"
    "JSON_HAS_FILESYSTEM"
    "JSON_USE_IMPLICIT_CONVERSIONS=1")
add_subdirectory("${UTOPIA_LIBRARY}/json" EXCLUDE_FROM_ALL)
#=========================

#===== eigen =====
add_library("eigen" INTERFACE)
target_include_directories(eigen SYSTEM INTERFACE "${UTOPIA_LIBRARY}/eigen")
#=================

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

#===== zlib =====
add_library(zlib INTERFACE)

set(ZLIB_INCLUDE_DIR "${UTOPIA_SYSROOT}/zlib/include")
set(ZLIB_LIBRARY "${UTOPIA_SYSROOT}/zlib/lib/zlibstatic.lib")

target_include_directories(zlib INTERFACE "${ZLIB_INCLUDE_DIR}")
target_link_libraries(zlib INTERFACE "${ZLIB_LIBRARY}")
#================

#===== libpng =====
add_library(libpng INTERFACE)

set(PNG_PNG_INCLUDE_DIR "${UTOPIA_SYSROOT}/libpng/include")
set(PNG_LIBRARY "${UTOPIA_SYSROOT}/libpng/lib/libpng16_static.lib")

target_include_directories(libpng INTERFACE "${PNG_PNG_INCLUDE_DIR}")
target_link_libraries(libpng INTERFACE "${PNG_LIBRARY}")
#==================

#===== SDL =====
set(SDL2_DIR "${UTOPIA_SYSROOT}/SDL/cmake")
find_package("SDL2" REQUIRED CONFIG)
#===============

#===== FreeType =====
set(freetype_DIR "${UTOPIA_SYSROOT}/freetype/lib/cmake/freetype")
find_package("freetype" REQUIRED CONFIG)
#====================


#======= ICU =======
# this library is compiled
add_library(icu INTERFACE)
if(U_UNDER_WINDOWS)
    target_include_directories(icu INTERFACE "${UTOPIA_LIBRARY}/icu/windows/include")
    target_link_libraries(icu INTERFACE "${UTOPIA_LIBRARY}/icu/windows/lib64/icudt.lib")
    target_link_libraries(icu INTERFACE "${UTOPIA_LIBRARY}/icu/windows/lib64/icuin.lib")
    target_link_libraries(icu INTERFACE "${UTOPIA_LIBRARY}/icu/windows/lib64/icuio.lib")
    target_link_libraries(icu INTERFACE "${UTOPIA_LIBRARY}/icu/windows/lib64/icutu.lib")
    target_link_libraries(icu INTERFACE "${UTOPIA_LIBRARY}/icu/windows/lib64/icuuc.lib")

    function(copy_icu_library_for ARG)
        add_custom_command(TARGET ${ARG} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${UTOPIA_LIBRARY}/icu/windows/bin64"
            $<TARGET_FILE_DIR:${ARG}>)
    endfunction()
elseif(U_UNDER_LINUX)
    target_include_directories(icu INTERFACE "${UTOPIA_LIBRARY}/icu/linux/include")
    target_link_libraries(icu INTERFACE "${UTOPIA_LIBRARY}/icu/linux/lib64/libicudata.so.71.1")
    target_link_libraries(icu INTERFACE "${UTOPIA_LIBRARY}/icu/linux/lib64/libicui18n.so.71.1")
    target_link_libraries(icu INTERFACE "${UTOPIA_LIBRARY}/icu/linux/lib64/libicuio.so.71.1")
    target_link_libraries(icu INTERFACE "${UTOPIA_LIBRARY}/icu/linux/lib64/libicutu.so.71.1")
    target_link_libraries(icu INTERFACE "${UTOPIA_LIBRARY}/icu/linux/lib64/libicuuc.so.71.1")

    function(copy_icu_library_for ARG)
        add_custom_command(TARGET ${ARG} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${UTOPIA_LIBRARY}/icu/linux/lib64/"
            $<TARGET_FILE_DIR:${ARG}>)
    endfunction()
else()
    message(FATAL_ERROR 
    "your platform is not support because we haven't compile libICU for it"
    "you can send a PR for us!")
endif()
#===================


#===== imgui ======
file(GLOB IMGUI_SOURCE CONFIGURE_DEPENDS "${UTOPIA_LIBRARY}/imgui/*.cpp")
add_library(imgui STATIC ${IMGUI_SOURCE})
unset(IMGUI_SOURCE)

target_include_directories(imgui PUBLIC "${UTOPIA_LIBRARY}/imgui")
target_include_directories(imgui PUBLIC "${UTOPIA_LIBRARY}/imgui/backends")

# backend
if(U_ANDROID_MODE)
    target_sources(imgui PUBLIC "${UTOPIA_LIBRARY}/imgui/backends/imgui_impl_android.cpp")
elseif(U_UNDER_WINDOWS)
    target_sources(imgui PUBLIC "${UTOPIA_LIBRARY}/imgui/backends/imgui_impl_win32.cpp")
elseif(U_UNDER_LINUX)
    target_sources(imgui PUBLIC "${UTOPIA_LIBRARY}/imgui/backends/imgui_impl_vulkan.cpp")
elseif(U_UNDER_APPLE)
    target_sources(imgui PUBLIC "${UTOPIA_LIBRARY}/imgui/backends/imgui_impl_vulkan.cpp")
else()
    message(FATAL_ERROR "unknown imgui backends for your platform!" "send a PR for us!")
endif()
#==================


#===== harfbuzz =====
set(U_HARFBUZZ_INSTALL_DIR "${UTOPIA_SYSROOT}/harfbuzz")

add_library(harfbuzz INTERFACE)
target_include_directories(harfbuzz INTERFACE "${U_HARFBUZZ_INSTALL_DIR}/include")
target_link_libraries(harfbuzz INTERFACE "${U_HARFBUZZ_INSTALL_DIR}/lib/libharfbuzz.a")
target_link_libraries(harfbuzz INTERFACE "${U_HARFBUZZ_INSTALL_DIR}/lib/libharfbuzz-subset.a")
#====================


#===== imgui ======
enable_testing()
add_subdirectory("${UTOPIA_LIBRARY}/doctest" EXCLUDE_FROM_ALL)
#=================
