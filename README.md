# Linear Algebra Library in C

A small C-based numerical tool implementing basic linear algebra routines.  
The project contains a demo command-line interface to demonstrate functionality.  
It also contains test cases and a benchmark for performance.

---

## Build Instructions

### Requirements
- C compiler with C11 support
- CMake ≥ 3.10

### Build
```bash
cmake -S . -B build
cmake --build build
./build/smt
```

---

## Running Tests

```bash
./build/test_la
```

Expected output:
```
test_la: PASS
```

---

## CLI Demo

A minimal CLI is provided to demonstrate library usage:

```bash
./build/la_cli2
```

The CLI is intentionally lightweight and exists only as a usage example.  
All core functionality is exposed through the library API.

---

## Benchmark

A simple benchmark is included to measure solver performance on dense systems:

```bash
./build/la_benchmark
```

---

## Features

### Matrix operations
- Addition
- Subtraction
- Multiplication
- Transpose

### Linear algebra routines
- Determinant computation via Gaussian elimination with partial pivoting
- Linear system solver for Ax = b
- Matrix inversion

---

## Extra Notes

- Functions that write to output matrices allocate memory internally
- Output matrices must be empty on entry (`out->data == NULL`)
- The caller is responsible for freeing matrices using `la_matrix_free`
- All public functions return a `la_status` code to indicate success or failure
