# Luna String Library

The String library provides comprehensive text manipulation, searching, and formatting functions for working with strings.
String indexing in Luna is zero-based.

---

## Inspection

Check string properties and characteristics.
| Function      | Description                        | Example                |
|--------------|------------------------------------|------------------------|
| `len(x)`     | Returns length of string or list   | `len("abc") → 3`      |
| `is_empty(s)`| Returns true if string length is 0 | `is_empty("") → true` |

### Note: len() is a generic function that works on both strings and lists. In earlier versions the string length was provided via str_len().

## Slicing & Access

Extract parts of strings or access individual characters.

| Function                   | Description                                      | Example                           |
| -------------------------- | ------------------------------------------------ | --------------------------------- |
| `char_at(s, i)`            | Returns character at index i as a string         | `char_at("ABC", 1) → "B"`         |
| `substring(s, start, len)` | Extracts len characters starting at start index  | `substring("Hello", 0, 2) → "He"` |
| `slice(s, start, end)`     | Extracts from start index up to (not including) end index | `slice("Hello", 1, 4) → "ell"`|

## Searching

Find substrings and check string patterns.

| Function                | Description                                        | Example                                  |
| ----------------------- | -------------------------------------------------- | ---------------------------------------- |
| `contains(s, sub)`      | Returns true if substring sub is found in s        | `contains("Team", "ea") → true`          |
| `index_of(s, sub)`      | Returns index of first occurrence of sub (-1 if not found) | `index_of("banana", "nan") → 2`  |
| `last_index_of(s, sub)` | Returns index of last occurrence of sub (-1 if not found)  | `last_index_of("banana", "a") → 5`|
| `starts_with(s, pre)`   | Returns true if s starts with prefix pre           | `starts_with("file.txt", "file") → true` |
| `ends_with(s, suf)`     | Returns true if s ends with suffix suf             | `ends_with("file.txt", ".txt") → true`   |

## Manipulation

Transform and modify strings.

| Function                | Description                                        | Example                              |
| ----------------------- | -------------------------------------------------- | ------------------------------------ |
| `concat(s1, s2)`        | Concatenates (joins) two strings                   | `concat("He", "llo") → "Hello"`      |
| `to_upper(s)`           | Converts all characters to uppercase               | `to_upper("abc") → "ABC"`            |
| `to_lower(s)`           | Converts all characters to lowercase               | `to_lower("ABC") → "abc"`            |
| `trim(s)`               | Removes whitespace from both ends                  | `trim("  hi  ") → "hi"`              |
| `trim_left(s)`          | Removes whitespace from the start only             | `trim_left("  hi") → "hi"`           |
| `trim_right(s)`         | Removes whitespace from the end only               | `trim_right("hi  ") → "hi"`          |
| `replace(s, old, new)`  | **Replaces all occurrences** of old with new           | `replace("foobar", "o", "a") → "faabar"`|
| `reverse(s)`            | Reverses the entire string                         | `reverse("Luna") → "anuL"`           |
| `repeat(s, n)`          | Repeats string s exactly n times                   | `repeat("Na", 3) → "NaNaNa"`         |
| `pad_left(s, w, char)`  | Pads the start of s to width w using char          | `pad_left("7", 3, "0") → "007"`      |
| `pad_right(s, w, char)` | Pads the end of s to width w using char            | `pad_right("Ok", 5, ".") → "Ok..."`  |

### **Note:** `replace()` replaces **all** matching occurrences. There is currently no single-occurrence replace function.

### Sample code:
```javascript
print(replace("one one one", "one", "two"))
```
```bash
Output: two two two
```
## Lists & Formatting

Split and join strings with lists.

| Function            | Description                                        | Example                                     |
| ------------------- | -------------------------------------------------- | ------------------------------------------- |
| `split(s, delim)`   | Splits string into a list using delimiter          | `split("a,b,c", ",") → ["a", "b", "c"]`     |
| `join(list, delim)` | Joins a list of strings into one string with delimiter | `join(["a", "b", "c"], "-") → "a-b-c"`  |

---

## Example 1: Username Validator

Validate and format usernames with various string checks.

```javascript
func validate_username(username) {
    # Remove whitespace
    let clean = trim(username)
    
    # Check if empty
    if (is_empty(clean)) {
        print("Error: Username cannot be empty")
        return false
    }
    
    # Check length
    let len = str_len(clean)
    if (len < 3) {
        print("Error: Username too short (min 3 characters)")
        return false
    }
    if (len > 20) {
        print("Error: Username too long (max 20 characters)")
        return false
    }
    
    # Check if it starts with a number (not allowed)
    let first = char_at(clean, 0)
    if (contains("0123456789", first)) {
        print("Error: Username cannot start with a number")
        return false
    }
    
    print("Username valid:", clean)
    return true
}

# Test with user input
let username = input("Enter your username: ")
validate_username(username)

# Or test with predefined values
# validate_username("  john123  ")  # Valid
# validate_username("a")            # Too short
# validate_username("9gamer")       # Starts with number
```

## Example 2: Text Formatter

Format and manipulate text for display purposes.

```javascript
func format_title(text) {
    # Convert to uppercase and add decoration
    let upper = to_upper(text)
    let border = repeat("=", str_len(upper) + 4)
    
    print(border)
    print("| " + upper + " |")
    print(border)
}

func format_list(items) {
    # Create a formatted list from array of strings
    let result = join(items, ", ")
    print("Items:", result)
    
    # Show count
    let count = len(items)
    print("Total items:", count)
}

func censor_text(text, bad_word) {
    # Replace bad words with asterisks
    let stars = repeat("*", str_len(bad_word))
    return replace(text, bad_word, stars)
}

# Test the formatters
format_title("welcome to luna")

let shopping = ["apples", "bread", "milk"]
format_list(shopping)

let message = "This is a darn good example"
let clean = censor_text(message, "darn")
print("Censored:", clean)
```
