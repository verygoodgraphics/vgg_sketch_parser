cmake_minimum_required(VERSION 3.0.0)
project(sketch_parser_test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(${CMAKE_SOURCE_DIR}/vgg_contrib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

add_executable(test ${CMAKE_SOURCE_DIR}/test/main.cpp)
target_link_libraries(test PRIVATE vgg_sketch_parser)
