#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

# clang tidy设置
if(DEFINED UTOPIA_CLANG_TIDY)
    function(u_open_clang_tidy)
        set(CMAKE_CXX_CLANG_TIDY 
            ${UTOPIA_CLANG_TIDY}
            --use-color
            PARENT_SCOPE)
    endfunction(u_open_clang_tidy)
else()
    function(u_open_clang_tidy)
    endfunction(u_open_clang_tidy)

    message(STATUS "define UTOPIA_CLANG_TIDY=/path/to/clang-tidy to enable clang-tidy")
endif()

function(u_close_clang_tidy)
    unset(CMAKE_CXX_CLANG_TIDY)
    unset(CMAKE_CXX_CLANG_TIDY PARENT_SCOPE)
endfunction(u_close_clang_tidy)

u_close_clang_tidy()

# 设置编译选项
if(MSVC) # for msvc
    set(U__TEMP__FLAGS_ 
    "/DWIN32 /D_WINDOWS /utf-8 /Zi /W4 /permissive- /EHsc /GR /Zc:__cplusplus /D_CRT_SECURE_NO_WARNINGS /external:anglebrackets /external:W3")
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

    set(U__TEMP__FLAGS_ "-Wall -Wextra -Werror -g3")

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
