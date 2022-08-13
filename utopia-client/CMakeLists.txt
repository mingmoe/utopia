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

add_executable(${UTOPIA_CLIENT} 
    "src/main.cpp"
)

target_include_directories(${UTOPIA_CLIENT} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${UTOPIA_CLIENT} PUBLIC "${UTOPIA_CORE}")
target_link_libraries(${UTOPIA_CLIENT} PUBLIC zlib)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC libpng)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC SDL2::SDL2-static)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC freetype)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC Ogg::ogg)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC zstd::libzstd_static)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC flac)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC libjpeg-turbo::jpeg-static)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC libjpeg-turbo::turbojpeg-static)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC Vorbis::vorbis)
target_link_libraries(${UTOPIA_CLIENT} PUBLIC OpenAL)
