cmake_minimum_required(VERSION 3.16)

project(GA_test)

find_package(GTest REQUIRED)
include_directories(${GA_INCLUDE_DIR})

add_executable(GA_test_run test.cpp)

target_link_libraries(GA_test_run genetic gtest gmock -pthread)
