cmake_minimum_required(VERSION 3.6)
project(ai_project1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    AIproject2017.pdf
    genome.data
    q1.cpp
    q1.data
    q1.py
    q2.data
    q2.py
    q3.data
    test.py)

add_executable(ai_project1 ${SOURCE_FILES})