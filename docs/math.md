# Luna Math Library

The Math library provides comprehensive mathematical operations for numerical computing, trigonometry, randomization, and more.

## Basic Utility Functions

Common mathematical utilities for everyday calculations.

| Function             | Description                                       | Example                 |
| -------------------- | ------------------------------------------------- | ----------------------- |
| `abs(x)`             | Returns the absolute value of x                   | `abs(-10) → 10`         |
| `min(a, b)`          | Returns the smaller of two values                 | `min(10, 5) → 5`        |
| `max(a, b)`          | Returns the larger of two values                  | `max(10, 5) → 10`       |
| `clamp(x, min, max)` | Constrains x to be within the range [min, max]    | `clamp(15, 0, 10) → 10` |
| `sign(x)`            | Returns -1 if negative, 1 if positive, 0 if zero  | `sign(-50) → -1`        |

## Powers & Roots

Exponential and logarithmic operations.

| Function         | Description                                | Example                 |
| ---------------- | ------------------------------------------ | ----------------------- |
| `pow(base, exp)` | Returns base raised to the power of exp    | `pow(2, 3) → 8.0`       |
| `sqrt(x)`        | Returns the square root of x               | `sqrt(16) → 4.0`        |
| `cbrt(x)`        | Returns the cube root of x                 | `cbrt(27) → 3.0`        |
| `exp(x)`         | Returns e raised to the power of x         | `exp(1) → 2.718...`     |
| `ln(x)`          | Returns the natural logarithm (base e) of x| `ln(10) → 2.302...`     |
| `log10(x)`       | Returns the base-10 logarithm of x         | `log10(100) → 2.0`      |

## Rounding Functions

Control how numbers are rounded.

| Function    | Description                                    | Example                 |
| ----------- | ---------------------------------------------- | ----------------------- |
| `floor(x)`  | Rounds down to the nearest integer             | `floor(5.9) → 5`        |
| `ceil(x)`   | Rounds up to the nearest integer               | `ceil(5.1) → 6`         |
| `round(x)`  | Rounds to the nearest integer                  | `round(5.5) → 6`        |
| `trunc(x)`  | Removes the fractional part (rounds toward 0)  | `trunc(5.9) → 5`        |
| `fract(x)`  | Returns only the fractional part               | `fract(5.9) → 0.9`      |
| `mod(x, y)` | Returns the floating-point remainder of x / y  | `mod(5.5, 2) → 1.5`     |

## Trigonometry

Standard trigonometric functions (angles in radians).

| Function      | Description                                    | Example                    |
| ------------- | ---------------------------------------------- | -------------------------- |
| `sin(rad)`    | Returns sine of an angle (in radians)          | `sin(0) → 0.0`             |
| `cos(rad)`    | Returns cosine of an angle (in radians)        | `cos(0) → 1.0`             |
| `tan(rad)`    | Returns tangent of an angle (in radians)       | `tan(0) → 0.0`             |
| `asin(x)`     | Returns arc sine (inverse sine)                | `asin(0) → 0.0`            |
| `acos(x)`     | Returns arc cosine (inverse cosine)            | `acos(1) → 0.0`            |
| `atan(x)`     | Returns arc tangent (inverse tangent)          | `atan(0) → 0.0`            |
| `atan2(y, x)` | Returns arc tangent of y/x with correct quadrant | `atan2(10, 10) → 0.785...` |

## Hyperbolic Functions

Hyperbolic trigonometric functions.

| Function  | Description                      | Example             |
| --------- | -------------------------------- | ------------------- |
| `sinh(x)` | Returns hyperbolic sine of x     | `sinh(0) → 0.0`     |
| `cosh(x)` | Returns hyperbolic cosine of x   | `cosh(0) → 1.0`     |
| `tanh(x)` | Returns hyperbolic tangent of x  | `tanh(0) → 0.0`     |

## Random Number Generation

Generate random numbers for simulations and games.

| Function            | Description                                          | Example              |
| ------------------- | ---------------------------------------------------- | -------------------- |
| `rand()`            | Returns a random float between 0.0 and 1.0           | `rand() → 0.42...`   |
| `randint(min, max)` | Returns a random integer in the range [min, max]     | `randint(1, 10) → 7` |
| `srand(seed)`       | Seeds the random number generator for reproducibility| `srand(123)`         |

## Conversion & Interpolation

Useful conversions and interpolation functions.

| Function          | Description                                       | Example                       |
| ----------------- | ------------------------------------------------- | ----------------------------- |
| `deg_to_rad(deg)` | Converts degrees to radians                       | `deg_to_rad(180) → 3.14...`   |
| `rad_to_deg(rad)` | Converts radians to degrees                       | `rad_to_deg(3.14) → 179.9...` |
| `lerp(a, b, t)`   | Linear interpolation between a and b by factor t  | `lerp(0, 100, 0.5) → 50.0`    |

---

## Example 1: Distance Calculator

Calculate the distance between two points using the Pythagorean theorem.

```javascript
# Distance between two 2D points
func distance(x1, y1, x2, y2) {
    let dx = x2 - x1
    let dy = y2 - y1
    let dist_squared = pow(dx, 2) + pow(dy, 2)
    return sqrt(dist_squared)
}

let d = distance(0, 0, 3, 4)
print("Distance:", d)  # Output: 5.0
```

## Example 2: Dice Roll Simulator

Simulate rolling multiple dice and calculate statistics.

```javascript
# Roll a 6-sided die multiple times
func roll_dice(num_rolls) {
    # Using trand() ensures every "make run" gives you new results
    srand(trand()) 
    
    let rolls = []
    let total = 0
    let min_roll = 6
    let max_roll = 1
    
    for (let i = 0; i < num_rolls; i++) {
        let roll = rand(1, 7) 
        
        append(rolls, roll)
        total = total + roll
        
        min_roll = min(min_roll, roll)
        max_roll = max(max_roll, roll)
    }
    
    let average = float(total) / float(num_rolls)
    
    print("Results")
    print("Rolls:   ", rolls)
    print("Total:   ", total)
    print("Average: ", average)
    print("Min:     ", min_roll)
    print("Max:     ", max_roll)
}

roll_dice(10)
```
