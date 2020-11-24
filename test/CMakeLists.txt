#================================================================================
# Automaton Tests CMake file
#================================================================================

cmake_minimum_required(VERSION 2.8)

#--------------------------------------------------------------------------------
# This is the list of source files that need to be compiled
#--------------------------------------------------------------------------------
# if("${CMAKE_GENERATOR}" STREQUAL "Xcode")
# set( automaton_HDRS 
#   ${PROJECT_SOURCE_DIR}/include/core/common.h
# )
# endif("${CMAKE_GENERATOR}" STREQUAL "Xcode")

set( tests_SRCS 
  sample2_unittest.cpp
  sample2.cpp
)

# adding GoogleTest static library (x86_64 only)
set( gtest_LIBRARIES 
  ${PROJECT_SOURCE_DIR}/lib/gtest/libgtest_main.a
  ${PROJECT_SOURCE_DIR}/lib/gtest/libgtest.a
)

#--------------------------------------------------------------------------------
# We need this to be able to include headers produced by uic in our code
# CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES()
#  works just like INCLUDEPATH from qmake
#--------------------------------------------------------------------------------
include_directories (
  ${CMAKE_CURRENT_BINARY_DIR} 
  ${PROJECT_SOURCE_DIR}/include
)

#--------------------------------------------------------------------------------
# For Apple set the icns file containing icons:
# ...how it shows up in the Info.plist file
# ...where in the bundle to put the icns file
# ...include the icns file in the target
#--------------------------------------------------------------------------------
# if(APPLE)
  # set(MACOSX_BUNDLE_ICON_FILE fdl.icns)
  # set_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/fdl.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  # set(automaton_SRCS ${automaton_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/fdl.icns)
# endif(APPLE)

#--------------------------------------------------------------------------------
# TARGET_VERSION macros are set in the top level CMakeLists.txt file.
#--------------------------------------------------------------------------------
add_definitions( 
  -DCMAKE_TARGET_VERSION=1 
  -DTARGET_VERSION_MAJOR=${TARGET_VERSION_MAJOR} 
  -DTARGET_VERSION_MINOR=${TARGET_VERSION_MINOR}
)

#--------------------------------------------------------------------------------
# Associate source and header dependencies to target(s).
#--------------------------------------------------------------------------------
add_executable( sample2_test
  ${tests_SRCS}
)

#--------------------------------------------------------------------------------
# Associate linker dependencies to target(s).
#--------------------------------------------------------------------------------
target_link_libraries ( sample2_test
  ${gtest_LIBRARIES}
)