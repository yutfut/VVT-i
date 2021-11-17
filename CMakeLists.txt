cmake_minimum_required(VERSION 3.6)

project(VVT-i)

# Make libraries
add_subdirectory(project)

# Make executable test
add_subdirectory(test)

# Make executable sever program
# add_executable(server.out main.cpp)
# target_link_libraries(server.out server_lib)
