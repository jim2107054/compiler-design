# SMPL Compiler - Manual Build Guide

## 🎯 Overview

This guide provides **step-by-step manual commands** to build the SMPL compiler without using automated build scripts.

---

## 📋 Prerequisites

Before building, ensure you have:

1. **Flex** (Lexical Analyzer Generator)
   - Windows: GnuWin32 Flex or MSYS2 Flex
   - Linux/Mac: `sudo apt install flex` or `brew install flex`

2. **Bison** (Parser Generator)  
   - Windows: MSYS2 Bison at `C:\msys64\usr\bin\bison.exe`
   - Linux/Mac: `sudo apt install bison` or `brew install bison`

3. **GCC** (C Compiler)
   - Windows: MSYS2 GCC at `C:\msys64\ucrt64\bin\gcc.exe`
   - Linux/Mac: Usually pre-installed

---

## 🔨 Manual Build Commands

### Step 1: Generate Lexer

**Windows (PowerShell)**:
```powershell
flex smpl_lexer.l
```

**Linux/Mac (Bash)**:
```bash
flex smpl_lexer.l
```

**Output**: `lex.yy.c` (lexer source code)

**What it does**: Converts lexer specification (`.l` file) into C code that tokenizes SMPL source files.

---

### Step 2: Generate Parser

**Windows (PowerShell)**:
```powershell
C:\msys64\usr\bin\bison.exe -d smpl_parser_new.y -o smpl_parser_new.tab.c
```

**Linux/Mac (Bash)**:
```bash
bison -d smpl_parser_new.y -o smpl_parser_new.tab.c
```

**Output**: 
- `smpl_parser_new.tab.c` (parser source code)
- `smpl_parser_new.tab.h` (parser header file)

**Expected Warnings** (safe to ignore):
- `warning: symbol 'array_size_opt' is used, but is not defined...`
- `warning: 3 shift/reduce conflicts`

**What it does**: Converts parser specification (`.y` file) into C code that builds an Abstract Syntax Tree (AST) from tokens.

---

### Step 3: Compile Core Modules

All modules must be compiled with `-Isrc` to find headers.

**Windows (PowerShell)**:
```powershell
# Navigate to project root
cd Y:\3-2\compiler\compiler-design

# Compile modules one by one
C:\msys64\ucrt64\bin\gcc.exe -c src/types.c -o src/types.o
C:\msys64\ucrt64\bin\gcc.exe -c src/ast.c -o src/ast.o -Isrc
C:\msys64\ucrt64\bin\gcc.exe -c src/symbol_table.c -o src/symbol_table.o -Isrc
C:\msys64\ucrt64\bin\gcc.exe -c src/semantic.c -o src/semantic.o -Isrc
C:\msys64\ucrt64\bin\gcc.exe -c src/ir.c -o src/ir.o -Isrc
C:\msys64\ucrt64\bin\gcc.exe -c src/codegen.c -o src/codegen.o -Isrc
C:\msys64\ucrt64\bin\gcc.exe -c src/main.c -o src/main.o -Isrc
```

**Linux/Mac (Bash)**:
```bash
# Navigate to project root
cd ~/path/to/compiler-design

# Compile modules
gcc -c src/types.c -o src/types.o
gcc -c src/ast.c -o src/ast.o -Isrc
gcc -c src/symbol_table.c -o src/symbol_table.o -Isrc
gcc -c src/semantic.c -o src/semantic.o -Isrc
gcc -c src/ir.c -o src/ir.o -Isrc
gcc -c src/codegen.c -o src/codegen.o -Isrc
gcc -c src/main.c -o src/main.o -Isrc
```

**Output**: `.o` object files in `src/` directory

**Expected Result**: All 7 modules compile successfully ✅

**What it does**: Compiles individual modules into object files that can be linked later.

---

### Step 4: Compile Lexer

**Windows (PowerShell)**:
```powershell
C:\msys64\ucrt64\bin\gcc.exe -c lex.yy.c -o lex.yy.o
```

**Linux/Mac (Bash)**:
```bash
gcc -c lex.yy.c -o lex.yy.o
```

**Output**: `lex.yy.o`

---

### Step 5: Compile Parser  

**Windows (PowerShell)**:
```powershell
C:\msys64\ucrt64\bin\gcc.exe -c smpl_parser_new.tab.c -o smpl_parser_new.tab.o -Isrc
```

**Linux/Mac (Bash)**:
```bash
gcc -c smpl_parser_new.tab.c -o smpl_parser_new.tab.o -Isrc
```

**Output**: `smpl_parser_new.tab.o`

**Note**: There may be some integration warnings - this is expected and being resolved.

---

### Step 6: Link Everything Together

**Windows (PowerShell)**:
```powershell
C:\msys64\ucrt64\bin\gcc.exe `
    smpl_parser_new.tab.o `
    lex.yy.o `
    src/types.o `
    src/ast.o `
    src/symbol_table.o `
    src/semantic.o `
    src/ir.o `
    src/codegen.o `
    src/main.o `
    -o smpl_compiler.exe
```

**Linux/Mac (Bash)**:
```bash
gcc \
    smpl_parser_new.tab.o \
    lex.yy.o \
    src/types.o \
    src/ast.o \
    src/symbol_table.o \
    src/semantic.o \
    src/ir.o \
    src/codegen.o \
    src/main.o \
    -o smpl_compiler
```

**Output**: `smpl_compiler.exe` (Windows) or `smpl_compiler` (Linux/Mac)

---

## ✅ Verification

### Test Individual Module Compilation

You can verify each module compiles correctly:

