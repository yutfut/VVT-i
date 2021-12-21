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
find_package(fmt REQUIRED)

# Find all source files
aux_source_directory(src SRC)

# Create server library
add_library(database_lib ${SRC})
target_include_directories(database_lib PUBLIC include)
target_link_libraries(database_lib ${PQXX_LIB} ${PQ_LIB} fmt::fmt)

# Create main target
add_executable(main.out main.cpp)
target_link_libraries(main.out database_lib)
target_include_directories(main.out PUBLIC include)

# Create gtest target
#add_executable(gtest.out gtest.cpp)
#target_link_libraries(gtest.out ${GTEST_LIBRARIES} database_lib Threads::Threads)
#target_include_directories(gtest.out PUBLIC ./include)
