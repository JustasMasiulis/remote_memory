cmake_minimum_required(VERSION 3.3)
project(remote_memory)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories (${PROJECT_SOURCE_DIR}/include)
add_library (remote_memory INTERFACE)
target_include_directories (remote_memory INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_sources (remote_memory INTERFACE ${PROJECT_SOURCE_DIR}/include/remote_memory.hpp)

enable_testing()
add_subdirectory (test)
