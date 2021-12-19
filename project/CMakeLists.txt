cmake_minimum_required(VERSION 3.6)

project(project)

# Include all libraries
add_subdirectory(database_lib)
add_subdirectory(fs_worker_lib)
add_subdirectory(server_lib)
