cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WORKING_WITH_FILESYSTEM_LIB_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/working_with_filesystem.cpp)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -pthread")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/project/include)

add_library(working_with_filesystem_lib STATIC ${WORKING_WITH_FILESYSTEM_LIB_SRCS})
target_include_directories(working_with_filesystem_lib PRIVATE ${INCLUDE_DIRS})