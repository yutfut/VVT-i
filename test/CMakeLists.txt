cmake_minimum_required(VERSION 3.6)

project(test)

# Set the 20th standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find library for tests
find_package(GTest REQUIRED)

# Find all source files
# aux_source_directory(database_lib DATABASE_LIB_SRC)
# aux_source_directory(fs_worker_lib FS_WORKER_LIB_SRC)
aux_source_directory(server_lib SERVER_LIB_SRC)

# Make executable test program
add_executable(test.out main.cpp ${DATABASE_LIB_SRC} ${FS_WORKER_LIB_SRC} ${SERVER_LIB_SRC})
target_link_libraries(test.out database_lib fs_worker_lib server_lib GTest::GTest Threads::Threads)
