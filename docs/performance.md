# Luna Performance Extensions

Luna includes specialized high-performance extensions built with Assembly (x86_64) to provide exceptional speed for critical operations like timing and mathematical computations.

---

## High-Precision Timing with `clock()`

Luna features a custom timing module built directly on Linux system calls (`sys_clock_gettime`), providing **nanosecond-resolution timing** essential for accurate benchmarking.

### What is `clock()`?

The `clock()` function returns the **current monotonic time** in seconds as a floating-point number (e.g., `12345.6789`).

**Monotonic time** means:

* It counts up from the moment your computer booted
* It **never goes backward** and never jumps suddenly
* Unlike wall-clock time, it's unaffected by system time adjustments
* Provides **nanosecond-resolution timing** via `struct timespec` in the C/ASM backend

### Basic Usage

```javascript
let start = clock()

# ... run your code here ...

let end = clock()
print("Time taken:", end - start, "seconds")
```

### How It Works: Step-by-Step

Consider this example:

```javascript
let start = clock()
# heavy code here
let end = clock()
print("Time taken:", end - start, "seconds")
```

**What happens internally:**

1. **`let start = clock()`**

   * Luna asks the OS for current monotonic time
   * CPU responds: "I've been on for `100.00` seconds"
   * Variable `start` stores `100.00`

2. **Your code runs**

   * Luna executes your heavy computation
   * This takes 5 seconds in real time
   * CPU keeps counting: `101... 102... 103... 104... 105...`

3. **`let end = clock()`**

   * Luna asks CPU again: "What time is it now?"
   * CPU responds: "I've been on for `105.00` seconds"
   * Variable `end` stores `105.00`

4. **`end - start`**

   * Compute: `105.00 - 100.00 = 5.00`
   * **Result: Your code took 5 seconds to run**

### Example 1: Simple Loop Benchmark

```javascript
print("Starting heavy task...")
let start = clock()

# Perform heavy task
let sum = 0
for (let i = 0; i < 1000000; i++) {
    sum = sum + i
}

let end = clock()
let duration = end - start

print("Task finished.")
print("Sum calculated:", sum)
print("Time taken:", duration, "seconds")
```

### Example 2: Matrix Multiplication

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
            let a_val = A[i][k]
            let b_val = B[k][j]
            sum = sum + (a_val * b_val)
        }
        C[i][j] = sum
    }
}

let end = clock()

# Verify result (should be 50 * 2 * 3 = 300)
let r0 = C[0]
print("Value at [0][0] (expected 300):", r0[0])
print("Time taken:", end - start, "seconds")
```

---

## Vector Mathematics (SIMD)

Luna includes hardware-accelerated vector mathematics using SIMD (Single Instruction, Multiple Data) instructions, processing lists of numbers **over 10,000× faster** than standard loops.

### Why Is It So Fast?

**Standard Loop (SISD - Single Instruction, Single Data):**

* The interpreter fetches one item at a time
* Checks its type, allocates memory, performs operation
* Repeats this overhead for every single element
* Processing: `A[0] + B[0]` → `A[1] + B[1]` → `A[2] + B[2]`

**Vector ASM (SIMD - Single Instruction, Multiple Data):**

* Data is packed into a raw C array once
* CPU uses XMM registers to process multiple numbers simultaneously
* Processing: `[A[0], A[1]] + [B[0], B[1]]`

### Automatic Vectorization

Luna **automatically** uses Assembly-optimized code when you perform operations on lists.

```javascript
let A = [1, 2, 3, 4]
let B = [5, 6, 7, 8]

let C = A + B
let D = A - B
let E = A * B
let F = A / B
```

### Manual Vector Functions

```javascript
let A = [10, 20, 30]
let B = [1, 2, 3]

let sum = vec_add(A, B)
let diff = vec_sub(A, B)
let prod = vec_mul(A, B)
let quot = vec_div(A, B)
```

---

## Technical Implementation

### Assembly Layer (`asm/vec_math.asm`)

* Uses SSE2 instructions (`addpd`, `subpd`, `mulpd`, `divpd`)
* Custom macro system for optimized operations

### C Bridge (`src/vec_lib.c`)

* Packs/unpacks dynamic Luna values
* Bridges interpreter and assembly

### Security

* `.note.GNU-stack` ensures non-executable stack
* Complies with Linux NX protections

---

## Best Practices

1. **Use automatic vectorization** when operating on lists
2. **Benchmark critical code** using `clock()`
3. **Process data in batches** (1000+ elements)
4. **Avoid mixed data types** in vector operations

---

## Notes

* `clock()` provides monotonic, nanosecond-resolution timing; actual precision depends on hardware and kernel.
* SIMD extensions are currently available only on **x86_64** platforms.
* Vector operations require equal-length, numeric-only lists; mismatches result in runtime errors.
* Vector division follows scalar semantics; division by zero results in a runtime error.
* Reported speedups are benchmark-dependent and vary by
