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
    soft 
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/itsYakub/Soft"
    DESCRIPTION "Simple 2D Real-Time Graphics Library"
    LANGUAGES C
)

set(
    SOURCES 
    
    ${CMAKE_CURRENT_SOURCE_DIR}/soft.c
)

add_library(
    ${PROJECT_NAME} STATIC 
    
    ${SOURCES}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (LINUX)

    target_link_options(
        ${PROJECT_NAME} PRIVATE 
        
        -static-libgcc 
        -static-libstdc++
    )

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

endif()