# SMPL Compiler - Modular Architecture v2.0

## 🚀 Overview

This document describes the professional modular architecture of the SMPL (Space Mission Programming Language) compiler. The compiler has been refactored from a monolithic design into a clean multi-module system with clear separation of concerns.

---

## 📐 Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         SMPL SOURCE CODE                        │
│                        (*.smpl files)                           │
└────────────────────────┬────────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────────┐
│  PHASE 1: LEXICAL ANALYSIS                                      │
│  ┌──────────────────┐                                           │
│  │  smpl_lexer.l    │  →  Flex  →  lex.yy.c                    │
│  │  (Flex spec)     │         (Token stream)                    │
│  └──────────────────┘                                           │
└────────────────────────┬────────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────────┐
│  PHASE 2: SYNTAX ANALYSIS                                       │
│  ┌────────────────────┐                                         │
│  │ smpl_parser_new.y  │  →  Bison  →  AST Builder              │
│  │ (Bison grammar)    │         (Abstract Syntax Tree)          │
│  └────────────────────┘                                         │
│         Uses: ast.h, types.h                                    │
└────────────────────────┬────────────────────────────────────────┘
                         │
                         ▼
                    ┌────────┐
                    │   AST  │  (In-memory tree representation)
                    └────┬───┘
                         │
        ┌────────────────┼────────────────┐
        │                │                │
        ▼                ▼                ▼
┌──────────────┐  ┌──────────────┐  ┌──────────────┐
│ PHASE 3:     │  │ MODE 1:      │  │ OTHER MODES  │
│ SEMANTIC     │  │ INTERPRETER  │  │ (Future)     │
│ ANALYSIS     │  │              │  │              │
│              │  │ Runtime      │  │              │
│ semantic.h/c │  │ Execution    │  │              │
│              │  │              │  │              │
│ • Type check │  │ semantic.c   │  │              │
│ • Scope      │  │ (execute)    │  │              │
│ • Validation │  │              │  │              │
└──────┬───────┘  └──────────────┘  └──────────────┘
       │
       ▼
┌─────────────────────────────────────────────────────────────────┐
│  PHASE 4: INTERMEDIATE REPRESENTATION (IR)                      │
│  ┌──────────────┐                                               │
│  │  ir.h / ir.c │  →  Three-Address Code (TAC)                 │
│  └──────────────┘      • Temp variables (t0, t1, ...)           │
│                        • Labels (L0, L1, ...)                   │
│                        • Simple instructions                    │
│                        • Optimizations (constant fold, etc.)    │
└────────────────────────┬────────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────────┐
│  PHASE 5: CODE GENERATION                                       │
│  ┌────────────────────────┐                                     │
│  │ codegen.h / codegen.c  │  →  C Source Code                  │
│  └────────────────────────┘      • Type mapping                 │
│                                  • Operator conversion           │
│                                  • Format specifiers            │
│                                  • Full program structure       │
└────────────────────────┬────────────────────────────────────────┘
                         │
                         ▼
                   ┌──────────┐
                   │ output.c │  (Compiled C program)
                   └──────────┘
                         │
                         ▼  (gcc)
                   ┌──────────┐
                   │   a.out  │  (Executable)
                   └──────────┘
```

---

## 🏗️ Module Structure

### **Core Modules** (in `src/` directory)

| Module | Files | Responsibility | Dependencies |
|--------|-------|----------------|--------------|
| **Types** | `types.h` / `types.c` | Type system definitions and utilities | None |
| **AST** | `ast.h` / `ast.c` | Abstract Syntax Tree data structures | `types.h` |
| **Symbol Table** | `symbol_table.h` / `symbol_table.c` | Variable/function tracking with scopes | `types.h` |
| **Semantic** | `semantic.h` / `semantic.c` | Semantic analysis + runtime interpreter | `ast.h`, `symbol_table.h` |
| **IR** | `ir.h` / `ir.c` | Three-Address Code generation | `ast.h`, `symbol_table.h` |
| **Code Gen** | `codegen.h` / `codegen.c` | C code generation from AST | `ast.h`, `symbol_table.h`, `ir.h` |
| **Main** | `main.c` | Entry point, orchestration | All modules |

### **Front-End Modules**

| Module | File | Purpose |
|--------|------|---------|
| **Lexer** | `smpl_lexer.l` | Tokenization (Flex specification) |
| **Parser** | `smpl_parser_new.y` | Syntax analysis & AST building (Bison grammar) |

---

## 📋 Module Details

### 1. **types.h / types.c**

**Purpose**: Central type system for the compiler

**Key Types**:
```c
typedef enum {
    SMPL_INT, SMPL_FLOAT, SMPL_CHAR, 
    SMPL_DOUBLE, SMPL_VOID, SMPL_STRING
} SMPLType;

