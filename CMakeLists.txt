cmake_minimum_required(VERSION 3.30)
project(ledt)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(vendor)
add_executable(ledt main.cpp)
target_link_libraries(ledt PRIVATE vendor)