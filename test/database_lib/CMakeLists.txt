cmake_minimum_required(VERSION 3.6)

project(ProjectName)

# Find libraries
find_library(PQXX_LIB pqxx REQUIRED)
find_library(PQ_LIB pq REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)

aux_source_directory(src DB_TEST_SRC)

# Create gtest target
add_executable(gtest.out gtest.cpp ${DB_TEST_SRC})
target_link_libraries(gtest.out ${GTEST_LIBRARIES} database_lib Threads::Threads)
target_include_directories(gtest.out PUBLIC ../../project/database_lib/include)
