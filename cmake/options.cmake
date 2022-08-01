#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

# clang tidy设置
option(U_OPT_USE_CLANG_TIDY "using clang-tidy?")
option(U_OPT_CLANG_TIDY_PATH "clang-tidy path?" OFF)

if(NOT U_OPT_CLANG_TIDY_PATH)
    set(U_OPT_CLANG_TIDY_PATH "clang-tidy")
endif()

if(U_OPT_USE_CLANG_TIDY)
    message(STATIS "use clang-tidy:${U_OPT_CLANG_TIDY_PATH}")
    function(u_open_clang_tidy)
        set(CMAKE_CXX_CLANG_TIDY 
            ${U_OPT_CLANG_TIDY_PATH}
            --use-color
            PARENT_SCOPE)
    endfunction(u_open_clang_tidy)
else()
    function(u_open_clang_tidy)
    endfunction(u_open_clang_tidy)
endif()

function(u_close_clang_tidy)
    unset(CMAKE_CXX_CLANG_TIDY)
    unset(CMAKE_CXX_CLANG_TIDY PARENT_SCOPE)
endfunction(u_close_clang_tidy)

u_close_clang_tidy()

# 设置编译选项
if(MSVC) # for msvc
    set(U__TEMP__FLAGS_ 
    "/DWIN32 /D_WINDOWS /utf-8 /Zi /W1 /permissive- /EHsc /GR /Zc:__cplusplus /D_CRT_SECURE_NO_WARNINGS /external:anglebrackets")
    set(U__TEMP__FLAGS_DEBUG "/Od /MDd")
    set(U__TEMP__FLAGS_RELEASE "/O2 /MD /DNDEBUG")

    set(CMAKE_C_FLAGS "${U__TEMP__FLAGS_}")
    set(CMAKE_C_FLAGS_DEBUG "${U__TEMP__FLAGS_DEBUG}")
    set(CMAKE_C_FLAGS_RELEASE "${U__TEMP__FLAGS_RELEASE}")

    set(CMAKE_CXX_FLAGS "${U__TEMP__FLAGS_} /std:c++20")
    set(CMAKE_CXX_FLAGS_DEBUG "${U__TEMP__FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${U__TEMP__FLAGS_RELEASE}")

    unset(U__TEMP__FLAGS_)
    unset(U__TEMP__FLAGS_DEBUG)
    unset(U__TEMP__FLAGS_RELEASE)

else() # for gcc\clang

    set(U__TEMP__FLAGS_ "-W -g3 -fno-strict-aliasing")

    set(U__TEMP__FLAGS_DEBUG "-O0 -ggdb")
    set(U__TEMP__FLAGS_RELEASE "-O3 -DNDEBUG")

    set(CMAKE_C_FLAGS "${U__TEMP__FLAGS_} -std=c99")
    set(CMAKE_C_FLAGS_DEBUG "${U__TEMP__FLAGS_DEBUG}")
    set(CMAKE_C_FLAGS_RELEASE "${U__TEMP__FLAGS_RELEASE}")

    set(CMAKE_CXX_FLAGS "${U__TEMP__FLAGS_} -std=c++20 -frtti -fexception")
    set(CMAKE_CXX_FLAGS_DEBUG "${U__TEMP__FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${U__TEMP__FLAGS_RELEASE}")


    unset(U__TEMP__FLAGS_)
    unset(U__TEMP__FLAGS_DEBUG)
    unset(U__TEMP__FLAGS_RELEASE)
endif()

# helper function
function(u_add_define)
    foreach(arg IN LISTS ARGN)
        if(MSVC)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D${arg}" PARENT_SCOPE)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D${arg}" PARENT_SCOPE)
        else()
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D${arg}" PARENT_SCOPE)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${arg}" PARENT_SCOPE)
        endif()
    endforeach()
endfunction()

# 地址消毒
option(U_OPT_ASAN "enable Address Sanitize? default enable in debug" OFF)

if(U_OPT_ASAN)
    if(MSVC)
        set(U_TEMP__OPT "/fsanitize=address")
    else()
        set(U_TEMP__OPT "-fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope")
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${U_TEMP__OPT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${U_TEMP__OPT}")
    unset(U_TEMP__OPT)
endif()

# UTOPIA_NO_DEBUG_BREAK
option(U_OPT_DEBUG_BREAK "enable utopia::core::debug_break()? OFF to define UTOPIA_NO_DEBUG_BREAK macro" OFF)

if(NOT U_OPT_DEBUG_BREAK)
    if(MSVC)
        set(U_TEMP__OPT "/DUTOPIA_NO_DEBUG_BREAK")
    else()
        set(U_TEMP__OPT "-DUTOPIA_NO_DEBUG_BREAK")
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${U_TEMP__OPT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${U_TEMP__OPT}")
    unset(U_TEMP__OPT)
endif()
