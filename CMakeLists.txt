cmake_minimum_required(VERSION 3.3)
project(remote_memory)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(detail_header_files
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/error.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/utils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/windows/definitions.hpp)

set(header_files
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/operations_policy.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/windows/read_memory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/windows/write_memory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/linux/read_memory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/linux/write_memory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/osx/read_memory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/remote_memory/osx/write_memory.hpp)

include_directories (${PROJECT_SOURCE_DIR}/include)
add_library (remote_memory INTERFACE)
target_sources(remote_memory INTERFACE $<BUILD_INTERFACE:${detail_header_files} ${header_files}>)
target_include_directories(remote_memory INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_include_directories(remote_memory SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

enable_testing()
add_subdirectory (test)
