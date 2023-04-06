cmake_minimum_required(VERSION 3.0.0)
project(vgg_sketch_parser VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE all_h ${CMAKE_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE all_hpp ${CMAKE_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE all_cpp ${CMAKE_SOURCE_DIR}/src/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${all_h} ${all_hpp} ${all_cpp})

#add_subdirectory(vgg_contrib)
add_library(vgg_sketch_parser ${all_h} ${all_hpp} ${all_cpp})
#add_executable(vgg_sketch_parser ${all_h} ${all_hpp} ${all_cpp})

target_include_directories(vgg_sketch_parser PUBLIC ${VGG_CONTRIB_INCLUDE} ${CMAKE_SOURCE_DIR})
target_link_libraries(vgg_sketch_parser PRIVATE zip)

if (MSVC)
    #target_compile_definitions(vgg_sketch_parser PRIVATE _WIN32_WINNT=0x0601)
    target_compile_options(vgg_sketch_parser PRIVATE /W3 /WX /wd"4996")
else()
    # target_compile_options(vgg_sketch_parser PRIVATE -Wall -Wno-nonnull 
    #     -Wno-unused-variable -Wno-parentheses -Wno-sign-compare 
    #     -Wno-maybe-uninitialized)
endif()