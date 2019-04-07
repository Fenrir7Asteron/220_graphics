cmake_minimum_required(VERSION 3.13)
project(genetic_algorithm)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        ../ascii/Ascii.cpp
        GeneticAlgorithm.cpp
        Population.cpp Population.h)



target_include_directories(${PROJECT_NAME} PUBLIC ../ascii)
