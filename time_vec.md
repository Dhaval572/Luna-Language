# Luna Programming Language - High Performance Extensions

This project implements a custom interpreter for **Luna**, a dynamic scripting language. It features specialized **Assembly (x86_64)** modules to bypass standard interpreter overhead for critical tasks like timing and mathematical operations.

## High-Precision Timing

Luna includes a custom timing module built directly on Linux system calls (`sys_clock_gettime`). This provides nanosecond-precision timing, essential for benchmarking code performance.

### Usage

The `clock()` function returns the current monotonic time in seconds.

```javascript
let start = clock()

# ... run your heavy code here ...

let end = clock()
print("Time taken:", end - start, "seconds")
```
# `clock()` in Luna – How It Works

The `clock()` function returns the **current monotonic time** in seconds as a floating-point number (for example: `12345.6789`).

## What “Monotonic Time” Means

* **Monotonic:** It counts up from the moment your computer booted.
* It **never goes backward** and never jumps suddenly forward.
* This is different from **wall-clock time** (the time you see on your system clock), which can change if you adjust your system date/time.

Because the implementation uses `struct timespec` in the C/ASM backend, `clock()` measures time with **nanosecond precision**, then returns it as a `double` (seconds with a fractional part).

## Standard Benchmarking Pattern

The primary use of `clock()` is to measure **how long some code takes to run**.

The pattern is:

1. Call `clock()` before the work and store it in a variable (start time).
2. Run your heavy code.
3. Call `clock()` again (end time).
4. Subtract: `end - start` to get the elapsed time.

In simple terms: when you call `clock()`, you are asking the system:

> "How many seconds have passed since you turned on?"

Each call is just a **snapshot of time**.

---

## Step-by-Step Example

Imagine this Luna code:

```luna
let start = clock()

# heavy code here

let end = clock()
print("Time taken:", end - start, "seconds")
```

Here is what happens internally, step by step:

### 1. `let start = clock()`

* Luna asks the OS/CPU for the current monotonic time.
* The CPU replies: "I have been on for `100.00` seconds."
* The variable `start` stores `100.00`.

### 2. Your code runs

* Luna now runs your heavy math or loop.
* This takes real time — say **5 seconds** in the real world.
* Meanwhile, the CPU keeps counting: `101... 102... 103... 104... 105...`

### 3. `let end = clock()`

* Luna asks the CPU again: "What time is it now (monotonic)?"
* The CPU responds: "I have been on for `105.00` seconds now."
* The variable `end` stores `105.00`.

### 4. `end - start`

* You compute: `105.00 - 100.00 = 5.00`.
* That result means: **your code took 5 seconds to run**.

This simple pattern is the foundation for all **benchmarking** and **performance measurements** in Luna using the `clock()` function.

## Sample codes using clock()
```javascript
# 1. Capture the start time
print("Starting heavy task...")
let start = clock()

# 2. Perform the heavy task (e.g., a loop)
let sum = 0
for (let i = 0; i < 1000000; i++) {
    sum = sum + i
}

# 3. Capture the end time
let end = clock()

# 4. Calculate elapsed time
let duration = end - start

print("Task finished.")
print("Sum calculated:", sum)
print("Time taken:", duration, "seconds")
```
## Example showing Matrix Multiplication
```javascript
func create_matrix(rows, cols, val) {
    let m = []
    for (let r = 0; r < rows; r++) {
        let row = []
        for (let c = 0; c < cols; c++) {
            append(row, val)
        }
        append(m, row)
    }
    return m
}

print("Simulating 50x50 Matrix Multiplication...")
let start = clock()

let size = 50
let A = create_matrix(size, size, 2)
let B = create_matrix(size, size, 3)
let C = create_matrix(size, size, 0)

for (let i = 0; i < size; i++) {
    for (let j = 0; j < size; j++) {
        let sum = 0
        for (let k = 0; k < size; k++) {
            # Direct access is fine for reading
            let a_val = A[i][k]
            let b_val = B[k][j]
            sum = sum + (a_val * b_val)
        }
        
        # FIX: Assign directly to C[i][j]
        # This tells the interpreter to find the memory address of the 
        # item inside C, rather than making a copy variable.
        C[i][j] = sum
    }
}
let end = clock()

# Verify one value (should be size * 2 * 3 = 50 * 6 = 300)
let r0 = C[0]
print("Value at [0][0] (should be 300):", r0[0])
print("Time taken:", end - start)
```

## Vector Mathematics (SIMD)

This extension introduces hardware-accelerated vector mathematics. By utilizing SIMD (Single Instruction, Multiple Data) instructions, Luna can process lists of numbers over 1,400× faster than standard loops.

### Why Is It So Fast?

In a standard interpreter, adding two lists of numbers is slow because of overhead:

**Scalar (Normal Loop):** The interpreter must fetch an item, check its type, allocate memory, add it, and save it. It repeats this administrative work 50,000 times for a 50k‑item list.

**Vector (ASM):** We pack the data into a raw C array once. The CPU then uses XMM registers to load and add two numbers at the exact same instant in a single clock cycle.

### Visualizing the Difference

* **Normal Loop (SISD):** `A[0] + B[0]` → `A[1] + B[1]` → `A[2] + B[2]` (processed one at a time)
* **Vector ASM (SIMD):** `[A[0], A[1]] + [B[0], B[1]]` (processed simultaneously)

