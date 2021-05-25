cmake_minimum_required(VERSION 3.20)

project(Chip8 VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CHIP8_CXX_COMPILE_FLAGS "-g -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CHIP8_CXX_COMPILE_FLAGS}")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(Chip8 src/main.cpp)
add_library(chip8Base src/chip8.cpp)

target_include_directories(chip8Base PUBLIC "./include")
target_link_libraries(Chip8 ${SDL2_LIBRARIES} chip8Base)

target_include_directories(Chip8 PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(Chip8 PUBLIC "./include")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.10.0.zip
  )
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  chip8_test
  test/chip8_test.cc
  )
target_link_libraries(
  chip8_test
  gtest_main
  chip8Base
  )

include(GoogleTest)

gtest_discover_tests(chip8_test)
