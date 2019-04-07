cmake_minimum_required(VERSION 3.13)
project(ascii)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        Ascii.cpp
        ../genetic_algorithm/Population.cpp ../genetic_algorithm/Population.h)