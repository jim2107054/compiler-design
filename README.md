# SMPL - Space Mission Programming Language

A domain-specific programming language lexer that uses space exploration terminology for programming constructs. This project implements lexical analysis using **Flex** as part of a compiler design course.

## 🚀 Overview

SMPL (Space Mission Programming Language) is a creative, human-readable language where:
- Programs are **missions**
- Variables are **cargo**
- Operations are **spacecraft maneuvers**
- Control structures are **mission protocols**

The lexer tokenizes SMPL source code and outputs tokens suitable for parsing with Bison.

## 📁 Project Structure

```
compiler-design/
├── smpl_lexer.l           # Flex specification file
├── lex.yy.c               # Generated C code (from Flex)
├── samples/               # Sample SMPL programs
│   ├── sample1_basic.smpl
│   ├── sample2_conditionals.smpl
│   ├── sample3_loops.smpl
│   ├── sample4_functions.smpl
│   └── sample5_complete.smpl
├── output/                # Token output files
├── SMPL_PROPOSAL.md       # Full language specification
├── COMMAND_GUIDE.md       # Detailed usage guide
└── README.md              # This file
```

## ⚙️ Prerequisites

- **Flex** (version 2.5.4 or later)
- **GCC** (version 10.0 or later)

On Windows, use MSYS2 or MinGW.

```powershell
# Verify installation
flex --version
gcc --version
```

## 🔧 Build & Run

### Compile the Lexer

```powershell
# Generate C code from Flex specification
flex smpl_lexer.l

# Compile the executable
gcc -o smpl_lexer.exe lex.yy.c

# Or combine both steps
flex smpl_lexer.l; gcc -o smpl_lexer.exe lex.yy.c
```

### Run the Lexer

```powershell
# Output to console
.\smpl_lexer.exe samples\sample1_basic.smpl

# Output to file
.\smpl_lexer.exe samples\sample1_basic.smpl output\tokens.txt
```

## 📝 Complete Keywords Mapping (SMPL ↔ C)

### Program Structure

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `MISSION` | `main()` | Program entry point |
| `LANDING` | `return 0;` | Program exit |
| `LAUNCH` | `{` | Block start |
| `ABORT` | `}` | Block end |

### Data Types

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `CARGO_INT` | `int` | Integer type |
| `CARGO_FLOAT` | `float` | Float type |
| `CARGO_CHAR` | `char` | Character type |
| `CARGO_DOUBLE` | `double` | Double type |
| `VOID_SPACE` | `void` | Void type |
| `CARGO_ARRAY` | `[]` | Array declaration |

### Variable Declaration & Assignment

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `LOAD` | (declaration) | Variable declaration |
| `STORE` | `=` | Assignment operator |

### Arithmetic Operators

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `COMBINE` | `+` | Addition |
| `REDUCE` | `-` | Subtraction |
| `AMPLIFY` | `*` | Multiplication |
| `SPLIT` | `/` | Division |
| `REMAINDER` | `%` | Modulus |
| `BOOST` | `++` | Increment |
| `DEGRADE` | `--` | Decrement |

### Relational Operators

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `EXCEEDS` | `>` | Greater than |
| `BELOW` | `<` | Less than |
| `EXCEEDS_OR_EQUAL` | `>=` | Greater than or equal |
| `BELOW_OR_EQUAL` | `<=` | Less than or equal |
| `MATCHES` | `==` | Equal to |
| `DIFFERS` | `!=` | Not equal to |

### Logical Operators

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `BOTH` | `&&` | Logical AND |
| `EITHER` | `\|\|` | Logical OR |
| `NEGATE` | `!` | Logical NOT |

### Conditional Statements

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `CHECK_IF` | `if` | If condition |
| `ELSE_CHECK` | `else if` | Else-if condition |
| `OTHERWISE` | `else` | Else block |

### Switch-Case

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `PROTOCOL` | `switch` | Switch statement |
| `SCENARIO` | `case` | Case label |
| `DEFAULT_PROTOCOL` | `default` | Default case |

### Loop Constructs

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `ORBIT_WHILE` | `while` | While loop |
| `SEQUENCE` | `for` | For loop |
| `REPEAT` | `do` | Do-while (do part) |
| `UNTIL` | `while` | Do-while (condition) |

### Loop Control

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `TERMINATE` | `break` | Break statement |
| `SKIP` | `continue` | Continue statement |

### Functions

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `FUNCTION` | (function def) | Function declaration |
| `RETURN_CARGO` | `return` | Return statement |

### Input/Output

| SMPL Keyword | C Equivalent | Description |
|--------------|--------------|-------------|
| `TRANSMIT` | `printf` | Output/print |
| `RECEIVE` | `scanf` | Input/read |

## 💡 Example

### SMPL Code
```
MISSION
    LOAD CARGO_INT fuel STORE 100;
    LOAD CARGO_FLOAT temperature STORE 25.5;
    
    TRANSMIT "Mission initialized";
    TRANSMIT fuel;
    
    fuel STORE COMBINE fuel 50;
    BOOST fuel;
    
    TRANSMIT "Mission complete";
LANDING;
```

### Token Output
```
TOKEN TYPE           LEXEME                    LINE
--------------------------------------------------------------------------------
MISSION              MISSION                   1
LOAD                 LOAD                      2
CARGO_INT            CARGO_INT                 2
IDENTIFIER           fuel                      2
STORE                STORE                     2
INTEGER              100                       2
SEMICOLON            ;                         2
...
```

## 📚 Documentation

- [SMPL_PROPOSAL.md](SMPL_PROPOSAL.md) - Complete language specification and grammar
- [COMMAND_GUIDE.md](COMMAND_GUIDE.md) - Detailed compilation and usage instructions

## 🎯 Features

- ✅ Multiple data types (int, float, char, double)
- ✅ Arithmetic and logical operators
- ✅ Relational comparisons
- ✅ Conditional statements (if/else)
- ✅ Loop constructs (while, for, do-while)
- ✅ Switch-case statements
- ✅ Functions with return values
- ✅ Array support
- ✅ Input/Output operations
- ✅ Single-line and multi-line comments
- ✅ Line number tracking

## 👤 Author

**JIM2107054**

---

*This project was created as part of a Compiler Design course to demonstrate lexical analysis concepts using Flex.*
