cmake_minimum_required(VERSION 3.10)
project(LinearAlgebraUtilities C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Library: la
add_library(la
    src/la_matrix.c
    src/la_ops.c
)

target_include_directories(la PUBLIC include)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(la PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()


# Legacy CLI 
add_executable(la_cli
    apps/la_cli_legacy.c
    src/la_legacy_linalg.c
    src/input.c
    src/cancel.c
)

target_include_directories(la_cli PRIVATE include)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(la_cli PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()


# New CLI demo 
add_executable(la_cli2
    apps/la_cli.c
    src/input.c
    src/cancel.c
)

target_include_directories(la_cli2 PRIVATE include)
target_link_libraries(la_cli2 PRIVATE la)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(la_cli2 PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()

# Tests (link against la)
add_executable(test_la
    tests/test_la.c
)

target_link_libraries(test_la PRIVATE la)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(test_la PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()
