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

# 设置安装路径
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/release")

# 安装可执行文件
install(TARGETS ${UTOPIA_CORE_TEST} DESTINATION . PERMISSIONS GROUP_EXECUTE)
install(TARGETS ${UTOPIA_SERVER} DESTINATION . PERMISSIONS GROUP_EXECUTE)
install(TARGETS ${UTOPIA_CLIENT} DESTINATION . PERMISSIONS GROUP_EXECUTE)
install(TARGETS icu DESTINATION . PERMISSIONS GROUP_EXECUTE)
install(TARGETS OpenAL DESTINATION . PERMISSIONS GROUP_EXECUTE)

# 安装assert资产
set(U__TEMP__ASSERT "assert")

install(FILES "${UTOPIA_ASSERT}/noto-cjk/Sans/OTC/NotoSansCJK-Regular.ttc" DESTINATION "${U__TEMP__ASSERT}/font")
install(FILES "${UTOPIA_ASSERT}/noto-fonts/hinted/ttf/NotoSans/NotoSans-Regular.ttf" DESTINATION "${U__TEMP__ASSERT}/font")
install(FILES "${UTOPIA_ASSERT}/noto-emoji/fonts/NotoColorEmoji.ttf" DESTINATION "${U__TEMP__ASSERT}/font")

install(DIRECTORY "${UTOPIA_ASSERT}/icu4c-data" DESTINATION "${U__TEMP__ASSERT}")

unset(U__TEMP__ASSERT)
