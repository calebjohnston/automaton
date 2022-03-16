#================================================================================
# Automaton CMake file
#================================================================================

cmake_minimum_required(VERSION 3.11)

#--------------------------------------------------------------------------------
# This is the list of source files that need to be compiled
#--------------------------------------------------------------------------------

file(GLOB SOURCES "*.cpp")
set(automaton_SRCS ${SOURCES})

# remove CLI from library
list(FILTER SOURCES EXCLUDE REGEX "main\.cpp")
set(automaton_lib_SRCS ${SOURCES})

#--------------------------------------------------------------------------------
# Associate source and header dependencies to target(s).
#--------------------------------------------------------------------------------
add_executable(automaton ${automaton_SRCS})

add_library(auto STATIC ${automaton_lib_SRCS})

#--------------------------------------------------------------------------------
# We need this to be able to include headers produced by uic in our code
# CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES()
#  works just like INCLUDEPATH from qmake
#--------------------------------------------------------------------------------
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}
  # ${Boost_INCLUDE_DIRS}
  # ${PROJECT_SOURCE_DIR}/include
)

# Add FXTUI dependency
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/FTXUI" "${PROJECT_BINARY_DIR}/fxtui")
include_directories("${PROJECT_SOURCE_DIR}/lib/FTXUI/include")
option(FTXUI_BUILD_EXAMPLES OFF)

#--------------------------------------------------------------------------------
# TARGET_VERSION macros are set in the top level CMakeLists.txt file.
#--------------------------------------------------------------------------------
add_definitions(
  # -DTIXML_USE_STL
  -DCMAKE_TARGET_VERSION=1
  -DTARGET_VERSION_MAJOR=${TARGET_VERSION_MAJOR}
  -DTARGET_VERSION_MINOR=${TARGET_VERSION_MINOR}
)

target_compile_features(automaton PRIVATE cxx_std_14)

#--------------------------------------------------------------------------------
# Associate linker dependencies to target(s).
#--------------------------------------------------------------------------------
target_link_libraries( automaton )
