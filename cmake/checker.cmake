#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
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
# U_PUBLIC_COMPILER: Msvc|Gcc|Clang
# U_COMPILE_MODE: Debug|Release
# U_SYSROOT_NAME: 用于标识需要使用的sysroot
# UTOPIA_SYSROOT: 第三方库使用的sysroot

# 检查编译器
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "12.0.0")
		message(WARNING "gcc version may be too old")
		message(STATUS "official-build use gcc 11 or higher")
	endif()

    set(U_USE_GCC "true")
    set(U_PUBLIC_COMPILER "Gcc")
    message(STATUS "compile by gcc")


elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14.0.0")
		message(WARNING "clang version may be too old")
		message(STATUS "official-build use clang 12 or higher")
	endif()

    set(U_USE_CLANG "true")
    set(U_PUBLIC_COMPILER "Clang")
    message(STATUS "compile by clang")


elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.30.0")
		message(WARNING "msvc version may be too old")
		message(STATUS "official-build use msvc 19.30 or higher")
	endif()
    
    set(U_USE_MSVC "true")
    set(U_PUBLIC_COMPILER "Msvc")
    message(STATUS "compile by msvc")


else()
	message(
		FATAL_ERROR 
		"unknow c++ compiler:${CMAKE_CXX_COMPILER_ID}"
	)
endif()


# 检查编译模式
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(U_DEBUG_MODE "true")

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(U_RELEASE_MODE "true")

else()
    message(FATAL_ERROR "unknown build type:${CMAKE_BUILD_TYPE} only support Debug or Release")
endif()


set(U_COMPILE_MODE "${CMAKE_BUILD_TYPE}")
message(STATUS "build type:${CMAKE_BUILD_TYPE}")


# 检查操作系统
if(UNIX AND NOT APPLE)
    message(STATUS "compile under linux")
    set(U_PUBLIC_OS "Linux")
    set(U_UNDER_LINUX "true")

elseif(WIN32)
    message(STATUS "compile under windows")
    set(U_PUBLIC_OS "Windows")
    set(U_UNDER_WINDOWS "true")

else()
    message(STATUS "compile under apple")
    set(U_PUBLIC_OS "Apple")
    set(U_UNDER_APPLE "true")
endif()

# 检查安卓设置
if(ANDROID)
    message(STATUS "compile for android")
    set(U_ANDROID_MODE "true")
endif()

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

set(CMAKE_PREFIX_PATH "${UTOPIA_SYSROOT}")
