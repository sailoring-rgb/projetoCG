cmake_minimum_required(VERSION 3.21)
project(Fase1)

set(CMAKE_CXX_STANDARD 14)

include_directories(Generator)

add_executable(Fase1
    ENGINE/main.cpp
    Generator/main.cpp
    Generator/primitives.cpp
    Generator/primitives.h)
