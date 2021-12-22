cmake_minimum_required(VERSION 3.6)

# Create main target
add_executable(main.out ../../main.cpp)
target_link_libraries(main.out database_lib)
target_include_directories(main.out PUBLIC include)
