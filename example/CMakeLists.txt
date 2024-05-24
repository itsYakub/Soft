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

project(
    demo 
    DESCRIPTION "Demonstration of Soft Graphics Library"
    LANGUAGES C
)


project(demo)

add_executable(${PROJECT_NAME} demo.c)
target_link_libraries(${PROJECT_NAME} PUBLIC soft)

if (LINUX)

    target_link_options(
        ${PROJECT_NAME} PRIVATE 
        
        -static-libgcc 
        -static-libstdc++
    )

    set_target_properties(demo PROPERTIES SUFFIX ".out")

endif()

if (WIN32)

    target_compile_options(
        ${PROJECT_NAME} PRIVATE 
        
        -mwindows
    )

    target_link_options(
        ${PROJECT_NAME} PRIVATE 
        
        -static
    )

    set_target_properties(demo PROPERTIES SUFFIX ".exe")

endif()

if (APPLE)

    target_link_libraries(
        ${PROJECT_NAME} 
        
        "-framework IOKit"
    )

    target_link_libraries(
        ${PROJECT_NAME} 
        
        "-framework Cocoa"
    )

    set_target_properties(demo PROPERTIES SUFFIX ".app")

endif()