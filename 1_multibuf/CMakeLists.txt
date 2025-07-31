cmake_minimum_required ( VERSION 3.28 )

project (
    multibuf
    VERSION 1.0.0
    LANGUAGES CXX
)

add_subdirectory ( multibuf )

add_executable ( example example/main.cpp )
target_link_libraries ( example PRIVATE MemoryPool )