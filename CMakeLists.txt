cmake_minimum_required(VERSION 3.9)

project(examples_of_cpp_concurrency_programming VERSION 0.1.0 LANGUAGES CXX)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif(NOT CMAKE_CXX_STANDARD)

add_subdirectory(block_queue)