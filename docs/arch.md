# Luna Architecture Documentation

This document provides a basic overview of Luna's internal architecture and implementation.

## Architecture Overview

The interpreter processes code in the following pipeline order:

### Source Code (.lu)

The raw text file containing the source code is read into memory.

### Lexer (lexer.c / token.c)

The source code is converted into a stream of tokens.

* Handles whitespace skipping and comment removal (# and //)
* Comments are discarded during tokenization and do not appear in the AST
* Recognizes keywords (let, if, func, for, switch), literals (numbers, strings), and operators

**Output:** A sequence of tokens (e.g., T_LET, T_IDENT, T_EQ, T_NUMBER)

### Parser (parser.c)

The tokens are analyzed to ensure they follow the grammar rules.

* Constructs an Abstract Syntax Tree (AST) defined in ast.c
* Handles operator precedence for mathematical expressions
* Groups statements into blocks (functions, loops, conditional bodies)

**Output:** A hierarchical tree of nodes (e.g., NODE_FUNC_DEF, NODE_WHILE, NODE_BINOP)

### Interpreter (interpreter.c / value.c)

The AST is traversed recursively to execute the program.

* **Environment:** Manages memory scopes. Functions create new local scopes; global variables exist in the root scope
* **Execution:** Performs arithmetic, executes control flow logic, and handles I/O

**Output:** The actual program results printed to the console

---

## File Structure & Explanation

### Source Files (src/)

* **src/main.c**: Entry point. Reads the file, initializes the parser, and calls the interpreter
* **src/lexer.c**: Scans the source string and produces tokens. Handles string literals, numbers (int/float), and comments
* **src/parser.c**: Consumes tokens to build the AST. Contains logic for grammar rules (expressions, statements, blocks)
* **src/ast.c**: Defines AST node structures and functions to create/free them
* **src/interpreter.c**: Core runtime for executing the AST. Handles variable lookups, function calls, and control flow (return/break/continue)
* **src/value.c**: Manages the dynamic Value type (Int, Float, String, List, Bool) and memory management for values
* **src/token.c**: Helper to convert token enums to string names for debugging
* **src/util.c**: File reading utilities

### Header Files (include/)

* **include/*.h**: Header files defining the structs and function prototypes for the corresponding .c files

---

## Execution Flow

```
Source Code (.lu)
    ↓
Lexer (Tokenization)
    ↓
Parser (AST Generation)
    ↓
Interpreter (Execution)
    ↓
Output
```

---

## Memory Management

Luna uses a dynamic value system where all values are represented by a `Value` struct that can hold different types:

* Integers
* Floats
* Strings
* Lists
* Booleans

The interpreter manages memory through scoped environments; allocation and deallocation are handled based on scope lifetime.

---

## Scope Management

Luna uses lexical scoping:

* **Global Scope**: Variables defined at the top level
* **Function Scope**: Each function creates a new local scope
* **Block Scope**: Control structures (if, while, for) create nested scopes

Variables are resolved by searching from the innermost scope outward to parent scopes.

---

## Token Types

The lexer recognizes the following token categories:

* **Keywords**: `let`, `if`, `else`, `while`, `for`, `func`, `return`, `break`, `continue`, `switch`, `case`, `default`
* **Literals**: Numbers (int/float), strings, booleans
* **Operators**: `+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`, `!`
* **Delimiters**: `(`, `)`, `{`, `}`, `[`, `]`, `,`, `;`
* **Assignment**: `=`
* **Comments**: `#`, `//`

---

## AST Node Types

The parser constructs an Abstract Syntax Tree with the following node types:

* **NODE_PROGRAM**: Root node containing all statements
* **NODE_LET**: Variable declaration
* **NODE_ASSIGN**: Variable assignment
* **NODE_BINOP**: Binary operation (arithmetic, comparison, logical)
* **NODE_UNOP**: Unary operation (negation, logical not)
* **NODE_IF**: Conditional statement
* **NODE_WHILE**: While loop
* **NODE_FOR**: For loop
* **NODE_FUNC_DEF**: Function definition
* **NODE_FUNC_CALL**: Function call
* **NODE_RETURN**: Return statement
* **NODE_BREAK**: Break statement
* **NODE_CONTINUE**: Continue statement
* **NODE_SWITCH**: Switch statement
* **NODE_CASE**: Case in switch
* **NODE_LIST**: List literal
* **NODE_INDEX**: List/string indexing
* **NODE_IDENT**: Identifier (variable name)# Luna Architecture Documentation

This document provides a basic overview of Luna's internal architecture and implementation.

## Architecture Overview

The interpreter processes code in the following pipeline order:

### Source Code (.lu)
The raw text file containing the source code is read into memory.

### Lexer (lexer.c / token.c)
The source code is converted into a stream of tokens.

- Handles whitespace skipping and comment removal (# and //)
- Recognizes keywords (let, if, func, for, switch), literals (numbers, strings), and operators

**Output:** A sequence of tokens (e.g., T_LET, T_IDENT, T_EQ, T_NUMBER)

### Parser (parser.c)
The tokens are analyzed to ensure they follow the grammar rules.

- Constructs an Abstract Syntax Tree (AST) defined in ast.c
- Handles operator precedence for mathematical expressions
- Groups statements into blocks (functions, loops, conditional bodies)

**Output:** A hierarchical tree of nodes (e.g., NODE_FUNC_DEF, NODE_WHILE, NODE_BINOP)

### Interpreter (interpreter.c / value.c)
The AST is traversed recursively to execute the program.

- **Environment:** Manages memory scopes. Functions create new local scopes; global variables exist in the root scope
- **Execution:** Performs arithmetic, executes control flow logic, and handles I/O

**Output:** The actual program results printed to the console

---

## File Structure & Explanation

### Source Files (src/)

- **src/main.c**: Entry point. Reads the file, initializes the parser, and calls the interpreter
- **src/lexer.c**: Scans the source string and produces tokens. Handles string literals, numbers (int/float), and comments
- **src/parser.c**: Consumes tokens to build the AST. Contains logic for grammar rules (expressions, statements, blocks)
- **src/ast.c**: Defines AST node structures and functions to create/free them
- **src/interpreter.c**: Core runtime for executing the AST. Handles variable lookups, function calls, and control flow (return/break/continue)
- **src/value.c**: Manages the dynamic Value type (Int, Float, String, List, Bool) and memory management for values
- **src/token.c**: Helper to convert token enums to string names for debugging
- **src/util.c**: File reading utilities

### Header Files (include/)

- **include/*.h**: Header files defining the structs and function prototypes for the corresponding .c files

---

## Execution Flow

```
Source Code (.lu)
    ↓
Lexer (Tokenization)
    ↓
Parser (AST Generation)
    ↓
Interpreter (Execution)
    ↓
Output
```

---

## Memory Management

Luna uses a dynamic value system where all values are represented by a `Value` struct that can hold different types:
- Integers
- Floats
- Strings
- Lists
- Booleans

The interpreter manages memory through scoped environments, automatically handling allocation and deallocation as needed.

---

## Scope Management

Luna uses lexical scoping:
- **Global Scope**: Variables defined at the top level
- **Function Scope**: Each function creates a new local scope
- **Block Scope**: Control structures (if, while, for) create nested scopes

Variables are resolved by searching from the innermost scope outward to parent scopes.

---

## Token Types

The lexer recognizes the following token categories:
- **Keywords**: `let`, `if`, `else`, `while`, `for`, `func`, `return`, `break`, `continue`, `switch`, `case`, `default`
- **Literals**: Numbers (int/float), strings, booleans
- **Operators**: `+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`, `!`
- **Delimiters**: `(`, `)`, `{`, `}`, `[`, `]`, `,`, `;`
- **Assignment**: `=`
- **Comments**: `#`, `//`

---

## AST Node Types

The parser constructs an Abstract Syntax Tree with the following node types:
- **NODE_PROGRAM**: Root node containing all statements
- **NODE_LET**: Variable declaration
- **NODE_ASSIGN**: Variable assignment
- **NODE_BINOP**: Binary operation (arithmetic, comparison, logical)
- **NODE_UNOP**: Unary operation (negation, logical not)
- **NODE_IF**: Conditional statement
- **NODE_WHILE**: While loop
- **NODE_FOR**: For loop
- **NODE_FUNC_DEF**: Function definition
- **NODE_FUNC_CALL**: Function call
- **NODE_RETURN**: Return statement
- **NODE_BREAK**: Break statement
- **NODE_CONTINUE**: Continue statement
- **NODE_SWITCH**: Switch statement
- **NODE_CASE**: Case in switch
- **NODE_LIST**: List literal
- **NODE_INDEX**: List/string indexing
- **NODE_IDENT**: Identifier (variable name)
- **NODE_NUMBER**: Numeric literal
- **NODE_STRING**: String literal
- **NODE_BOOL**: Boolean literal

---

## Built-in Functions Implementation

Built-in functions are implemented directly in the interpreter:
- **print()**: Outputs values to stdout
- **input()**: Reads user input from stdin
- **len()**: Returns length of strings/lists
- **int()**: Type conversion to integer
- **float()**: Type conversion to float
- **type()**: Returns type name as string
- **append()**: Adds element to list

Additional functions are available through the standard library modules (math, string, file).

---

* **NODE_NUMBER**: Numeric literal
* **NODE_STRING**: String literal
* **NODE_BOOL**: Boolean literal

---

## Built-in Functions Implementation

Built-in functions are implemented directly in the interpreter:

* **print()**: Outputs values to stdout
* **input()**: Reads user input from stdin
* **len()**: Returns length of strings/lists
* **int()**: Type conversion to integer
* **float()**: Type conversion to float
* **type()**: Returns type name as string
* **append()**: Adds element to list

**Additional functions are available through the standard library modules (math, string, file).**

---
