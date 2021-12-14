cmake_minimum_required(VERSION 3.6)

project(project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DATABASE_LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/database_lib)
set(FS_WORKER_LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/fs_worker_lib)

add_subdirectory(${DATABASE_LIB_SOURCES})
add_subdirectory(${FS_WORKER_LIB_SOURCES})
add_subdirectory(server)

