cmake_minimum_required(VERSION 3.10)

project(MultiTest2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(
    MultiTest2
    main.cpp
    worker.cpp
)