cmake_minimum_required(VERSION 3.6)

set(FILESYSTEM_LIB_SRCS src/fs_not_auth.cpp
        src/fs_auth_usr.cpp
        src/fs_group.cpp
        src/fs_worker.cpp
        )
set(INCLUDE_DIRS include)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall  -Wextra  -Wpedantic  -pthread")
#set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -pthread")


add_library(fs_worker_lib STATIC ${FILESYSTEM_LIB_SRCS})
target_include_directories(fs_worker_lib PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(fs_worker_lib PRIVATE FS_ROOT_PATH="/home/kirill/vvti_filesystem")
target_compile_definitions(fs_worker_lib PRIVATE FS_NOT_AUTH_DIR_NAME="not_auth")
