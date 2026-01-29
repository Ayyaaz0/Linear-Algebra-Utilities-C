cmake_minimum_required(VERSION 3.10)
project(LinearAlgebraUtilities C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Make headers in /include available to everything
include_directories(include)

# Build the CLI (still using your legacy files for now)
add_executable(la_cli
    apps/la_cli_legacy.c
    src/la_legacy_linalg.c
    src/input.c
    src/cancel.c
)

add_executable(test_la tests/test_la.c)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(la_cli PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()
