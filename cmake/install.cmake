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

# 设置安装路径
set(CMAKE_INSTALL_PREFIX "${UTOPIA_INSTALL_PATH}")


# 设置辅助函数
function(utopia_runtime_files NEED_FILES DESTINATION)
    install(FILES ${NEED_FILES} DESTINATION "${DESTINATION}")
    file(COPY ${NEED_FILES} DESTINATION "${UTOPIA_BUILD_TREE_INSTALL_PATH}/${DESTINATION}")
endfunction()

function(utopia_runtime_directory ORIGIN_DIRECTORY DESTINATION)
    install(DIRECTORY ${ORIGIN_DIRECTORY} DESTINATION "${DESTINATION}")
    file(COPY ${ORIGIN_DIRECTORY} DESTINATION "${UTOPIA_BUILD_TREE_INSTALL_PATH}/${DESTINATION}")
endfunction()

# 安装可执行文件
install(TARGETS ${UTOPIA_CORE_TEST} DESTINATION . PERMISSIONS GROUP_EXECUTE)
install(TARGETS ${UTOPIA_SERVER} DESTINATION . PERMISSIONS GROUP_EXECUTE)
install(TARGETS ${UTOPIA_CLIENT} DESTINATION . PERMISSIONS GROUP_EXECUTE)
install(TARGETS icu DESTINATION . PERMISSIONS GROUP_EXECUTE)
install(TARGETS OpenAL DESTINATION . PERMISSIONS GROUP_EXECUTE)

# search openssl
if(U_ANDROID_MODE OR U_UNDER_LINUX)
    file(GLOB U_OPENSSL_FILES
     "${UTOPIA_SYSROOT}/openssl/bin/*.so"
    )
elseif(U_UNDER_WINDOWS)
    file(GLOB U_OPENSSL_FILES
     "${UTOPIA_SYSROOT}/openssl/bin/*.dll"
     "${UTOPIA_SYSROOT}/openssl/bin/*.pdb"
    )
endif()

message(STATUS "Install Openssl: ${U_OPENSSL_FILES}")
utopia_runtime_files("${U_OPENSSL_FILES}" ".")

# 安装assert资产

utopia_runtime_directory("${UTOPIA_ASSERT}/icu4c-data" "${UTOPIA_ASSERT_DIR_NAME}")

