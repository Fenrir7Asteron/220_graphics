cmake_minimum_required(VERSION 3.13)
project(220_graphics)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        genetic_algorithm/GeneticAlgorithm.cpp
        ascii/Ascii.cpp
        main.cpp genetic_algorithm/Population.cpp genetic_algorithm/Population.h)
add_subdirectory(ascii)
add_subdirectory(genetic_algorithm)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

target_include_directories(${PROJECT_NAME} PUBLIC ascii)
target_include_directories(${PROJECT_NAME} PUBLIC genetic_algorithm)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
target_link_libraries(220_graphics sfml-graphics)

find_package(ImageMagick)
find_package(ImageMagick COMPONENTS Magick++)

include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(220_graphics ${ImageMagick_LIBRARIES})