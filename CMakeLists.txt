cmake_minimum_required(VERSION 3.20)
project(vvti)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall  -Wextra  -Wpedantic -fprofile-arcs -pthread")
#set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -pthread")

set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/project/include)
set(WORKING_WITH_FS_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/project/src/working_with_filesystem_lib)
set(TEST_WORKING_WITH_FS_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/test/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/project/test/test_FsSubWorkerNotAuth.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/project/test/test_FsSubWorkerAuthUsr.cpp
        )
add_subdirectory(${WORKING_WITH_FS_LIB_DIR})

find_package(GTest REQUIRED)

add_executable(test_working_with_fs ${TEST_WORKING_WITH_FS_SRCS})
target_include_directories(test_working_with_fs PRIVATE ${GTEST_INCLUDE_DIRS} ${INCLUDE_DIRS})
target_link_libraries(test_working_with_fs working_with_filesystem_lib)
target_link_libraries(test_working_with_fs ${GTEST_LIBRARIES})
#TODO: след строка нужна?
target_compile_definitions(test_working_with_fs PRIVATE TEST_PATH="${CMAKE_CURRENT_SOURCE_DIR}/project/test")

