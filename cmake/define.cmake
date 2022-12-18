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

# 设置版本号
set(U_PROJECT_VERSION_MAJOR "0")
set(U_PROJECT_VERSION_MINOR "1")
set(U_PROJECT_VERSION_PATCH "0")

# =================================
# 设置构建标识符
# 这个代码应该只在CI/CD环境下设置
# 默认设置为0（即release构建）
option(U_OPT_BUILD_NUMBER "set the build number for CI/CD" 0)

if(U_OPT_BUILD_NUMBER)
    set(U_PROJECT_VERSION_BUILD "${U_OPT_BUILD_NUMBER}")
    message(STATUS "build number has set(hint CI/CD):${U_OPT_BUILD_NUMBER}")
else()
    set(U_PROJECT_VERSION_BUILD "0")
    message(STATUS "build number not set(hint release-ready). default 0")
endif()

set(UTOPIA_VERSION "${U_PROJECT_VERSION_MAJOR}.${U_PROJECT_VERSION_MINOR}.${U_PROJECT_VERSION_PATCH}.${U_PROJECT_VERSION_BUILD}")

file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" "${UTOPIA_VERSION}")

message(STATUS "write version information to version.txt")
# =================================
message(STATUS "current utopia version:${UTOPIA_VERSION}")

# others
set(UTOPIA_CONFIG   "utopia-config")
set(UTOPIA_CORE     "utopia-core")
set(UTOPIA_CLIENT   "utopia-client")
set(UTOPIA_SERVER   "utopia-server")

set(UTOPIA_CORE_TEST "${UTOPIA_CORE}-test")

set(UTOPIA_LIBRARY "${CMAKE_SOURCE_DIR}/library")
set(UTOPIA_ASSERT "${CMAKE_SOURCE_DIR}/assert")

# about runtime files
set(UTOPIA_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/release")
set(UTOPIA_BUILD_TREE_INSTALL_PATH "${CMAKE_CURRENT_BINARY_DIR}/development-release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${UTOPIA_BUILD_TREE_INSTALL_PATH})

set(UTOPIA_ASSERT_DIR_NAME "assert")
