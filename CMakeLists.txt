# ---------------------------------------------------------------------------------
# Soft - Real-Time CPU Renderer
# ---------------------------------------------------------------------------------
# Author: https://github.com/itsYakub
# ---------------------------------------------------------------------------------
# Version history:
# - Version 1.0 (Current):
#      > Release date: 
# ---------------------------------------------------------------------------------
# External Dependencies:
# - SDL2: https://github.com/libsdl-org/SDL.git
# ---------------------------------------------------------------------------------
# LICENCE:
# Copyright (c) 2024 Jakub Oleksiak <yakubofficialmail@gmail.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
# OR OTHER DEALINGS IN THE SOFTWARE.
# ---------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    soft 
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/itsYakub/Soft"
    DESCRIPTION "Simple 2D Real-Time Graphics Library"
    LANGUAGES C
)

include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.3 
)

FetchContent_MakeAvailable(SDL2)
target_compile_definitions(SDL2 PUBLIC SDL2)

add_subdirectory(src soft)
add_subdirectory(example)
