cmake_minimum_required(VERSION 3.6)

project(test)

# Find package for test
find_package(GTest REQUIRED)

if(GTest_FOUND)
  # Find all source files
  aux_source_directory(server/src SERVER_SRC)

  add_executable(test.out main.cpp ${SERVER_SRC})
  target_link_libraries(test.out server_lib ${GTEST_LIBRARIES} Threads::Threads)
else()
  message(FATAL_ERROR "GTest has not been installed!")
endif()