typedef enum {
    TYPE_INT, TYPE_FLOAT, TYPE_CHAR,
    TYPE_DOUBLE, TYPE_VOID, TYPE_STRING
} ValueType;
```

**Key Functions**:
- `smpl_type_to_value_type()` - Convert SMPL type to value type
- `value_type_to_c()` - Map value type to C type string
- `types_compatible()` - Check type compatibility

---

### 2. **ast.h / ast.c**

**Purpose**: Abstract Syntax Tree representation

**Key Structures**:
- `ASTNode` - Generic AST node with union of all node types
- 30+ specialized node types (binary ops, loops, function calls, etc.)

**Key Functions**:
- **Constructors**: `ast_int_literal()`, `ast_binary_op()`, `ast_if_stmt()`, `ast_while_loop()`, etc.
- **Utilities**: `ast_print()`, `ast_free()`

**Example**:
```c
// Build AST: x = 5 + 3
ASTNode *five = ast_int_literal(5);
ASTNode *three = ast_int_literal(3);
ASTNode *add = ast_binary_op("+", five, three);
ASTNode *assign = ast_assignment("x", add);
```

---

### 3. **symbol_table.h / symbol_table.c**

**Purpose**: Track variables and functions with scope information

**Key Structure**:
```c
typedef struct {
    char name[256];
    char type[32];
    int scope;
    int is_function;
    int is_array;
} Symbol;

typedef struct {
    Symbol *symbols;
    int count;
    int capacity;
    int current_scope;
} SymbolTable;
```

**Key Functions**:
- `symbol_table_init()` / `symbol_table_free()`
- `symbol_table_insert()` - Add symbol
- `symbol_table_lookup()` - Find symbol (respects scope)
- `enter_scope()` / `exit_scope()` - Scope management

---

### 4. **semantic.h / semantic.c**

**Purpose**: Semantic analysis AND runtime interpretation

**Key Features**:
- Type checking
- Scope validation
- **Runtime interpreter** (execute SMPL programs directly!)

**Key Functions**:
- `semantic_analyze()` - Full semantic analysis pass
- `semantic_execute()` - Runtime program execution

**Example Usage**:
```c
AnalysisResult result = semantic_analyze(ast);
if (result.success) {
    semantic_execute(ast);  // Run the program!
}
```

**Runtime Features**:
- Variable storage
- Expression evaluation
- Control flow (if/else, loops, switch)
- Function calls
- I/O operations

---

### 5. **ir.h / ir.c**

**Purpose**: Three-Address Code (TAC) intermediate representation

**TAC Format**:
```
t0 = 5
t1 = 3
t2 = t0 + t1
x = t2
```

**Key Structures**:
```c
typedef struct TACInstruction {
    char *result;
    char *arg1;
    char *arg2;
    char *op;
    struct TACInstruction *next;
} TACInstruction;
```

**Key Functions**:
- `tac_init()` / `tac_free()`
- `tac_generate()` - Generate TAC from AST
- `tac_write_to_file()` - Output TAC to file

**Optimizations** (stub):
- Constant folding
- Strength reduction
- Dead code elimination

---

### 6. **codegen.h / codegen.c**

**Purpose**: Generate executable C code from AST

**Key Features**:
- SMPL → C operator mapping
- Type-aware format specifiers
- Indentation management
- Full program scaffolding

**Mapping Examples**:
```
SMPL              →  C
─────────────────────────────
COMBINE x y       →  (x + y)
AMPLIFY x y       →  (x * y)
EXCEEDS x y       →  (x > y)
BOTH x y          →  (x && y)
ORBIT_WHILE cond  →  while (cond)
```

**Key Functions**:
- `codegen_generate()` - Full program generation
- `codegen_expr()` - Expression code
- `codegen_stmt()` - Statement code

---

### 7. **main.c**

**Purpose**: Orchestrate all compilation/execution phases

**Command-Line Modes**:

1. **Compilation Mode** (default):
   ```bash
   ./smpl_compiler program.smpl program.c
   ```
   Generates C code

2. **Execution Mode**:
   ```bash
   ./smpl_compiler --execute program.smpl
   ```
   Runs the program directly (interpreter)

3. **TAC-Only Mode**:
   ```bash
   ./smpl_compiler --tac-only program.smpl program.tac
   ```
   Only generates TAC

**Flags**:
- `--execute, -e` : Interpret and run
- `--tac-only, -t` : Generate TAC only
- `--print-ast` : Display AST tree
- `--print-symbols` : Show symbol table
- `--verbose, -v` : Detailed output
- `--help, -h` : Show usage

---

## 🔄 Compilation Pipeline

### **Standard Compilation Flow**:

```bash
program.smpl → [Lexer] → Tokens → [Parser] → AST
                                               ↓
                                     [Semantic Analysis]
                                               ↓
                                         [TAC Gen] → program.tac
                                               ↓
                                        [Code Gen] → program.c
                                               ↓
                                            [GCC] → executable
