cmake_minimum_required(VERSION 3.8)
project(remote_memory)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES library.cpp remote_memory.hpp)
add_library(remote_memory ${SOURCE_FILES})