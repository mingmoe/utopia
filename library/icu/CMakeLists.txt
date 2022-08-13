#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#   Copyright (C) 2021-2022 mingmoe(me@kawayi.moe)(https://blog.kawayi.moe)
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

project(ICU LANGUAGES CXX C)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/common/sources.txt" COMMON_RAW_SOURCES 
ENCODING "UTF-8")
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/i18n/sources.txt" I18N_RAW_SOURCES
ENCODING "UTF-8")
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/stubdata/sources.txt" STUBDATA_RAW_SOURCES
ENCODING "UTF-8")

foreach(S IN LISTS COMMON_RAW_SOURCES)
    list(APPEND COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/common/${S}")
endforeach()
foreach(S IN LISTS I18N_RAW_SOURCES)
    list(APPEND I18N_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/i18n/${S}")
endforeach()
foreach(S IN LISTS STUBDATA_RAW_SOURCES)
    list(APPEND STUBDATA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/stubdata/${S}")
endforeach()

add_library(icu SHARED ${COMMON_SOURCES} ${I18N_SOURCES} ${STUBDATA_SOURCES})

# compiler options depends on root
# we only set those we need
target_compile_definitions("icu" PRIVATE "U_COMBINED_IMPLEMENTATION=1")
target_compile_definitions("icu" PRIVATE "U_COMMON_IMPLEMENTATION=1")
target_compile_definitions("icu" PRIVATE "U_I18N_IMPLEMENTATION=1")
target_compile_definitions("icu" INTERFACE "U_HIDE_INTERNAL_API=1")
target_compile_definitions("icu" INTERFACE "U_HIDE_DEPRECATED_API=1")
target_include_directories("icu" SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/common/")
target_include_directories("icu" SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/i18n/")
target_include_directories("icu" SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/stubdata/")

