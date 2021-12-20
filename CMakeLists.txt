cmake_minimum_required(VERSION 3.16.3)
project(VVT_i)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

add_executable(main.out main.cpp src/data_base.cpp src/database_connection.cpp
        src/db_single_auth_mode.cpp src/db_group_auth_mode.cpp src/db_not_auth_mode.cpp src/db_reg_auth.cpp src/transaction_exec.cpp)
target_link_libraries(main.out ${PQXX_LIB} ${PQ_LIB} fmt::fmt)
target_include_directories(main.out PUBLIC include)

add_executable(gtest.out gtest.cpp src/data_base.cpp src/database_connection.cpp
        src/db_single_auth_mode.cpp src/db_group_auth_mode.cpp src/db_not_auth_mode.cpp src/db_reg_auth.cpp src/transaction_exec.cpp)
target_link_libraries(gtest.out ${GTEST_LIBRARIES} ${PQXX_LIB} ${PQ_LIB} Threads::Threads fmt::fmt)
target_include_directories(gtest.out PUBLIC ./include)
