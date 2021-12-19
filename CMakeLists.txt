cmake_minimum_required(VERSION 3.6)

project(VVT-i)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Make libraries
add_subdirectory(project)

# Make executable test program
add_subdirectory(test)

# Make executable sever program
add_executable(server.out main.cpp)
target_link_libraries(server.out database_lib fs_worker_lib server_lib)
