cmake_minimum_required(VERSION 3.6)

project(project)
set(DATABASE_LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/database_lib)

add_subdirectory(${DATABASE_LIB_SOURCES})
add_subdirectory(server)

