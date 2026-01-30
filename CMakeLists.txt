cmake_minimum_required(VERSION 3.10)
project(LinearAlgebraUtilities C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Library
add_library(la
    src/la_matrix.c
    src/la_ops.c
)

target_include_directories(la PUBLIC include)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(la PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()

# Legacy CLI (still works, still uses old code for now)
add_executable(la_cli
    apps/la_cli_legacy.c
    src/la_legacy_linalg.c
    src/input.c
    src/cancel.c
)
target_include_directories(la_cli PRIVATE include)

# Tests for the new library
add_executable(test_la tests/test_la.c)
target_link_libraries(test_la PRIVATE la)
