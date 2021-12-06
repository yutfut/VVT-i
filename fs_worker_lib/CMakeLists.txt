cmake_minimum_required(VERSION 3.20)
project(vvti)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall  -Wextra  -Wpedantic -fprofile-arcs -pthread")
#set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -pthread")

set(INCLUDE_DIRS include)
set(FILESYSTEM_LIB_DIR src/filesystem_lib)
set(TEST_FILESYSTEM_LIB_SRCS test/main.cpp
        test/test_fs_not_auth.cpp
        test/test_fs_auth_usr.cpp
        test/test_fs_group.cpp
        )
add_subdirectory(${FILESYSTEM_LIB_DIR})

find_package(GTest REQUIRED)

add_executable(test_filesystem_lib ${TEST_FILESYSTEM_LIB_SRCS})
target_include_directories(test_filesystem_lib PRIVATE ${GTEST_INCLUDE_DIRS} ${INCLUDE_DIRS})
target_link_libraries(test_filesystem_lib filesystem_lib)
target_link_libraries(test_filesystem_lib ${GTEST_LIBRARIES})
target_compile_definitions(test_filesystem_lib PRIVATE TEST_PATH="${CMAKE_CURRENT_SOURCE_DIR}/test")

