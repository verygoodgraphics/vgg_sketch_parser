cmake_minimum_required(VERSION 3.0.0)
project(vgg_sketch_parser VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE all_h ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE all_hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE all_cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${all_h} ${all_hpp} ${all_cpp})

# vgg_sketch_parser relay on vgg_contrib zip boost json
add_library(vgg_sketch_parser ${all_h} ${all_hpp} ${all_cpp})

get_filename_component(tmp_path ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(vgg_sketch_parser PUBLIC 
    ${VGG_CONTRIB_JSON_INCLUDE} 
    ${VGG_CONTRIB_BOOST_INCLUDE}
    ${VGG_CONTRIB_ZIP_INCLUDE}
    ${tmp_path})

target_link_libraries(vgg_sketch_parser PRIVATE zip)

if (MSVC)
    #target_compile_definitions(vgg_sketch_parser PRIVATE _WIN32_WINNT=0x0601)
    target_compile_options(vgg_sketch_parser PRIVATE /W3 /WX /wd"4996")
else()
    # target_compile_options(vgg_sketch_parser PRIVATE -Wall -Wno-nonnull 
    #     -Wno-unused-variable -Wno-parentheses -Wno-sign-compare 
    #     -Wno-maybe-uninitialized)
endif()