### Benchmark: Addition

Comparing the time to add two lists of 50,000 items:

| Method        | Code Example                                       | Execution Time |
| ------------- | -------------------------------------------------- | -------------- |
| Standard Loop | `for (let i=0; i<len; i++) { C[i] = A[i] + B[i] }` | ~0.5780s       |
| Vector ASM    | `let C = vec_add(A, B)`                            | ~0.0004s       |

**Result:** The vector implementation is approximately **1,445× faster**.

## Available Vector Functions

Each function accepts two lists and returns a new list.

### 1. Vector Addition

```javascript
let A = [10, 20]
let B = [1, 2]
let C = vec_add(A, B)
# Result: [11.0, 22.0]
```

### 2. Vector Subtraction

```javascript
let C = vec_sub(A, B)
# Result: [9.0, 18.0]
```

### 3. Vector Multiplication

```javascript
let C = vec_mul(A, B)
# Result: [10.0, 40.0]
```

### 4. Vector Division

```javascript
let C = vec_div(A, B)
# Result: [10.0, 10.0]
```

## Vector Math Benchmark Summary

Below is a comparison of all four vectorized operations in Luna.

| Operation      | Vector Function | Luna Usage Example | Normal Loop Time | Vector ASM Time | Speedup |
| -------------- | --------------- | ------------------ | ---------------- | --------------- | ------- |
| Addition       | `vec_add`       | `vec_add(A, B)`    | 49.0219 s        | 0.00544725 s    | 8,999×  |
| Subtraction    | `vec_sub`       | `vec_sub(A, B)`    | 48.8679 s        | 0.00436844 s    | 11,186× |
| Multiplication | `vec_mul`       | `vec_mul(A, B)`    | 49.6054 s        | 0.00457386 s    | 10,845× |
| Division       | `vec_div`       | `vec_div(A, B)`    | 49.4849 s        | 0.00472969 s    | 10,462× |

## Technical Implementation

* **Assembly (`asm/vec_math.asm`):** Contains the core logic using SSE2 instructions (`addpd`, `mulpd`, etc.). Uses a custom macro system to generate optimized code for all four operations.

* **C Bridge (`src/vec_lib.c`):** Handles memory packing—converting dynamic Luna values into contiguous C arrays that the CPU can process efficiently.

* **Security:** All assembly files include the `.note.GNU-stack` section to ensure the stack is marked non-executable, complying with modern Linux security standards.

## Sample code for benchmark
```javascript
print("=== Luna Vector Math Complete Benchmark ===")
print("generating 50,000 items...")

# 1. Setup Data
let size = 50000
let A = []
let B = []

# Generate data (ensure B has no zeros for division)
for (let i = 0; i < size; i++) {
    append(A, i + 1)      # 1, 2, 3...
    append(B, (i % 10) + 1) # 1..10, 1..10 (safe for division)
}

print("Data ready. Starting tests...\n")

# --- TEST 1: ADDITION ---
print("1. Testing Addition (+)")
let start = clock()
let C_loop = []
for (let i = 0; i < size; i++) {
    append(C_loop, A[i] + B[i])
}
let mid = clock()
let C_asm = vec_add(A, B)
let end = clock()

let t_loop = mid - start
let t_asm = end - mid
print("   Loop:", t_loop, "s")
print("   ASM: ", t_asm, "s")
print("   Speedup:", t_loop / t_asm, "x")
assert(C_asm[0] == C_loop[0]) 
assert(C_asm[size-1] == C_loop[size-1])


# --- TEST 2: SUBTRACTION ---
print("\n2. Testing Subtraction (-)")
start = clock()
C_loop = []
for (let i = 0; i < size; i++) {
    append(C_loop, A[i] - B[i])
}
mid = clock()
C_asm = vec_sub(A, B)
end = clock()

t_loop = mid - start
t_asm = end - mid
print("   Loop:", t_loop, "s")
print("   ASM: ", t_asm, "s")
print("   Speedup:", t_loop / t_asm, "x")
assert(C_asm[0] == C_loop[0])


# --- TEST 3: MULTIPLICATION ---
print("\n3. Testing Multiplication (*)")
start = clock()
C_loop = []
for (let i = 0; i < size; i++) {
    append(C_loop, A[i] * B[i])
}
mid = clock()
C_asm = vec_mul(A, B)
end = clock()

t_loop = mid - start
t_asm = end - mid
print("   Loop:", t_loop, "s")
print("   ASM: ", t_asm, "s")
print("   Speedup:", t_loop / t_asm, "x")
assert(C_asm[0] == C_loop[0])


# --- TEST 4: DIVISION ---
print("\n4. Testing Division (/)")
start = clock()
C_loop = []
for (let i = 0; i < size; i++) {
    append(C_loop, A[i] / B[i])
}
mid = clock()
C_asm = vec_div(A, B)
end = clock()

t_loop = mid - start
t_asm = end - mid
print("   Loop:", t_loop, "s")
print("   ASM: ", t_asm, "s")
print("   Speedup:", t_loop / t_asm, "x")
assert(C_asm[0] == C_loop[0])

print("\n=== ALL BENCHMARKS COMPLETED ===")
```