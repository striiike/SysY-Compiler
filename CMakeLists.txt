cmake_minimum_required(VERSION 3.26)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler main.cpp
        Lexer.cpp
        Lexer.h
        Lexer.cpp)
