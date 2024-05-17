cmake_minimum_required(VERSION 3.14)

project(soft VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.3 
)

FetchContent_MakeAvailable(SDL2)

file(
    GLOB SOURCES 

    ${CMAKE_SOURCE_DIR}/src/soft.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2)

add_executable(demo src/main.c)
target_link_libraries(demo PUBLIC ${PROJECT_NAME})

if (LINUX)

    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
    target_link_options(demo PRIVATE -static-libgcc -static-libstdc++)

    set_target_properties(demo PROPERTIES SUFFIX ".out")

endif()

if (WIN32)

    target_compile_options(${PROJECT_NAME} PRIVATE -mwindows)
    target_compile_options(${PROJECT_NAME} PRIVATE -mwindows)

    target_link_options(${PROJECT_NAME} PRIVATE -static)
    target_link_options(demo PRIVATE -static)

    set_target_properties(demo PROPERTIES SUFFIX ".exe")

endif()

if (APPLE)

    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")

    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(demo "-framework Cocoa")

    set_target_properties(demo PROPERTIES SUFFIX ".app")

endif()