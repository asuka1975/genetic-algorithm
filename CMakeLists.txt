cmake_minimum_required(VERSION 3.16)
project(genetic)

set(CMAKE_CXX_STANDARD 17)
set(GA_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(GA_LIBRARY_DIR "${PROJECT_BINARY_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

enable_testing()
add_library(genetic src/random_generator.cpp include/random_generator.h include/selector.h include/genetic.h src/selector.cpp)
target_include_directories(genetic PUBLIC include)

if(NOT TARGET GA_test)
    add_subdirectory(GA_test)
endif()