cmake_minimum_required(VERSION 3.16.3)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

#set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")
set(DATABASE_LIB_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/)
message(${DATABASE_LIB_INCLUDES})
#set(DATABASE_LIB_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/data_base.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/src/database_connection.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/src/db_not_auth_mode.cpp
#        )
aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR}/src/ DATABASE_LIB_SRCS)

add_library(database_lib ${DATABASE_LIB_SRCS})
target_include_directories(database_lib PUBLIC ${DATABASE_LIB_INCLUDES})
target_link_libraries(database_lib ${PQXX_LIB} ${PQ_LIB})

add_executable(db_main main.cpp)
target_include_directories( db_main PUBLIC ${DATABASE_LIB_INCLUDES})
target_link_libraries(db_main database_lib ${PQXX_LIB} ${PQ_LIB})
