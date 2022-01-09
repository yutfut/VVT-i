cmake_minimum_required(VERSION 3.6)

project(server_lib)

# Set the 20th standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libraries
find_package(Boost REQUIRED COMPONENTS log log_setup system date_time regex)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)

# Find all source files
aux_source_directory(config/src CONFIG_SRC)
aux_source_directory(http/src HTTP_SRC)
aux_source_directory(logs/src LOG_SRC)
aux_source_directory(main/src MAIN_SRC)
aux_source_directory(request_handlers/src REQUEST_HANDLERS_SRC)
aux_source_directory(workers/src WORKERS_SRC)

# Create server library
add_library(server_lib STATIC ${CONFIG_SRC} ${HTTP_SRC} ${LOG_SRC} ${MAIN_SRC} ${REQUEST_HANDLERS_SRC} ${WORKERS_SRC})
target_include_directories(server_lib
  PUBLIC config/include
  http/include
  logs/include
  main/include
  workers/include
  request_handlers/include
  ../database_lib/include
  ../fs_worker_lib/include
)
target_link_libraries(
  server_lib mailio Boost::log Boost::log_setup Boost::system Boost::date_time Boost::regex
  OpenSSL::SSL Threads::Threads fmt::fmt fs_worker_lib database_lib
)
