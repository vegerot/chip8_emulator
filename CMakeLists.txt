cmake_minimum_required(VERSION 3.20)

project(Chip8 VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CHIP8_CXX_COMPILE_FLAGS "-g -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CHIP8_CXX_COMPILE_FLAGS}")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(Chip8 src/main.cpp)
add_library(chip8stack src/chip8stack.cpp)
add_library(chip8memory src/chip8memory.cpp)
add_library(chip8keyboard src/chip8keyboard.cpp)
add_library(chip8Base src/chip8.cpp)
add_library(chip8screen src/chip8screen.cpp)

list(APPEND CHIP8_LIBRARIES chip8stack chip8memory chip8keyboard chip8Base chip8screen)

foreach(library IN ITEMS ${CHIP8_LIBRARIES})
  target_include_directories(${library} PUBLIC "./include")
endforeach()

target_link_libraries(Chip8 ${SDL2_LIBRARIES} chip8stack chip8memory chip8keyboard chip8Base chip8screen)

target_include_directories(Chip8 PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(Chip8 PUBLIC "./include")
