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
  test_placeholder.cpp
  ${PROJECT_SOURCE_DIR}/src/tree.cpp
  ${PROJECT_SOURCE_DIR}/src/Events.cpp
)

set(gtest_force_shared_crt ON CACHE INTERNAL "")
enable_testing()

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
  ${PROJECT_SOURCE_DIR}/lib/eventpp/include
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
add_executable( automaton_tests ${tests_SRCS} )

# set_property(TARGET automaton_tests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
# set_property(TARGET gtest_main PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")

#--------------------------------------------------------------------------------
# Associate linker dependencies to targets.
#--------------------------------------------------------------------------------
target_link_libraries( automaton_tests gtest_main )
