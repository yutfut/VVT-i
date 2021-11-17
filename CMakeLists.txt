cmake_minimum_required(VERSION 3.16.3)
project(VVT_i)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")
set(PQXX /usr/local/include/pqxx)

add_executable(VVT_i main.cpp)
add_executable(gtest.out gtest.cpp)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

#add_executable(connection.out data_base_connection.cpp)
#link_libraries(connection.out ${PQXX_LIB} ${PQ_LIB})
