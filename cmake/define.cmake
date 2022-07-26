#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# this file is under MIT License.
# See https://opensource.org/licenses/MIT for license information.
# Copyright (c) 2020-2022 moe-org All rights reserved.
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

set(U_PROJECT_VERSION_MAJOR "0")
set(U_PROJECT_VERSION_MINOR "1")
set(U_PROJECT_VERSION_PATCH "0")
set(UTOPIA_VERSION 
"${U_PROJECT_VERSION_MAJOR}.${U_PROJECT_VERSION_MINOR}.${U_PROJECT_VERSION_PATCH}")

# =================================
option(U_OPT_BUILD_NUMBER "set the build number for CI/CD" 0)
option(U_OPT_GET_VERSION_EXIT "write version to version.txt then exit" 0)

if(U_OPT_BUILD_NUMBER)
    set(U_PROJECT_VERSION_BUILD "${U_OPT_BUILD_NUMBER}")
else()
    set(U_PROJECT_VERSION_BUILD "0")
endif()

file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" "${UTOPIA_VERSION}.${U_PROJECT_VERSION_BUILD}")

if(U_OPT_GET_VERSION_EXIT)
    message(FATAL_ERROR "U_OPT_GET_VERSION_EXIT is enabled")
endif()
# =================================

set(UTOPIA_CONFIG   "utopia-config")
set(UTOPIA_CORE     "utopia-core")
set(UTOPIA_CLIENT   "utopia-client")
set(UTOPIA_SERVER   "utopia-server")

set(UTOPIA_LIBRARY "${CMAKE_SOURCE_DIR}/library")
