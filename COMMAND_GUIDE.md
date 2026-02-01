# SMPL Lexer - Command Guide

## Overview

This guide explains how to compile and use the SMPL (Space Mission Programming Language) lexical analyzer.

---

## Prerequisites

- **Flex**: Version 2.5.4 or later
- **GCC**: Version 10.0 or later (MSYS2 or MinGW recommended on Windows)

### Verify Installation

```powershell
flex --version
gcc --version
```

---

## Directory Structure

```
compiler_lab/
├── smpl_lexer.l          # Flex specification file
├── samples/              # Sample SMPL programs
│   ├── sample1_basic.smpl
│   ├── sample2_conditionals.smpl
│   ├── sample3_loops.smpl
│   ├── sample4_functions.smpl
│   └── sample5_complete.smpl
├── output/               # Token output files (created after running)
├── SMPL_PROPOSAL.md      # Language proposal documentation
└── COMMAND_GUIDE.md      # This file
```

---

## Compilation Steps

### Step 1: Generate C code from Flex specification

```powershell
flex smpl_lexer.l
```

This creates `lex.yy.c` in the current directory.

### Step 2: Compile the generated C code

```powershell
gcc -o smpl_lexer.exe lex.yy.c
```

This creates the executable `smpl_lexer.exe`.

### One-liner (both steps combined)

```powershell
flex smpl_lexer.l; gcc -o smpl_lexer.exe lex.yy.c
```

---

## Usage

### Basic Usage (output to console)

```powershell
.\smpl_lexer.exe <input_file>
```

### Output to File

```powershell
.\smpl_lexer.exe <input_file> <output_file>
```

---

## Examples

### Example 1: Tokenize basic sample (console output)

```powershell
.\smpl_lexer.exe samples\sample1_basic.smpl
```

### Example 2: Tokenize with file output

```powershell
# Create output directory first
mkdir output -ErrorAction SilentlyContinue

# Run lexer with output file
.\smpl_lexer.exe samples\sample1_basic.smpl output\sample1_tokens.txt
```

### Example 3: Process all samples

```powershell
mkdir output -ErrorAction SilentlyContinue
.\smpl_lexer.exe samples\sample1_basic.smpl output\sample1_tokens.txt
.\smpl_lexer.exe samples\sample2_conditionals.smpl output\sample2_tokens.txt
.\smpl_lexer.exe samples\sample3_loops.smpl output\sample3_tokens.txt
.\smpl_lexer.exe samples\sample4_functions.smpl output\sample4_tokens.txt
.\smpl_lexer.exe samples\sample5_complete.smpl output\sample5_tokens.txt
```

---

## Output Format

The lexer outputs tokens in the following format:

```
TOKEN TYPE           LEXEME                    LINE
--------------------------------------------------------------------------------
MISSION              MISSION                   1
LOAD                 LOAD                      3
CARGO_INT            CARGO_INT                 3
IDENTIFIER           fuel                      3
STORE                STORE                     3
INTEGER              100                       3
SEMICOLON            ;                         3
...
```

### Token Categories

| Category        | Examples                                    |
|-----------------|---------------------------------------------|
| Keywords        | MISSION, LANDING, LAUNCH, ABORT, etc.       |
| Data Types      | CARGO_INT, CARGO_FLOAT, CARGO_CHAR, etc.    |
| Operators       | COMBINE, REDUCE, EXCEEDS, BOTH, etc.        |
| Literals        | INTEGER, FLOAT_NUM, STRING_LITERAL, etc.    |
| Identifiers     | Variable names (fuel, counter, etc.)        |
| Punctuation     | SEMICOLON, COLON, LPAREN, RPAREN, etc.      |

---

## Troubleshooting

### Error: flex not recognized

Make sure Flex is installed and in your PATH:
```powershell
# Check if flex is in PATH
Get-Command flex
```

### Error: gcc not recognized

Make sure GCC is installed (via MSYS2 or MinGW):
```powershell
# Check if gcc is in PATH
Get-Command gcc
```

### Lexical Error: Unrecognized character

This means the input file contains characters not defined in the SMPL language. Check:
- File encoding (should be UTF-8 or ASCII)
- Special characters that aren't part of SMPL

---

## Next Steps (Bison Integration)

After lexical analysis is complete, the next phase is syntax analysis with Bison. The token output format is designed to be compatible with Bison parser integration.

---

## Token Reference

### All 37 SMPL Keywords

| # | Token | Description |
|---|-------|-------------|
| 1 | MISSION | Program entry point |
| 2 | LANDING | Program exit |
| 3 | LAUNCH | Block start ({) |
| 4 | ABORT | Block end (}) |
| 5 | CARGO_INT | Integer type |
| 6 | CARGO_FLOAT | Float type |
| 7 | CARGO_CHAR | Character type |
| 8 | CARGO_DOUBLE | Double type |
| 9 | VOID_SPACE | Void type |
| 10 | LOAD | Variable declaration |
| 11 | STORE | Assignment (=) |
| 12 | CARGO_ARRAY | Array modifier |
| 13 | COMBINE | Addition (+) |
| 14 | REDUCE | Subtraction (-) |
| 15 | AMPLIFY | Multiplication (*) |
| 16 | SPLIT | Division (/) |
| 17 | REMAINDER | Modulus (%) |
| 18 | BOOST | Increment (++) |
| 19 | DEGRADE | Decrement (--) |
| 20 | EXCEEDS | Greater than (>) |
| 21 | BELOW | Less than (<) |
| 22 | EXCEEDS_OR_EQUAL | >= |
| 23 | BELOW_OR_EQUAL | <= |
| 24 | MATCHES | Equal (==) |
| 25 | DIFFERS | Not equal (!=) |
| 26 | BOTH | Logical AND (&&) |
| 27 | EITHER | Logical OR (\|\|) |
| 28 | NEGATE | Logical NOT (!) |
| 29 | CHECK_IF | if statement |
| 30 | ELSE_CHECK | else if |
| 31 | OTHERWISE | else |
| 32 | PROTOCOL | switch |
| 33 | SCENARIO | case |
| 34 | DEFAULT_PROTOCOL | default |
| 35 | ORBIT_WHILE | while loop |
| 36 | SEQUENCE | for loop |
| 37 | REPEAT/UNTIL | do-while loop |
| 38 | TERMINATE | break |
| 39 | SKIP | continue |
| 40 | FUNCTION | Function declaration |
| 41 | RETURN_CARGO | return |
| 42 | RECEIVE | Input (scanf) |
| 43 | TRANSMIT | Output (printf) |
