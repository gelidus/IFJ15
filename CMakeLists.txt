cmake_minimum_required(VERSION 3.3)
project(IFJ)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        common.h
        #interpret.c
        #interpret.h
        #ast.c
        #ast.h
        main.c
        parser.c
        parser.h
        scanner.c
        scanner.h
        stack.c
        stack.h
        string.c
        string.h
        #gc.h
        #instruction.h
        gc.c gc.h)

add_executable(IFJ ${SOURCE_FILES})