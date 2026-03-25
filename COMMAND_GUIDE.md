# SMPL Compiler — Build & Command Guide

## Overview

This guide covers how to build and use the complete SMPL compiler (lexer + parser → C code).

**Author:** MD. Jahid Hasan Jim | Roll: 2107054 | KUET CSE 3212

---

## Prerequisites

| Tool  | Purpose                              | Download                              |
|-------|--------------------------------------|---------------------------------------|
| Flex  | Lexical analyser generator           | GnuWin32 or MSYS2                     |
| Bison | Parser generator                     | GnuWin32 or MSYS2                     |
| GCC   | C compiler (links everything)        | GnuWin32 / MinGW / MSYS2              |

### Verify installation

```powershell
flex --version
bison --version
gcc --version
```

---

## Directory Structure

```
compiler-design/
├── smpl_lexer.l              # Flex lexer (modified for Bison integration)
├── smpl_parser.y             # Bison parser (this is the main compiler logic)
├── samples/                  # Sample SMPL programs
│   ├── sample1_basic.smpl
│   ├── sample2_conditionals.smpl
│   ├── sample3_loops.smpl
│   ├── sample4_functions.smpl
│   └── sample5_complete.smpl
├── output/                   # Generated files (tokens, C code)
├── SMPL_PROPOSAL.md          # Language proposal / specification
├── COMMAND_GUIDE.md          # This file
└── LEXER_CODE.md             # Lexer documentation
```

---

## Build Steps — Integrated Compiler (Flex + Bison)

### Step 1 — Generate parser from Bison grammar

```powershell
bison -d smpl_parser.y
```

Produces:
- `smpl_parser.tab.c` — the parser C source
- `smpl_parser.tab.h` — token `#define`s (included by the lexer)

### Step 2 — Generate lexer from Flex specification

```powershell
flex smpl_lexer.l
```

Produces: `lex.yy.c`

### Step 3 — Compile everything together

```powershell
gcc smpl_parser.tab.c lex.yy.c -o smpl_compiler.exe -lfl
```

> **Note:** On some GnuWin32 setups `-lfl` may not be needed (use `-lfl` or remove it if it errors).

### One-liner (all three steps)

```powershell
bison -d smpl_parser.y; flex smpl_lexer.l; gcc smpl_parser.tab.c lex.yy.c -o smpl_compiler.exe -lfl
```

---

## Usage

### Compile to stdout

```powershell
.\smpl_compiler.exe samples\sample5_complete.smpl
```

### Compile to a named C file

```powershell
.\smpl_compiler.exe samples\sample5_complete.smpl output\sample5.c
```

### Run the generated C code

```powershell
# First compile SMPL → C
.\smpl_compiler.exe samples\sample5_complete.smpl output\sample5.c

# Then compile and run the C code
gcc output\sample5.c -o output\sample5.exe
.\output\sample5.exe
```

---

## Process All Samples

```powershell
mkdir output -ErrorAction SilentlyContinue

$samples = @("sample1_basic","sample2_conditionals","sample3_loops","sample4_functions","sample5_complete")
foreach ($s in $samples) {
    Write-Host "Compiling $s..."
    .\smpl_compiler.exe "samples\$s.smpl" "output\$s.c"
}
```

---

## SMPL → C Translation Reference

| SMPL                              | Generated C                             |
|-----------------------------------|-----------------------------------------|
| `MISSION`                         | `int main() {`                          |
| `LANDING;`                        | `    return 0; }`                       |
| `LAUNCH` / `ABORT`                | `{` / `}`                               |
| `LOAD CARGO_INT x STORE 5;`       | `int x = 5;`                            |
| `LOAD CARGO_INT CARGO_ARRAY a[3]` | `int a[3]`                              |
| `x STORE expr;`                   | `x = expr;`                             |
| `BOOST x;`                        | `x++;`                                  |
| `DEGRADE x;`                      | `x--;`                                  |
| `COMBINE a b`                     | `(a + b)` *(constant-folded if both int)* |
| `REDUCE a b`                      | `(a - b)`                               |
| `AMPLIFY a b`                     | `(a * b)`                               |
| `SPLIT a b`                       | `(a / b)`                               |
| `REMAINDER a b`                   | `(a % b)`                               |
| `a EXCEEDS b`                     | `(a > b)`                               |
| `a BELOW b`                       | `(a < b)`                               |
| `a MATCHES b`                     | `(a == b)`                              |
| `a DIFFERS b`                     | `(a != b)`                              |
| `a BOTH b`                        | `(a && b)`                              |
| `a EITHER b`                      | `(a \|\| b)`                            |
| `NEGATE expr`                     | `(!expr)`                               |
| `CHECK_IF (cond) LAUNCH … ABORT`  | `if (cond) { … }`                       |
| `ELSE_CHECK (cond) LAUNCH … ABORT`| `else if (cond) { … }`                  |
| `OTHERWISE LAUNCH … ABORT`        | `else { … }`                            |
| `PROTOCOL (x) LAUNCH … ABORT`     | `switch (x) { … }`                      |
| `SCENARIO 1:`                     | `case 1:`                               |
| `DEFAULT_PROTOCOL:`               | `default:`                              |
| `TERMINATE;`                      | `break;`                                |
| `SKIP;`                           | `continue;`                             |
| `ORBIT_WHILE (cond) block`        | `while (cond) { … }`                    |
| `SEQUENCE (init; cond; update)`   | `for (init; cond; update) { … }`        |
| `REPEAT block UNTIL (cond);`      | `do { … } while (cond);`                |
| `TRANSMIT "string";`              | `printf("string\n");`                   |
| `TRANSMIT x;`                     | `printf("%d\n", x);`                    |
| `RECEIVE x;`                      | `scanf("%d", &x);`                      |
| `FUNCTION CARGO_INT f(…) block`   | `int f(…) { … }`                        |
| `RETURN_CARGO expr;`              | `return expr;`                          |

---

## Troubleshooting

| Error | Solution |
|-------|----------|
| `bison: command not found` | Install Bison via MSYS2 (`pacman -S bison`) or GnuWin32 |
| `flex: command not found` | Install Flex via MSYS2 or GnuWin32 |
| `-lfl not found` | Try `-lfl` → `-lflexdll` on MSYS2, or remove the flag |
| `smpl_parser.tab.h: No such file` | Run `bison -d smpl_parser.y` first |
| `Syntax Error at line N: …` | Read the error — the SMPL source has a grammar mistake |
| `Lexical Error at line N: …` | Unrecognised character in the SMPL source |

---

## GnuWin32 Installation (Windows — if not using MSYS2)

```powershell
# Flex (already installed based on system check)
# Bison — download from:
#   http://gnuwin32.sourceforge.net/packages/bison.htm
# After installing, add to PATH:
$env:PATH += ";C:\Program Files (x86)\GnuWin32\bin"
```

Or use **MSYS2** (recommended):
```bash
pacman -S flex bison gcc
```
Then run all commands from the MSYS2 terminal pointing at your project directory.
