# Template Project
#
# Copyright (C) 2020 Malte Kießling
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or any later version.

# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.

# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# functions stolen from mkalte666/cpputils
include(cmake/funcEnableStrictOptions.cmake)
include(cmake/funcEnableClangTidy.cmake)

# disable in-source build. found at
# https://stackoverflow.com/questions/1208681/with-cmake-how-would-you-disable-
# in-source-builds
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# c++ standard. starting at 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(imguiIncludeDir
    ${CMAKE_SOURCE_DIR}/3rdparty/imgui-cmake-blob/imgui/
    CACHE PATH "Path to imgui headers")
add_subdirectory(3rdparty/imgui-cmake-blob)
add_subdirectory(3rdparty/imguiplot)
add_subdirectory(3rdparty/fmt)
find_package(SDL2 REQUIRED)

project(imguiplot_demo)

add_executable(imguiplot_demo include/messagebox.h include/plotdemo.h
                              include/shared.h src/main.cpp src/plotdemo.cpp)
target_include_directories(imguiplot_demo PRIVATE include/)
target_link_libraries(
    imguiplot_demo
    fmt::fmt
    imguiplot
    imgui
    gl3w
    SDL2::SDL2main
    SDL2::SDL2)
enablestrictoptions(imguiplot_demo)

install(
    TARGETS imguiplot_demo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib)

find_package(Doxygen OPTIONAL_COMPONENTS dot)
if(DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_LATEX YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc)
    doxygen_add_docs(imguiplot_demo_docs src/ include/ doc/)
endif()
