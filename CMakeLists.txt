#================================================================================
# Automaton CMake file
#================================================================================

cmake_minimum_required(VERSION 3.11)

#--------------------------------------------------------------------------------
# basic definitions
#--------------------------------------------------------------------------------

project( automaton )

set( TARGET_VERSION_MAJOR 0 )
set( TARGET_VERSION_MINOR 1 )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW) 
endif()

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

set( AUTOMATON_BUILD_TESTS ON CACHE BOOL "Build tests")
set( AUTOMATON_BUILD_DOCS OFF CACHE BOOL "Build documentation")

#--------------------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------------------

add_subdirectory( src )
# add_subdirectory( docs )

if(AUTOMATON_BUILD_TESTS)
  add_subdirectory( test )
endif(AUTOMATON_BUILD_TESTS)
