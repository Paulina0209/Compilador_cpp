cmake_minimum_required(VERSION 3.30)
project(Compilador_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

add_executable(Compilador_cpp
        src/main.cpp
        src/lexer.cpp
        src/tokens.cpp
        src/parser.cpp
        src/ast.cpp
)