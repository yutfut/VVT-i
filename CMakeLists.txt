cmake_minimum_required(VERSION 3.6)

project(project)

# Set the 20th standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libraries
find_library(PQXX_LIB pqxx REQUIRED)
find_library(PQ_LIB pq REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)

# Find all source files
aux_source_directory(project/database_lib/src SRC)

# Create server library
add_library(database_lib ${SRC})
target_include_directories(database_lib PUBLIC project/database_lib/include)
target_link_libraries(database_lib ${PQXX_LIB} ${PQ_LIB} fmt::fmt)

add_subdirectory(project/database_lib)
add_subdirectory(test/database_lib)
