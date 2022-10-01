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
#
# 环境检查
# 
# 标准编译套件:
# msvc >= 19.30.0
# gcc >= 11.0.0
# clang >= 12.0.0
#
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# 同时会根据设置设置以下变量
# 编译器: U_USE_GCC U_USE_CLANG U_USE_MSVC
# 操作系统: U_UNDER_WINDOWS U_UNDER_LINUX U_UNDER_APPLE
# 编译模式: U_DEBUG_MODE U_RELEASE_MODE
# 同时设置一个特殊的变量供ANDROID使用: U_ANDROID_MODE
# 同时设置以下变量:
# U_PUBLIC_OS: Windows|Linux|Apple
# U_PUBLIC_COMPILER: MSVC|GCC|CLANG
# U_COMPILE_MODE: Debug|Release
# U_SYSROOT_NAME: 用于标识需要使用的sysroot
# UTOPIA_SYSROOT: 第三方库使用的sysroot

# 检查编译器
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(U__TEMPL_CXX_V "12.0.0")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "${U__TEMPL_CXX_V}")
		message(WARNING "gcc version may be too old"
                "official-build use gcc ${U__TEMPL_CXX_V} or higher")
	endif()

    set(U_USE_GCC "True")
    set(U_PUBLIC_COMPILER "GCC")
    message(STATUS "compile by gcc ${CMAKE_CXX_COMPILER_VERSION}")
    unset(U__TEMPL_CXX_V)


elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(U__TEMPL_CXX_V "14.0.0")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "${U__TEMPL_CXX_V}")
		message(WARNING "clang version may be too old"
            "official-build use clang ${U__TEMPL_CXX_V} or higher")
	endif()

    set(U_USE_CLANG "True")
    set(U_PUBLIC_COMPILER "CLANG")
    message(STATUS "compile by clang ${CMAKE_CXX_COMPILER_VERSION}")
    unset(U__TEMPL_CXX_V)


elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(U__TEMPL_CXX_V "19.30.0")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "${U__TEMPL_CXX_V}")
		message(WARNING "msvc version may be too old"
        "official-build use msvc ${U__TEMPL_CXX_V} or higher")
	endif()
    
    set(U_USE_MSVC "True")
    set(U_PUBLIC_COMPILER "MSVC")
    message(STATUS "compile by msvc ${CMAKE_CXX_COMPILER_VERSION}")
    unset(U__TEMPL_CXX_V)


else()
	message(
		FATAL_ERROR 
		"no supported c++ compiler:${CMAKE_CXX_COMPILER_ID}"
	)
endif()


# 检查编译模式
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(U_DEBUG_MODE "True")

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(U_RELEASE_MODE "True")

else()
    message(FATAL_ERROR "unknown build type:${CMAKE_BUILD_TYPE} only support Debug or Release")
endif()


set(U_COMPILE_MODE "${CMAKE_BUILD_TYPE}")
message(STATUS "build type:${CMAKE_BUILD_TYPE}")


# 检查操作系统
if(UNIX AND NOT APPLE)
    message(STATUS "compile under linux")
    set(U_PUBLIC_OS "Linux")
    set(U_UNDER_LINUX "True")

elseif(WIN32)
    message(STATUS "compile under windows")
    set(U_PUBLIC_OS "Windows")
    set(U_UNDER_WINDOWS "True")

else()
    message(STATUS "compile under apple")
    set(U_PUBLIC_OS "Apple")
    set(U_UNDER_APPLE "True")
endif()

# 检查安卓设置
if(ANDROID)
    message(STATUS "compile for android")
    set(U_ANDROID_MODE "True")
endif()

#=========================================
# 设置sysroot_name
if(U_ANDROID_MODE)
    set(U_SYSROOT_NAME "android-arm64")
else()
    if(U_UNDER_WINDOWS)
        set(U_SYSROOT_NAME "windows-x64")
    elseif(U_UNDER_LINUX)
        set(U_SYSROOT_NAME "linux-x64")
    elseif(U_UNDER_APPLE)
        set(U_SYSROOT_NAME "macos-x64")
    else()
        message(FATAL_ERROR "unknown os for sysroot")
    endif()
endif()

if(U_DEBUG_MODE)
    set(U_SYSROOT_NAME "${U_SYSROOT_NAME}-Debug")
elseif(U_RELEASE_MODE)
    set(U_SYSROOT_NAME "${U_SYSROOT_NAME}-Release")
else()
    message(FATAL_ERROR "unknown build type for sysroot")
endif()

# 设置sysroot
if(NOT DEFINED UTOPIA_SYSROOT)
    set(UTOPIA_SYSROOT "${UTOPIA_LIBRARY}/install-tree/${U_SYSROOT_NAME}")
    message(STATUS "using ${UTOPIA_SYSROOT} as SYSROOT")
endif()

# 设置系统find package的路径
FILE(GLOB children_root LIST_DIRECTORIES True  "${UTOPIA_SYSROOT}/*")

set(CMAKE_PREFIX_PATH "${UTOPIA_SYSROOT}")

foreach(X IN LISTS children_root)
    if(IS_DIRECTORY ${X})
        list(APPEND CMAKE_PREFIX_PATH ${X})
    endif()
endforeach()

message(STATUS "library roots: ${CMAKE_PREFIX_PATH}")


# 禁用一些我们不想要的搜索方式
set(NO_SYSTEM_ENVIRONMENT_PATH       ON)
set(NO_CMAKE_PACKAGE_REGISTRY        ON)
set(NO_CMAKE_SYSTEM_PATH             ON)
set(NO_CMAKE_SYSTEM_PACKAGE_REGISTRY ON)

