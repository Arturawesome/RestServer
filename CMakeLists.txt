cmake_minimum_required(VERSION 3.10)

project(CloudFS VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)



include_directories(${CMAKE_SOURCE_DIR}/include/cpp-httplib/src/cpp-httplib-0.18.3/)
include_directories(${CMAKE_SOURCE_DIR}/include/json/include/)
add_executable(cloudfs src/main.cpp)
