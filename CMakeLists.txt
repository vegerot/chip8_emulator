cmake_minimum_required(VERSION 3.20)

project(Chip8 VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CHIP8_CXX_COMPILE_FLAGS "-g -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CHIP8_CXX_COMPILE_FLAGS}")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(Chip8 src/main.cpp)

target_link_libraries(Chip8 ${SDL2_LIBRARIES} )

target_include_directories(Chip8 PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(Chip8 PUBLIC "./include")
