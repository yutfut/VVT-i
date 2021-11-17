cmake_minimum_required(VERSION 3.6)

project(test)

# Find package for test
find_package(GTest REQUIRED)

if(GTest_FOUND)
  add_executable(test.out src/main.cpp)
  target_include_directories(test.out PRIVATE include)
  target_link_libraries(test.out server_lib ${GTEST_LIBRARIES} Threads::Threads)
else()
  message(FATAL_ERROR "GTest has not been installed!")
endif()
