#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#==============================================
macro(u_include_at_root INCLUDE_FILE_ARG)
    get_filename_component(INCLUDE_FILE ${INCLUDE_FILE_ARG} ABSOLUTE)

    message(STATUS "include cmake:${INCLUDE_FILE}")
    
    list(APPEND U_TOTAL_CMAKE_FILES ${INCLUDE_FILE})

    include(${INCLUDE_FILE})

    unset(${INCLUDE_FILE})
    unset(${INCLUDE_FILE_ARG})
endmacro(u_include_at_root)

macro(u_include INCLUDE_FILE_ARG)
    u_include_at_root(${INCLUDE_FILE_ARG})
    set(U_TOTAL_CMAKE_FILES ${U_TOTAL_CMAKE_FILES} PARENT_SCOPE)
endmacro(u_include)

#==============================================

macro(u_add_subdirectory_at_root SUBDIR_ARG)
    get_filename_component(SUBDIR ${SUBDIR_ARG} ABSOLUTE)

    message(STATUS "include subdirectory:${SUBDIR}/CMakeLists.txt")

    list(APPEND U_TOTAL_CMAKE_FILES "${SUBDIR}/CMakeLists.txt")

    add_subdirectory(${SUBDIR})

    unset(${SUBDIR_ARG})
    unset(${SUBDIR})
endmacro(u_add_subdirectory_at_root)

macro(u_add_subdirectory SUBDIR_ARG)
    u_add_subdirectory_at_root(${SUBDIR_ARG})
    set(U_TOTAL_CMAKE_FILES ${U_TOTAL_CMAKE_FILES} PARENT_SCOPE)
endmacro()

#==============================================
# add some files, you may want to edit whem in IDE
set(U_TOTAL_CMAKE_FILES "${CMAKE_CURRENT_LIST_FILE}")

list(APPEND U_TOTAL_CMAKE_FILES "${CMAKE_SOURCE_DIR}/.clang-tidy")
list(APPEND U_TOTAL_CMAKE_FILES "${CMAKE_SOURCE_DIR}/.clang-format")
list(APPEND U_TOTAL_CMAKE_FILES "${CMAKE_SOURCE_DIR}/.editorconfig")
list(APPEND U_TOTAL_CMAKE_FILES "${CMAKE_SOURCE_DIR}/.gitignore")
list(APPEND U_TOTAL_CMAKE_FILES "${CMAKE_SOURCE_DIR}/.gitattributes")
