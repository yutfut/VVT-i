cmake_minimum_required(VERSION 3.16)
project(VVT-i)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wno-unknown-pragmas -Wno-sign-compare -Wwrite-strings -Wno-unused -Werror -Wpedantic -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-g -O0 -Wall -Wno-unknown-pragmas -Wno-sign-compare -Wwrite-strings -Wno-unused -Werror -Wpedantic -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

find_package(Boost REQUIRED COMPONENTS iostreams)

set(SOURCE_DIRECTORY source)
set(INCLUDE_DIRECTORY include)

add_library(client STATIC ${SOURCE_DIRECTORY}/client.cpp
                          ${SOURCE_DIRECTORY}/validation.cpp
                          ${SOURCE_DIRECTORY}/user.cpp
                          ${SOURCE_DIRECTORY}/command.cpp
                          ${SOURCE_DIRECTORY}/http_base.cpp
                          ${SOURCE_DIRECTORY}/http_request.cpp
                          ${SOURCE_DIRECTORY}/http_response.cpp
                          ${SOURCE_DIRECTORY}/password.cpp)
target_include_directories(client PUBLIC ${INCLUDE_DIRECTORY})
target_link_libraries(client PRIVATE Boost::iostreams)

add_executable(vvti_client source/main.cpp)
target_link_libraries(vvti_client Boost::iostreams)
target_link_libraries(vvti_client client)

target_include_directories(vvti_client PUBLIC ../include)

enable_testing()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(vvti_client_test test/main.cpp
                                test/test_http_request.cpp
                                test/test_validation.cpp
                                test/test_http_response.cpp)
target_link_libraries(vvti_client_test client ${GTEST_LIBRARIES} Threads::Threads)
