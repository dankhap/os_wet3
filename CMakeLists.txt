cmake_minimum_required(VERSION 3.15)
project(ttftps)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic-errors -DNDEBUG -std=c++11 -pthread")

add_executable (ttftps main.cpp runnable.cpp server.cpp logger.cpp)


# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# helloDemo
