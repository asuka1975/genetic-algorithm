cmake_minimum_required(VERSION 3.16)
project(genetic)

set(CMAKE_CXX_STANDARD 17)

add_library(genetic src/random_generator.cpp include/random_generator.h)
target_include_directories(genetic PUBLIC include)