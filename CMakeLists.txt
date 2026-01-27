cmake_minimum_required(VERSION 3.10)

project(ScientificMathTool C)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
endif()

set(SOURCES
    main.c
    input.c
    linalg.c
    cancel.c
)

add_executable(smt ${SOURCES})
