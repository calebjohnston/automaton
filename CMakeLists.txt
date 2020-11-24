#================================================================================
# Automaton CMake file
#================================================================================

cmake_minimum_required( VERSION 2.8 )

project( Automaton )

set( TARGET_VERSION_MAJOR 0 )
set( TARGET_VERSION_MINOR 1 )

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Set the build configuration")

# if("${CMAKE_GENERATOR}" STREQUAL "Xcode")
#   set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/)
#   set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/)
# else("${CMAKE_GENERATOR}" STREQUAL "XCode")
#   set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
#   set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
# endif("${CMAKE_GENERATOR}" STREQUAL "Xcode")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/ )
set( AUTOMATON_BUILD_TESTS ON CACHE BOOL "Build docs")
set( AUTOMATON_BUILD_DOCS OF CACHE BOOL "Build documentation")
# set( USE_OLD_SOLVER OFF CACHE BOOL "Add -DUSE_OLD_SOLVER compiler flag" )

add_subdirectory( src )
# add_subdirectory( docs )
# add_subdirectory( src/tinyxml )

if(AUTOMATON_BUILD_TESTS)
  add_subdirectory( test )
endif(AUTOMATON_BUILD_TESTS)

# Add the Doxyfile.in and UseDoxygen.cmake files to the projects source directory.
# if(AUTOMATON_BUILD_DOCS)
#   set( "DOXYFILE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src "
#        "DOXYFILE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/include "
#        "DOXYFILE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/docs/pages " )
#   set( DOXYFILE_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/docs )
#   set( DOXYFILE_LATEX "NO" )
#   include( UseDoxygen )
# endif(AUTOMATON_BUILD_DOCS)
