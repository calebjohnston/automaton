#================================================================================
# Automaton Tests CMake file
#================================================================================

cmake_minimum_required(VERSION 3.11)

#--------------------------------------------------------------------------------
# basic definitions
#--------------------------------------------------------------------------------

option(BUILD_GMOCK OFF)
option(INSTALL_GTEST OFF)

set( tests_SRCS
  sample2_unittest.cpp
  sample2.cpp
)

# Add GoogleTest dependency
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/googletest
                 ${PROJECT_SOURCE_DIR}/lib/googletest/build
                 EXCLUDE_FROM_ALL)

#--------------------------------------------------------------------------------
# includes
#--------------------------------------------------------------------------------
include_directories (
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}
)

#--------------------------------------------------------------------------------
# compile time flags
#--------------------------------------------------------------------------------
add_definitions(
  -DCMAKE_TARGET_VERSION=1
  -DTARGET_VERSION_MAJOR=${TARGET_VERSION_MAJOR}
  -DTARGET_VERSION_MINOR=${TARGET_VERSION_MINOR}
)

#--------------------------------------------------------------------------------
# Associate source and header dependencies to targets.
#--------------------------------------------------------------------------------
add_executable( sample2_test
  ${tests_SRCS}
)

#--------------------------------------------------------------------------------
# Associate linker dependencies to targets.
#--------------------------------------------------------------------------------
target_link_libraries ( sample2_test gtest_main )