```

### **Execution Flow** (Interpreter Mode):

```bash
program.smpl → [Lexer] → Tokens → [Parser] → AST
                                               ↓
                                     [Semantic Analysis]
                                               ↓
                                         [Execute] → Output
```

---

## 🛠️ Build Process

### **Commands**:

**Linux/Mac/WSL**:
```bash
chmod +x build.sh
./build.sh
```

**Windows (PowerShell)**:
```powershell
.\build.ps1
```

### **Build Steps**:

1. **Generate Lexer**: `flex smpl_lexer.l` → `lex.yy.c`
2. **Generate Parser**: `bison -d smpl_parser_new.y` → `smpl_parser_new.tab.c/h`
3. **Compile Modules**: `gcc -c src/*.c`
4. **Link**: `gcc *.o -o smpl_compiler`

---

## 📊 Comparison: Old vs New Architecture

| Aspect | Old (Monolithic) | New (Modular) |
|--------|------------------|---------------|
| **Structure** | Single 2300+ line file | 12 separate modules (~3500 lines) |
| **Separation** | All phases mixed | Clean phase separation |
| **Testability** | Difficult | Each module testable independently |
| **Maintainability** | Low | High |
| **Extensibility** | Hard to extend | Easy to add features |
| **Runtime Execution** | ❌ Not supported | ✅ Full interpreter included |
| **Compilation Time** | Fast (single file) | Modular (rebuild only changed) |
| **Code Reuse** | Limited | High |

---

## 🎯 Key Improvements

### **1. Professional Structure**
- Industry-standard compiler architecture
- Follows GCC/Clang organization principles
- Clean interfaces between modules

### **2. Runtime Interpretation**
- **NEW**: Execute SMPL programs without C compilation
- Instant feedback for testing
- Educational value (see program behavior immediately)

### **3. Scalability**
- Easy to add new features (e.g., new operators, data structures)
- Module can be replaced independently
- Support for future optimizations

### **4. Debugging**
- AST visualization (`--print-ast`)
- Symbol table inspection (`--print-symbols`)
- Verbose mode for all phases

---

## 📚 Example Usage

### **1. Direct Execution** (Fastest for testing)
```bash
./smpl_compiler -e samples/sample1_basic.smpl
```

### **2. Compile to C**
```bash
./smpl_compiler samples/sample1_basic.smpl output/program.c
gcc output/program.c -o output/program
./output/program
```

### **3. Generate TAC** (For IR analysis)
```bash
./smpl_compiler -t samples/sample1_basic.smpl output/program.tac
```

### **4. Verbose Compilation** (See all phases)
```bash
./smpl_compiler -v --print-ast samples/sample1_basic.smpl
```

---

## 🧩 Extending the Compiler

### **Adding a New Feature**:

**Example**: Add a new operator (e.g., power: `POWER x y` for x^y)

1. **Lexer** (`smpl_lexer.l`):
   ```c
   "POWER"  { return TOK_POWER; }
   ```

2. **Parser** (`smpl_parser_new.y`):
   ```c
   %token TOK_POWER
   
   binary_expr: TOK_POWER expr expr
       { $$ = ast_binary_op("**", $2, $3); }
   ```

3. **Semantic** (`semantic.c`):
   ```c
   else if (strcmp(op, "**") == 0) {
       return pow(left_val, right_val);
   }
   ```

4. **Code Gen** (`codegen.c`):
   ```c
   if (strcmp(op, "**") == 0) {
       fprintf(ctx->output, "pow(");
       codegen_expr(node->data.binary_op.left, ctx);
       fprintf(ctx->output, ", ");
       codegen_expr(node->data.binary_op.right, ctx);
       fprintf(ctx->output, ")");
   }
   ```

5. **Rebuild**: `./build.sh`

---

## 📖 Further Reading

- **AST Implementation**: See `AST_IMPLEMENTATION.md`
- **Language Proposal**: See `SMPL_PROPOSAL.md`
- **Command Reference**: See `COMMAND_GUIDE.md`

---

## 👨‍💻 Author

**MD. Jahid Hasan Jim**  
Roll: 2107054  
Course: CSE 3212 — Compiler Design Lab  
Institution: Khulna University of Engineering & Technology (KUET)

---

**Happy Compiling! 🚀**
