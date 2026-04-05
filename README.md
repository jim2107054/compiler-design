# 🚀 SMPL Compiler - Space Mission Programming Language

**A Creative, Space-Themed Programming Language Compiler with Professional Modular Architecture**

[![Version](https://img.shields.io/badge/version-2.0-blue.svg)](https://github.com)
[![Language](https://img.shields.io/badge/language-C-brightgreen.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/license-MIT-orange.svg)](LICENSE)

---

## 📖 Overview

**SMPL (Space Mission Programming Language)** is a domain-specific language featuring space-themed keywords that make programming feel like commanding a spacecraft. This compiler implements a complete professional architecture with:

✅ **Lexical & Syntax Analysis** (Flex + Bison)  
✅ **Abstract Syntax Tree (AST)** representation  
✅ **Symbol Table** with scope management  
✅ **Semantic Analysis** with type checking  
✅ **Runtime Interpreter** (Execute SMPL code directly!)  
✅ **Three-Address Code (TAC)** intermediate representation  
✅ **C Code Generation** (Transpile to C)  
✅ **Code Optimizations** (Constant folding, strength reduction)

---

## 🎯 Features

### **Language Features**

- ✨ **Creative Space-Themed Syntax**
  - `MISSION` ... `LANDING` (main program)
  - `LOAD CARGO` (variable declaration)
  - `COMBINE`, `REDUCE`, `AMPLIFY` (arithmetic)
  - `ORBIT_WHILE`, `SEQUENCE` (loops)
  - `CHECK_IF`, `PROTOCOL` (conditionals)
  
- 📊 **Data Types**: `int`, `float`, `char`, `double`, `string`
- 🔢 **Arrays**: `LOAD CARGO_ARRAY` with initialization
- 🔀 **Control Flow**: if-else, while, for, do-while, switch-case
- 🧮 **Operators**: Arithmetic, relational, logical
- 📝 **Functions**: User-defined with parameters/return values
- 💬 **I/O**: `RECEIVE` (input), `TRANSMIT` (output)

### **Compiler Features**

- 🏗️ **Modular Architecture** - 12 separate, maintainable modules
- ⚡ **Runtime Execution** - Interpret SMPL code without C compilation
- 🔍 **Type Checking** - Comprehensive semantic analysis
- 🚀 **Optimizations** - Constant folding, algebraic simplification
- 📄 **Multiple Output Formats** - C code, TAC, AST visualization
- 🐛 **Debug Tools** - AST printing, symbol table inspection

---

## 🛠️ Installation & Build

### **Prerequisites**

- **GCC** (GNU Compiler Collection)
- **Flex** (Lexical analyzer generator)
- **Bison** (Parser generator)

#### **Install on Linux/WSL**:
```bash
sudo apt update
sudo apt install gcc flex bison
```

#### **Install on macOS**:
```bash
brew install gcc flex bison
```

#### **Install on Windows**:
- Use Cygwin, MinGW, or WSL
- Or install GCC/Flex/Bison manually

### **Build the Compiler**

#### **Linux/Mac/WSL**:
```bash
chmod +x build.sh
./build.sh
```

#### **Windows (PowerShell)**:
```powershell
.\build.ps1
```

**Expected Output**:
```
╔═══════════════════════════════════════════════════════════════════════╗
║  Building SMPL Compiler (Modular Architecture v2.0)                  ║
╚═══════════════════════════════════════════════════════════════════════╝

[1/4] Generating lexer from smpl_lexer.l...
  ✓ Lexer generated (lex.yy.c)
[2/4] Generating parser from smpl_parser_new.y...
  ✓ Parser generated (smpl_parser_new.tab.c)
[3/4] Compiling all modules...
  ✓ All modules compiled
[4/4] Linking executable...
  ✓ Executable created: smpl_compiler

╔═══════════════════════════════════════════════════════════════════════╗
║  BUILD SUCCESSFUL                                                     ║
╚═══════════════════════════════════════════════════════════════════════╝
```

---

## 🚀 Usage

### **Command-Line Interface**

```bash
./smpl_compiler [options] <input_file> [output_file]
```

### **Options**

| Option | Description |
|--------|-------------|
| `--execute` / `-e` | Execute the program (runtime interpreter) |
| `--tac-only` / `-t` | Only generate TAC (no C code) |
| `--tac-file <file>` | Specify TAC output file (default: `output.tac`) |
| `--print-ast` | Print AST tree structure |
| `--print-symbols` | Print symbol table |
| `--verbose` / `-v` | Verbose output |
| `--help` / `-h` | Show help message |

### **Usage Examples**

#### **1. Direct Execution** (Fastest - No C compilation needed!)
```bash
./smpl_compiler -e samples/sample1_basic.smpl
```

**Output**:
```
--- Program Output ---
Sum: 15
Product: 50
Greater: 1

✓ Execution completed successfully
```

#### **2. Compile to C**
```bash
./smpl_compiler samples/sample1_basic.smpl output/program.c
```

Then compile the generated C code:
```bash
gcc output/program.c -o output/program
./output/program
```

#### **3. Generate TAC (Three-Address Code)**
```bash
./smpl_compiler -t samples/sample1_basic.smpl output/program.tac
```

**Generated TAC** (`output/program.tac`):
```
begin_program
t0 = 10
t1 = 5
t2 = t0 + t1
sum = t2
...
end_program
```

#### **4. Verbose Compilation with AST**
```bash
./smpl_compiler -v --print-ast samples/sample1_basic.smpl
```

**Output**:
```
╔═══════════════════════════════════════════════════════════════╗
║  SMPL COMPILER - v2.0 (Modular Architecture)                 ║
╚═══════════════════════════════════════════════════════════════╝

Phase 1: Parsing 'samples/sample1_basic.smpl'...
    ✓ Parsing completed successfully

╔═══════════════════════════════════════════════════════════════╗
║  ABSTRACT SYNTAX TREE (AST)                                   ║
╚═══════════════════════════════════════════════════════════════╝

PROGRAM
  STMT_LIST
    DECLARATION: sum (type: int)
    ASSIGNMENT: sum
      BINARY_OP: +
        INT: 10
        INT: 5
    ...
```

---

## 📝 SMPL Language Syntax

### **Hello World**
```smpl
MISSION
    TRANSMIT("Hello, Space Explorer!");
LANDING;
```

### **Variables**
```smpl
MISSION
    LOAD CARGO_INT x;
    LOAD CARGO_FLOAT pi <- 3.14;
    LOAD CARGO_CHAR grade <- 'A';
    
    x <- 42;
    TRANSMIT(x);
LANDING;
```

### **Arrays**
```smpl
MISSION
    LOAD CARGO_ARRAY CARGO_INT numbers[5] <- {1, 2, 3, 4, 5};
    TRANSMIT(numbers[2]);  // Output: 3
LANDING;
```

### **Control Flow**

#### **If-Else**
```smpl
CHECK_IF (x EXCEEDS 10) LAUNCH
    TRANSMIT("x is greater than 10");
ABORT OTHERWISE LAUNCH
    TRANSMIT("x is 10 or less");
ABORT
```

#### **While Loop**
```smpl
LOAD CARGO_INT i <- 0;
ORBIT_WHILE (i BELOW 5) LAUNCH
    TRANSMIT(i);
    BOOST i;  // i++
ABORT
```

#### **For Loop**
```smpl
SEQUENCE (LOAD CARGO_INT i <- 0; i BELOW 10; BOOST i) LAUNCH
    TRANSMIT(i);
ABORT
```

#### **Do-While**
```smpl
REPEAT LAUNCH
    TRANSMIT("Running...");
ABORT UNTIL (condition);
```

#### **Switch-Case**
```smpl
PROTOCOL (grade) LAUNCH
    SCENARIO 'A': TRANSMIT("Excellent!"); TERMINATE;
    SCENARIO 'B': TRANSMIT("Good!"); TERMINATE;
    DEFAULT_PROTOCOL: TRANSMIT("Try harder!");
ABORT
```

### **Functions**
```smpl
FUNCTION CARGO_INT add(CARGO_INT a, CARGO_INT b) LAUNCH
    RETURN_CARGO COMBINE a b;
ABORT

MISSION
    LOAD CARGO_INT result <- add(5, 3);
    TRANSMIT(result);  // Output: 8
LANDING;
```

### **Operators**

#### **Arithmetic** (Prefix notation)
```smpl
COMBINE x y      // x + y
REDUCE x y       // x - y
AMPLIFY x y      // x * y
SPLIT x y        // x / y
REMAINDER x y    // x % y
```

#### **Relational** (Infix notation)
```smpl
x EXCEEDS y           // x > y
x BELOW y             // x < y
x EXCEEDS_OR_EQUAL y  // x >= y
x BELOW_OR_EQUAL y    // x <= y
x MATCHES y           // x == y
x DIFFERS y           // x != y
```

#### **Logical**
```smpl
x BOTH y     // x && y
x EITHER y   // x || y
NEGATE x     // !x
```

---

## 📂 Project Structure

```
compiler-design/
│
├── src/                          # Modular compiler source
│   ├── types.h / types.c         # Type system
│   ├── ast.h / ast.c             # Abstract Syntax Tree
│   ├── symbol_table.h / .c       # Symbol table management
│   ├── semantic.h / .c           # Semantic analysis + interpreter
│   ├── ir.h / ir.c               # TAC generation
│   ├── codegen.h / .c            # C code generation
│   └── main.c                    # Entry point
│
├── samples/                      # Example SMPL programs
│   ├── sample1_basic.smpl
│   ├── sample2_conditionals.smpl
│   ├── sample3_loops.smpl
│   ├── sample4_functions.smpl
│   └── sample5_complete.smpl
│
├── output/                       # Generated files
│   ├── *.c                       # Generated C code
│   └── *.tac                     # Generated TAC
│
├── smpl_lexer.l                  # Flex lexical analyzer
├── smpl_parser_new.y             # Bison parser (modular)
├── build.sh                      # Linux/Mac build script
├── build.ps1                     # Windows PowerShell build script
│
├── README.md                     # This file
├── ARCHITECTURE.md               # Detailed architecture docs
├── SMPL_PROPOSAL.md              # Language specification
└── AST_IMPLEMENTATION.md         # AST design document
```

---

## 🧪 Testing

### **Run All Samples**

```bash
# Test 1: Basic operations
./smpl_compiler -e samples/sample1_basic.smpl

# Test 2: Conditionals
./smpl_compiler -e samples/sample2_conditionals.smpl

# Test 3: Loops
./smpl_compiler -e samples/sample3_loops.smpl

# Test 4: Functions
./smpl_compiler -e samples/sample4_functions.smpl

# Test 5: Complete program
./smpl_compiler -e samples/sample5_complete.smpl
```

### **Compile to C and Run**

```bash
./smpl_compiler samples/sample1_basic.smpl output/test.c
gcc output/test.c -o output/test
./output/test
```

---

## 🏗️ Architecture Overview

```
┌──────────────┐   ┌──────────────┐   ┌──────────────┐
│    Lexer     │ → │    Parser    │ → │     AST      │
│ (smpl_lexer) │   │(smpl_parser) │   │  (ast.c)     │
└──────────────┘   └──────────────┘   └──────┬───────┘
                                              │
                    ┌─────────────────────────┼─────────────┐
                    │                         │             │
                    ▼                         ▼             ▼
            ┌──────────────┐         ┌──────────────┐  ┌─────────┐
            │   Semantic   │         │      IR      │  │ Codegen │
            │  (semantic)  │         │   (ir.c)     │  │(codegen)│
            └──────┬───────┘         └──────┬───────┘  └────┬────┘
                   │                        │               │
                   ▼                        ▼               ▼
            ┌──────────────┐         ┌──────────┐    ┌──────────┐
            │  Interpreter │         │   TAC    │    │  C Code  │
            │   (Execute)  │         │   File   │    │   File   │
            └──────────────┘         └──────────┘    └──────────┘
```

**See [ARCHITECTURE.md](ARCHITECTURE.md) for detailed module documentation.**

---

## 🎓 Educational Value

This compiler is designed as a comprehensive learning resource for:

- **Compiler Design Courses** - Complete implementation of all phases
- **Programming Language Theory** - Custom syntax design
- **Software Architecture** - Modular design patterns
- **Algorithm Implementation** - AST traversal, symbol tables, etc.

---

## 🚀 Key Innovations

### **1. Runtime Interpreter**
Unlike traditional compilers that only generate output code, SMPL includes a **built-in interpreter** that executes programs directly from the AST. This enables:
- 🎯 Instant program testing
- 🐛 Easier debugging
- 📚 Educational demonstrations

### **2. Space-Themed Domain-Specific Language**
Makes learning compilers more engaging with creative, memorable syntax:
- `MISSION` instead of `main()`
- `ORBIT_WHILE` instead of `while`
- `TRANSMIT` instead of `printf`

### **3. Professional Modular Architecture**
Follows industry best practices:
- Separation of concerns
- Independent modules
- Clean interfaces
- Scalable design

---

## 📊 Comparison with Similar Projects

| Feature | SMPL Compiler | Friend's Bangla Compiler | Typical Student Compiler |
|---------|---------------|--------------------------|--------------------------|
| **Modular Architecture** | ✅ Yes | ✅ Yes | ❌ Usually monolithic |
| **Runtime Interpreter** | ✅ Yes | ✅ Yes | ❌ No |
| **TAC Generation** | ✅ Yes | ✅ Yes | ⚠️ Sometimes |
| **C Code Generation** | ✅ Yes | ❌ No | ⚠️ Sometimes |
| **Arrays** | ✅ Yes | ❌ No | ⚠️ Sometimes |
| **Switch-Case** | ✅ Yes | ❌ No | ⚠️ Sometimes |
| **Function Declarations** | ✅ Yes | ✅ Yes | ⚠️ Sometimes |
| **Optimizations** | ✅ Yes | ❌ No | ❌ Rare |
| **Creative Syntax** | ✅ Space theme | ✅ Bangla keywords | ❌ Usually C-like |

**SMPL combines the best of both worlds**: Friend's professional modular architecture + comprehensive language features.

---

## 🤝 Contributing

Contributions are welcome! Areas for improvement:

- 🔧 Additional optimizations (dead code elimination, loop unrolling)
- 📦 More built-in functions (string manipulation, math library)
- 🎨 Improved error messages with suggestions
- 🧪 Comprehensive test suite
- 📚 More example programs

---

## 📄 License

MIT License - See LICENSE file for details

---

## 👨‍💻 Author

**MD. Jahid Hasan Jim**  
🎓 Roll: 2107054  
📚 Course: CSE 3212 — Compiler Design Lab  
🏫 Institution: Khulna University of Engineering & Technology (KUET)  
📧 Email: [Your email]  
🔗 GitHub: [Your GitHub]

---

## 📚 Documentation

- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Detailed architecture and module documentation
- **[SMPL_PROPOSAL.md](SMPL_PROPOSAL.md)** - Complete language specification
- **[AST_IMPLEMENTATION.md](AST_IMPLEMENTATION.md)** - AST design and implementation
- **[COMMAND_GUIDE.md](COMMAND_GUIDE.md)** - Command-line reference

---

## 🙏 Acknowledgments

- Inspired by the **Bangla Programming Language Compiler** (friend's project) for modular architecture
- Based on compiler design principles from the **Dragon Book**
- Built with **Flex** and **Bison** (GNU tools)

---

## 🌟 Star This Project!

If you find this compiler useful for learning or teaching compiler design, please consider giving it a ⭐!

---

**Happy Space Missions! 🚀🛰️👨‍🚀**
