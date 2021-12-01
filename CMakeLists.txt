cmake_minimum_required(VERSION 3.16.3)
project(VVT_i)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")
set(PQXX /usr/local/include/pqxx)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(main.out main.cpp data_base.cpp database_connection.cpp)
target_link_libraries(main.out ${PQXX_LIB} ${PQ_LIB})
add_executable(gtest.out gtest.cpp data_base.cpp database_connection.cpp)
target_link_libraries(gtest.out ${GTEST_LIBRARIES} ${PQXX_LIB} ${PQ_LIB} Threads::Threads)
