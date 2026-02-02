cmake_minimum_required(VERSION 3.10)
project(LinearAlgebraUtilities C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Common warnings
set(LA_WARNINGS "")
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    set(LA_WARNINGS -Wall -Wextra -Wpedantic -O2)
endif()

# --------------------------
# Library: la
# --------------------------
add_library(la
    src/la_matrix.c
    src/la_ops.c
    src/la_solve.c
)

target_include_directories(la PUBLIC include)
target_compile_options(la PRIVATE ${LA_WARNINGS})

# --------------------------
# Demo CLI (uses the la library)
# --------------------------
add_executable(la_cli2
    apps/la_cli.c
    src/input.c
)

target_include_directories(la_cli2 PRIVATE include)
target_link_libraries(la_cli2 PRIVATE la)
target_compile_options(la_cli2 PRIVATE ${LA_WARNINGS})

# --------------------------
# Tests
# --------------------------
add_executable(test_la
    tests/test_la.c
)

target_link_libraries(test_la PRIVATE la)
target_compile_options(test_la PRIVATE ${LA_WARNINGS})
