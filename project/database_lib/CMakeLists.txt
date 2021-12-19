cmake_minimum_required(VERSION 3.6)

project(database_lib)

# Set the 20th standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libraries
find_library(PQXX_LIB pqxx REQUIRED)
find_library(PQ_LIB pq REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Find all source files
aux_source_directory(src SRC)

# Create server library
add_library(database_lib ${SRC})
target_include_directories(database_lib PUBLIC include)
target_link_libraries(database_lib ${PQXX_LIB} ${PQ_LIB})
