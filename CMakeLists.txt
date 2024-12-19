cmake_minimum_required(VERSION 3.10)
project(game)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
add_compile_options(-Wall -Wextra -Wfloat-conversion)

# Include directories
include_directories(include)

# Gather source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add the main executable
add_executable(game main.cpp ${SOURCES})

# Link SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(game PUBLIC SDL2::SDL2 SDL2::SDL2main)
