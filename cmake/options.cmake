#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

# clang tidy设置
option(U_OPT_USE_CLANG_TIDY "using clang-tidy?" OFF)
option(U_OPT_CLANG_TIDY_PATH "clang-tidy path?" "clang-tidy")

if(U_OPT_USE_CLANG_TIDY)
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
    "/DWIN32 /D_WINDOWS /utf-8 /Zi /W4 /permissive- /EHsc /GR /Zc:__cplusplus /D_CRT_SECURE_NO_WARNINGS /external:anglebrackets")
    set(U__TEMP__FLAGS_DEBUG "/Od /MDd")
    set(U__TEMP__FLAGS_RELEASE "/O2 /MD /DNDEBUG")

    set(CMAKE_C_FLAGS "${U__TEMP__FLAGS_} /std:c11")
    set(CMAKE_C_FLAGS_DEBUG "${U__TEMP__FLAGS_DEBUG}")
    set(CMAKE_C_FLAGS_RELEASE "${U__TEMP__FLAGS_RELEASE}")

    set(CMAKE_CXX_FLAGS "${U__TEMP__FLAGS_} /std:c++20")
    set(CMAKE_CXX_FLAGS_DEBUG "${U__TEMP__FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${U__TEMP__FLAGS_RELEASE}")

    unset(U__TEMP__FLAGS_)
    unset(U__TEMP__FLAGS_DEBUG)
    unset(U__TEMP__FLAGS_RELEASE)

else() # for gcc\clang

    set(U__TEMP__FLAGS_ "-Wall -Wextra -g3")

    set(U__TEMP__FLAGS_DEBUG "-O0 -ggdb")
    set(U__TEMP__FLAGS_RELEASE "-O3 -DNDEBUG")

    set(CMAKE_C_FLAGS "${U__TEMP__FLAGS_} -std=c11")
    set(CMAKE_C_FLAGS_DEBUG "${U__TEMP__FLAGS_DEBUG}")
    set(CMAKE_C_FLAGS_RELEASE "${U__TEMP__FLAGS_RELEASE}")

    set(CMAKE_CXX_FLAGS "${U__TEMP__FLAGS_} -std=c++20 -frtti")
    set(CMAKE_CXX_FLAGS_DEBUG "${U__TEMP__FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${U__TEMP__FLAGS_RELEASE}")


    unset(U__TEMP__FLAGS_)
    unset(U__TEMP__FLAGS_DEBUG)
    unset(U__TEMP__FLAGS_RELEASE)
endif()

function(u_add_define)
    foreach(arg IN LISTS ARGN)
        if(MSVC)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D${arg}")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D${arg}")
        else()
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D${arg}")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${arg}")
        endif()
    endforeach()
endfunction()

# 地址消毒
if(U_DEBUG_MODE)
    option(U_OPT_ASAN "enable Address Sanitize? default enable in debug" ON)
elseif(U_RELEASE_MODE)
    option(U_OPT_ASAN "enable Address Sanitize? default enable in debug" OFF)
endif()

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
