cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FILESYSTEM_LIB_SRCS src/fs_not_auth.cpp
        src/fs_auth_usr.cpp
        src/fs_group.cpp
        src/fs_worker.cpp
        )
set(INCLUDE_DIRS include)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall  -Wextra  -Wpedantic -fprofile-arcs -pthread")
#set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -pthread")


add_library(filesystem_lib STATIC ${FILESYSTEM_LIB_SRCS})
target_include_directories(filesystem_lib PRIVATE ${INCLUDE_DIRS})