cmake_minimum_required(VERSION 3.6)

project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libraries
find_package(Boost REQUIRED COMPONENTS log log_setup system date_time regex)
find_package(OpenSSL)
find_package(Threads)

if (Boost_FOUND)
    # Find all source files
    aux_source_directory(config/src CONFIG_SRC)
    aux_source_directory(http/src HTTP_SRC)
    aux_source_directory(logs/src LOG_SRC)
    aux_source_directory(main/src MAIN_SRC)
    aux_source_directory(workers/src WORKERS_SRC)

    # Create server library
    add_library(server_lib STATIC ${CONFIG_SRC} ${HTTP_SRC} ${LOG_SRC} ${MAIN_SRC} ${WORKERS_SRC})
    target_include_directories( server_lib
            PUBLIC config/include
            http/include
            logs/include
            main/include
            workers/include
            ../database_lib/include
            ../fs_worker_lib/include
    )
    target_link_libraries(
        server_lib mailio Boost::log Boost::log_setup Boost::system Boost::date_time Boost::regex
        ${OPENSSL_LIBRARIES} Threads::Threads
    )
else ()
    message(FATAL_ERROR "Boost has not been installed!")
endif ()
