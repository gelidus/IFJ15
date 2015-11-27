cmake_minimum_required(VERSION 3.3)
project(IFJ)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES
        common.h
        interpret.c
        interpret.h
        ast.c
        ast.h
        main.c
        parser.c
        parser.h
        scanner.c
        scanner.h
        stack.c
        stack.h
        string.c
        string.h
        input.h
        input.c
        errors.h
        errors.c
        symbol_table.h
        symbol_table.c
        gc.c
        gc.h ial.c ial.h test.c test.h)

add_executable(IFJ ${SOURCE_FILES})