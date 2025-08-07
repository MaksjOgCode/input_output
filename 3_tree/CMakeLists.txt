cmake_minimum_required ( VERSION 3.15 )

project ( tree LANGUAGES C CXX )

set ( BOOST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/external/boost/boost_1_88_0 ) # PATH TO LIBRARY
set ( CMAKE_CXX_STANDARD       20   ) 
set ( Boost_NO_SYSTEM_PATHS    ON   )
set ( Boost_USE_MULTITHREADED  ON   )
set ( Boost_USE_STATIC_LIBS    ON   )
set ( Boost_USE_STATIC_RUNTIME OFF  )
set ( BOOST_ALL_DYN_LINK       OFF  )
    
find_package ( Boost REQUIRED COMPONENTS program_options REQUIRED ) 

if ( Boost_FOUND )
    include_directories ( SYSTEM ${Boost_INCLUDE_DIR} )
    add_library (   tree_components 
                    components/options_parser/options_parser.cpp
                    components/file_system_walker/file_system_walker.cpp
                    components/tree_printer/tree_printer.cpp    )
                
    target_include_directories (    tree_components
                                    PUBLIC
                                        ${CMAKE_CURRENT_SOURCE_DIR}/components
                                        ${Boost_INCLUDE_DIRS}   )
                                    
    target_link_libraries ( tree_components PUBLIC Boost::program_options )
    add_executable ( tree components/main/main.cpp )
    target_link_libraries( tree PRIVATE tree_components )
endif ()