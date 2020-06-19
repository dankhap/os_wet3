cmake_minimum_required(VERSION 3.15)
project(ttftps)

set(CMAKE_CXX_STANDARD 11)
# removed pedentic erros and WERROR for now... TODO: bring back before submission
set(CMAKE_CXX_FLAGS "-Wall  -DNDEBUG -std=c++11 -pthread")

add_executable (ttftps main.cpp server.cpp packets.h)


# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# helloDemo
