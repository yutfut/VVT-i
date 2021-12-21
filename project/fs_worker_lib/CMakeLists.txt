cmake_minimum_required(VERSION 3.6)

project(fs_worker_lib)

# Set the 20th standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all source files
aux_source_directory(src SRC)

# Create server library
add_library(fs_worker_lib STATIC ${SRC})
target_include_directories(fs_worker_lib PRIVATE include)
#TODO hardcode!!!
target_compile_definitions(fs_worker_lib PRIVATE FS_ROOT_PATH="/home/ns/vvti_filesystem")
target_compile_definitions(fs_worker_lib PRIVATE FS_NOT_AUTH_DIR_NAME="not_auth")
