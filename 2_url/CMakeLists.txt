cmake_minimum_required ( VERSION 3.28 )

project ( url VERSION 1.0.0 LANGUAGES CXX )

add_subdirectory (url)

add_executable ( example example/example.cpp )
target_link_libraries ( example PRIVATE url )