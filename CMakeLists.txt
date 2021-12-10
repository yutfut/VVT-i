cmake_minimum_required(VERSION 3.16)
project(VVT-i)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_DIRECTORY source)
set(INCLUDE_DIRECTORY include)

#set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
#set(CMAKE_C_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
#set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

add_library(client STATIC ${SOURCE_DIRECTORY}/client.cpp
                          ${SOURCE_DIRECTORY}/validation.cpp
                          ${SOURCE_DIRECTORY}/user.cpp
                          ${SOURCE_DIRECTORY}/command.cpp
                          ${SOURCE_DIRECTORY}/http_base.cpp
                          ${SOURCE_DIRECTORY}/http_request.cpp
                          ${SOURCE_DIRECTORY}/http_response.cpp)
target_include_directories(client PUBLIC ${INCLUDE_DIRECTORY})

add_executable(vvti_client source/main.cpp)
target_link_libraries(vvti_client client)

target_include_directories(vvti_client PUBLIC ../include)

#enable_testing()
#
#find_package(GTest REQUIRED)
#find_package(Threads REQUIRED)
#
#add_executable(vvti_client_test test/gtest.cpp)
#target_link_libraries(vvti_client_test client ${GTEST_LIBRARIES} Threads::Threads)
