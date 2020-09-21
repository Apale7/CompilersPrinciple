cmake_minimum_required(VERSION 3.16)
project(CompilersPrinciple)

set(CMAKE_CXX_STANDARD 14)

add_executable(CompilersPrinciple main.cpp Preprocessor.cpp Preprocessor.h LexicalAnalyzer.cpp LexicalAnalyzer.h)