cmake_minimum_required(VERSION 3.15)
project(os_wet3)

set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic-errors -DNDEBUG -std=c++11")

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(os_wet3
        data_handler.cpp
        data_handler.h
        handler.h
        main.cpp
        packets.h
        server.cpp
        server.h
        state.cpp
        state.h
        wrq_handler.cpp
        wrq_handler.h)
