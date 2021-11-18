cmake_minimum_required(VERSION 3.20)
project(vvti_client)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_DIRECTORY source)
set(INCLUDE_DIRECTORY include)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

add_library(client STATIC ${SOURCE_DIRECTORY}/client.cpp)
target_include_directories(client PUBLIC ${INCLUDE_DIRECTORY})

add_executable(hw-1 source/main.cpp)
target_link_libraries(hw-1 client)

enable_testing()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(test.out test/gtest.cpp)
target_link_libraries(test.out client ${GTEST_LIBRARIES} Threads::Threads)
