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
list(APPEND U_TOTAL_CMAKE_FILES "${CMAKE_SOURCE_DIR}/config.hpp")
list(APPEND U_TOTAL_CMAKE_FILES "${CMAKE_SOURCE_DIR}/LICENSE.txt")