```powershell
# Windows - should all exit with code 0
C:\msys64\ucrt64\bin\gcc.exe -c src/types.c -o src/types.o; echo $LASTEXITCODE
C:\msys64\ucrt64\bin\gcc.exe -c src/ast.c -o src/ast.o -Isrc; echo $LASTEXITCODE
# ... etc
```

```bash  
# Linux/Mac - should all exit with code 0
gcc -c src/types.c -o src/types.o && echo "Success" || echo "Failed"
gcc -c src/ast.c -o src/ast.o -Isrc && echo "Success" || echo "Failed"
# ... etc
```

---

## 🎯 Current Status

### ✅ What Works 100%

1. **All 7 Core Modules** compile without errors:
   - `src/types.c` → Type system ✅
   - `src/ast.c` → Abstract Syntax Tree (650+ lines) ✅
   - `src/symbol_table.c` → Variable tracking ✅
   - `src/semantic.c` → Semantic analysis + interpreter ✅
   - `src/ir.c` → TAC generation ✅
   - `src/codegen.c` → C code generation ✅
   - `src/main.c` → Main orchestrator ✅

2. **Lexer** generates and compiles ✅
3. **Parser** generates (with expected warnings) ✅

### ⚠️ Integration Status

The parser is ~95% complete. A few grammar rules still need wrapper function updates for complete line number tracking. You can:

**Option A**: Use current parser for testing (may have minor line number reporting issues in errors)

**Option B**: Wait for complete integration fix (coming soon)

**Option C**: Help fix remaining rules by replacing direct `ast_*` calls with `wrap_*` equivalents

---

## 🧹 Clean Build (Start Fresh)

If you want to start over:

**Windows (PowerShell)**:
```powershell
# Remove generated files
Remove-Item lex.yy.c, lex.yy.o -ErrorAction SilentlyContinue
Remove-Item smpl_parser_new.tab.c, smpl_parser_new.tab.h, smpl_parser_new.tab.o -ErrorAction SilentlyContinue
Remove-Item src/*.o -ErrorAction SilentlyContinue
Remove-Item smpl_compiler.exe -ErrorAction SilentlyContinue
```

**Linux/Mac (Bash)**:
```bash
# Remove generated files
rm -f lex.yy.c lex.yy.o
rm -f smpl_parser_new.tab.c smpl_parser_new.tab.h smpl_parser_new.tab.o
rm -f src/*.o
rm -f smpl_compiler
```

---

## 📦 What Files Are Generated

| File | Type | Description |
|------|------|-------------|
| `lex.yy.c` | Generated | Lexer source code (from Flex) |
| `lex.yy.o` | Object | Compiled lexer |
| `smpl_parser_new.tab.c` | Generated | Parser source code (from Bison) |
| `smpl_parser_new.tab.h` | Generated | Parser header file |
| `smpl_parser_new.tab.o` | Object | Compiled parser |
| `src/*.o` | Object | Compiled modules (7 files) |
| `smpl_compiler.exe` | Executable | Final compiler (Windows) |
| `smpl_compiler` | Executable | Final compiler (Linux/Mac) |

**Git Note**: `.o` files and generated `lex.yy.c`/`smpl_parser_new.tab.*` are temporary - don't commit them!

---

## 🚀 Usage After Building

Once built successfully (when integration complete):

### 1. Interpret Mode (Run immediately)
```bash
./smpl_compiler --execute samples/sample1_basic.smpl
```

### 2. TAC Generation Mode
```bash
./smpl_compiler --tac-only samples/sample1_basic.smpl output.tac
```

### 3. C Code Generation Mode
```bash
./smpl_compiler samples/sample1_basic.smpl output.c
gcc output.c -o program
./program
```

---

## 🔍 Troubleshooting

### "bison: command not found"

**Fix**: Add Bison to PATH or use full path:
```powershell
# Windows - use full path
C:\msys64\usr\bin\bison.exe -d smpl_parser_new.y
```

### "flex: command not found"

**Fix**: Install Flex:
```bash
# Linux
sudo apt install flex

# Mac
brew install flex

# Windows
# Use MSYS2: pacman -S flex
```

### "undefined reference to ..." during linking

**Fix**: Make sure all object files are included in the link command:
```bash
# Must include ALL these files:
# - smpl_parser_new.tab.o
# - lex.yy.o
# - src/types.o
# - src/ast.o
# - src/symbol_table.o
# - src/semantic.o
# - src/ir.o
# - src/codegen.o
# - src/main.o
```

### Compilation warnings about "implicit declaration"

**Fix**: Make sure you use `-Isrc` flag when compiling:
```bash
gcc -c src/semantic.c -o src/semantic.o -Isrc
#                                        ^^^^^ Important!
```

---

## 📚 Related Documentation

- **[COMPLETE_GUIDE.md](COMPLETE_GUIDE.md)** - Full explanation of how the compiler works
- **[TESTING_GUIDE.md](TESTING_GUIDE.md)** - How to test the compiler
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Technical architecture details
- **[PROJECT_STATUS.md](PROJECT_STATUS.md)** - Current project status

---

## ✨ Summary

**Build Process**:
1. `flex smpl_lexer.l` → Generate lexer
2. `bison -d smpl_parser_new.y` → Generate parser
3. `gcc -c src/*.c` → Compile 7 modules  
4. `gcc -c lex.yy.c smpl_parser_new.tab.c` → Compile generated files
5. `gcc *.o -o smpl_compiler` → Link everything

**Total**: 5 steps to build from source!

**Current Achievement**: All core modules (3,500+ lines) compile successfully ✅
