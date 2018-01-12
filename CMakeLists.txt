cmake_minimum_required(VERSION 3.3)
project(remote_memory)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14")
endif()

set(detail_header_files
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/error.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/utils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/windows/definitions.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/windows/read_memory.inl
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/windows/write_memory.inl
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/windows/safe_handle.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/linux/read_memory.inl
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/linux/write_memory.inl
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/linux/safe_handle.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/osx/read_memory.inl
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/osx/write_memory.inl
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/detail/osx/safe_handle.hpp)

set(header_files
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/operations_policy.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/read_memory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/write_memory.hpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(remote_memory INTERFACE)
target_sources(remote_memory INTERFACE $<BUILD_INTERFACE:${detail_header_files} ${header_files}>)
target_include_directories(remote_memory INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_include_directories(remote_memory SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

enable_testing()
add_subdirectory(test)
