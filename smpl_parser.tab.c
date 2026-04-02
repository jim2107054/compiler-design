/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "smpl_parser.y"

/*
 * ============================================================
 *  SMPL Parser — Space Mission Programming Language
 *  Bison specification file: syntax analysis + C code generation
 * ============================================================
 *  Author : MD. Jahid Hasan Jim
 *  Roll   : 2107054
 *  Course : CSE 3212 — Compiler Design Lab
 *  Inst   : Khulna University of Engineering & Technology (KUET)
 *  Date   : 2026
 * ============================================================
 *
 *  Features implemented:
 *    1) Lexical analysis (Flex) + Syntax analysis (Bison)
 *    2) Symbol table with type tracking
 *    3) Type checking  &  implicit type conversion warnings
 *    4) Variable-declaration / redeclaration checking
 *    5) Three-Address Code (TAC) intermediate representation
 *    6) Code optimisation:
 *         - Constant folding   (e.g. COMBINE 3 5 → 8)
 *         - Strength reduction (e.g. AMPLIFY x 8 → x << 3)
 *         - Algebraic identity (e.g. COMBINE x 0 → x)
 *    7) C code generation with correct printf/scanf format specifiers
 *
 *  Build:
 *    bison -d smpl_parser.y
 *    flex  smpl_lexer.l
 *    gcc smpl_parser.tab.c lex.yy.c -o smpl_compiler -lfl
 *
 *  Run:
 *    ./smpl_compiler <input.smpl> [output.c]
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* ── Lexer interface ────────────────────────────────────────────────────── */
extern int   yylex(void);
extern int   yylineno;
extern char *yytext;
extern FILE *yyin;
extern FILE *token_output;

/* ── Output / indentation ───────────────────────────────────────────────── */
FILE *output        = NULL;
int   indent_level  = 0;

static void print_indent(void) {
    int i;
    for (i = 0; i < indent_level; i++)
        fprintf(output, "    ");
}

/* ── printf-style heap-string builder ──────────────────────────────────── */
static char *mkstr(const char *fmt, ...) {
    char   buf[8192];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    return strdup(buf);
}

static char *safe_strdup(const char *s) {
    return s ? strdup(s) : strdup("");
}

/* ── Windows-portable strndup ───────────────────────────────────────────── */
#ifndef _GNU_SOURCE
static char *smpl_strndup(const char *s, size_t n) {
    size_t len = strlen(s);
    if (n < len) len = n;
    char *p = malloc(len + 1);
    if (p) { memcpy(p, s, len); p[len] = '\0'; }
    return p;
}
#define strndup smpl_strndup
#endif

/* ════════════════════════════════════════════════════════════════════════════
 * SYMBOL TABLE  —  tracks declared variables with their types
 * ════════════════════════════════════════════════════════════════════════════ */

typedef struct {
    char name[256];
    char type[32];          /* "int","float","char","double","void" */
    int  scope;
    int  is_function;
    int  is_array;
} Symbol;

#define MAX_SYMBOLS 1000
static Symbol sym_table[MAX_SYMBOLS];
static int    sym_count     = 0;
static int    current_scope = 0;

/* Add a symbol; warn on redeclaration */
static void add_symbol(const char *name, const char *type,
                        int is_func, int is_arr) {
    int i;
    for (i = sym_count - 1; i >= 0; i--) {
        if (sym_table[i].scope == current_scope &&
            strcmp(sym_table[i].name, name) == 0) {
            if (strcmp(sym_table[i].type, type) != 0) {
                fprintf(stderr,
                    "Warning at line %d: Redeclaration of '%s' "
                    "with different type (was %s, now %s)\n",
                    yylineno, name, sym_table[i].type, type);
            }
            /* silently accept same-type redecl (e.g. loop vars) */
            strncpy(sym_table[i].type, type, 31);
            sym_table[i].type[31] = '\0';
            return;
        }
    }
    if (sym_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: Symbol table overflow\n");
        return;
    }
    strncpy(sym_table[sym_count].name, name, 255);
    sym_table[sym_count].name[255] = '\0';
    strncpy(sym_table[sym_count].type, type, 31);
    sym_table[sym_count].type[31] = '\0';
    sym_table[sym_count].scope       = current_scope;
    sym_table[sym_count].is_function = is_func;
    sym_table[sym_count].is_array    = is_arr;
    sym_count++;
}

/* Look up the type of a name (most-recent first) */
static const char *lookup_type(const char *name) {
    int i;
    for (i = sym_count - 1; i >= 0; i--)
        if (strcmp(sym_table[i].name, name) == 0)
            return sym_table[i].type;
    return NULL;
}

static void enter_scope(void) { current_scope++; }
static void leave_scope(void) {
    while (sym_count > 0 &&
           sym_table[sym_count - 1].scope == current_scope)
        sym_count--;
    current_scope--;
}

/* ════════════════════════════════════════════════════════════════════════════
 * THREE-ADDRESS CODE (TAC) — intermediate representation
 * ════════════════════════════════════════════════════════════════════════════ */

static FILE *tac_output  = NULL;
static int   temp_count  = 0;
static int   label_count = 0;

static char *new_temp(void)  { return mkstr("t%d", temp_count++);  }
static char *new_label(void) { return mkstr("L%d", label_count++); }

static void tac_emit(const char *fmt, ...) {
    if (!tac_output) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(tac_output, fmt, ap);
    va_end(ap);
    fprintf(tac_output, "\n");
}

/* Label stack (for control-flow labelling in TAC) */
#define MAX_LABEL_STACK 256
static char *tac_label_stack[MAX_LABEL_STACK];
static int   tac_label_sp = -1;

static void tac_push(char *l)   { tac_label_stack[++tac_label_sp] = l; }
static char *tac_pop(void)      { return tac_label_stack[tac_label_sp--]; }

/* ════════════════════════════════════════════════════════════════════════════
 * TYPE-SYSTEM HELPERS
 * ════════════════════════════════════════════════════════════════════════════ */

/* Return the wider of two numeric types (C promotion rules) */
static const char *wider_type(const char *t1, const char *t2) {
    if (!t1 || !t2) return t1 ? t1 : (t2 ? t2 : "int");
    if (strcmp(t1, "double") == 0 || strcmp(t2, "double") == 0) return "double";
    if (strcmp(t1, "float")  == 0 || strcmp(t2, "float")  == 0) return "float";
    return "int";   /* int+int, char+char, char+int → int */
}

/* printf format specifier for a type */
static const char *format_for_type(const char *type) {
    if (!type) return "%d";
    if (strcmp(type, "float")  == 0) return "%f";
    if (strcmp(type, "double") == 0) return "%f";
    if (strcmp(type, "char")   == 0) return "%c";
    return "%d";
}

/* scanf format specifier for a type */
static const char *scanf_format_for_type(const char *type) {
    if (!type) return "%d";
    if (strcmp(type, "float")  == 0) return "%f";
    if (strcmp(type, "double") == 0) return "%lf";
    if (strcmp(type, "char")   == 0) return " %c";
    return "%d";
}

/* Check type compatibility; emit a note / warning when necessary */
static void check_type_compat(const char *var_name,
                               const char *var_type,
                               const char *expr_type) {
    if (!var_type || !expr_type) return;
    if (strcmp(var_type, expr_type) == 0) return;

    /* widening (OK, but note it) */
    if ((strcmp(var_type, "float") == 0 || strcmp(var_type, "double") == 0) &&
         strcmp(expr_type, "int") == 0) {
        fprintf(stderr,
            "Note at line %d: Implicit conversion int -> %s for '%s'\n",
            yylineno, var_type, var_name);
        return;
    }
    if (strcmp(var_type, "double") == 0 &&
        strcmp(expr_type, "float") == 0) {
        fprintf(stderr,
            "Note at line %d: Implicit conversion float -> double for '%s'\n",
            yylineno, var_name);
        return;
    }
    /* narrowing */
    if (strcmp(var_type, "int") == 0 &&
        (strcmp(expr_type, "float") == 0 ||
         strcmp(expr_type, "double") == 0)) {
        fprintf(stderr,
            "Warning at line %d: Narrowing conversion %s -> int for '%s'\n",
            yylineno, expr_type, var_name);
        return;
    }
    /* anything else */
    fprintf(stderr,
        "Warning at line %d: Type mismatch assigning %s to %s variable '%s'\n",
        yylineno, expr_type, var_type, var_name);
}

/* ════════════════════════════════════════════════════════════════════════════
 * CODE OPTIMISATION HELPERS
 * ════════════════════════════════════════════════════════════════════════════ */

/* (1) Constant folding — evaluate integer-literal arithmetic at compile time */
static int try_fold_int(const char *a, const char *b,
                        char op, char **result) {
    char *ea, *eb;
    long  va = strtol(a, &ea, 10);
    long  vb = strtol(b, &eb, 10);
    if (*ea != '\0' || *eb != '\0') return 0;
    long  res;
    switch (op) {
        case '+': res = va + vb; break;
        case '-': res = va - vb; break;
        case '*': res = va * vb; break;
        case '/': if (vb == 0) return 0; res = va / vb; break;
        case '%': if (vb == 0) return 0; res = va % vb; break;
        default:  return 0;
    }
    *result = mkstr("%ld", res);
    fprintf(stderr, "  [Optimize] Constant fold: %s %c %s => %ld\n",
            a, op, b, res);
    return 1;
}

/* (2) Strength reduction — replace multiply / divide by power-of-2
 *     with left / right shifts (integer operands only)                      */
static int try_strength_reduce(const char *a, const char *b,
                               const char *type_a, const char *type_b,
                               char op, char **result) {
    long val; char *end; int shift; long tmp;

    if (op != '*' && op != '/') return 0;
    /* only apply to integer types */
    if (type_a && strcmp(type_a, "int") != 0 && strcmp(type_a, "char") != 0)
        return 0;
    if (type_b && strcmp(type_b, "int") != 0 && strcmp(type_b, "char") != 0)
        return 0;

    /* check b */
    val = strtol(b, &end, 10);
    if (*end == '\0' && val > 0 && (val & (val - 1)) == 0) {
        shift = 0; tmp = val;
        while (tmp > 1) { tmp >>= 1; shift++; }
        if (op == '*')
            *result = mkstr("(%s << %d)", a, shift);
        else
            *result = mkstr("(%s >> %d)", a, shift);
        fprintf(stderr, "  [Optimize] Strength reduce: %s %c %ld => %s\n",
                a, op, val, *result);
        return 1;
    }
    /* for multiply, also check a (commutative) */
    if (op == '*') {
        val = strtol(a, &end, 10);
        if (*end == '\0' && val > 0 && (val & (val - 1)) == 0) {
            shift = 0; tmp = val;
            while (tmp > 1) { tmp >>= 1; shift++; }
            *result = mkstr("(%s << %d)", b, shift);
            fprintf(stderr,
                "  [Optimize] Strength reduce: %ld * %s => %s\n",
                val, b, *result);
            return 1;
        }
    }
    return 0;
}

/* (3) Algebraic identity simplification
 *     x+0→x, 0+x→x, x-0→x, x*1→x, 1*x→x, x/1→x, x*0→0, 0*x→0        */
static int try_algebraic_simplify(const char *a, const char *b,
                                   char op, char **result) {
    char *ea, *eb;
    long va = strtol(a, &ea, 10);
    long vb = strtol(b, &eb, 10);

    /* x + 0 → x , x - 0 → x */
    if ((op == '+' || op == '-') && *eb == '\0' && vb == 0) {
        *result = strdup(a);
        fprintf(stderr, "  [Optimize] Algebraic: %s %c 0 => %s\n", a, op, a);
        return 1;
    }
    /* 0 + x → x */
    if (op == '+' && *ea == '\0' && va == 0) {
        *result = strdup(b);
        fprintf(stderr, "  [Optimize] Algebraic: 0 + %s => %s\n", b, b);
        return 1;
    }
    /* x * 1 → x , x / 1 → x */
    if ((op == '*' || op == '/') && *eb == '\0' && vb == 1) {
        *result = strdup(a);
        fprintf(stderr, "  [Optimize] Algebraic: %s %c 1 => %s\n", a, op, a);
        return 1;
    }
    /* 1 * x → x */
    if (op == '*' && *ea == '\0' && va == 1) {
        *result = strdup(b);
        fprintf(stderr, "  [Optimize] Algebraic: 1 * %s => %s\n", b, b);
        return 1;
    }
    /* x * 0 → 0 , 0 * x → 0 */
    if (op == '*') {
        if ((*ea == '\0' && va == 0) || (*eb == '\0' && vb == 0)) {
            *result = strdup("0");
            fprintf(stderr, "  [Optimize] Algebraic: %s * %s => 0\n", a, b);
            return 1;
        }
    }
    return 0;
}

/* ── Error handler ──────────────────────────────────────────────────────── */
void yyerror(const char *msg) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yylineno, msg);
}

/* ════════════════════════════════════════════════════════════════════════════
 * ABSTRACT SYNTAX TREE (AST) — Tree-based intermediate representation
 * ════════════════════════════════════════════════════════════════════════════ */

/* AST node types */
typedef enum {
    /* Expressions */
    AST_INTEGER,
    AST_FLOAT,
    AST_CHAR,
    AST_STRING,
    AST_IDENTIFIER,
    AST_BINARY_OP,
    AST_UNARY_OP,
    AST_FUNCTION_CALL,
    AST_ARRAY_ACCESS,
    
    /* Statements */
    AST_DECLARATION,
    AST_ASSIGNMENT,
    AST_ARRAY_ASSIGN,
    AST_IF_STMT,
    AST_WHILE_LOOP,
    AST_FOR_LOOP,
    AST_DO_WHILE,
    AST_SWITCH_STMT,
    AST_CASE_STMT,
    AST_BREAK,
    AST_CONTINUE,
    AST_RETURN,
    AST_BLOCK,
    AST_EXPR_STMT,
    AST_INPUT,
    AST_OUTPUT,
    
    /* Program structure */
    AST_PROGRAM,
    AST_FUNCTION_DEF,
    AST_PARAM,
    AST_STMT_LIST
} ASTNodeType;

/* Forward declaration */
struct ASTNode;

/* AST Node structure */
typedef struct ASTNode {
    ASTNodeType type;
    int line_number;
    
    /* Data specific to node type */
    union {
        /* Literals */
        struct { int value; } int_literal;
        struct { float value; } float_literal;
        struct { char *value; } char_literal;
        struct { char *value; } string_literal;
        struct { char *name; } identifier;
        
        /* Binary operations */
        struct {
            char *op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_op;
        
        /* Unary operations */
        struct {
            char *op;
            struct ASTNode *operand;
        } unary_op;
        
        /* Function call */
        struct {
            char *func_name;
            struct ASTNode *args;  /* list of arguments */
        } func_call;
        
        /* Array access */
        struct {
            char *array_name;
            struct ASTNode *index;
        } array_access;
        
        /* Declaration */
        struct {
            char *var_type;
            char *var_name;
            int is_array;
            struct ASTNode *array_size;
            struct ASTNode *init_value;
        } declaration;
        
        /* Assignment */
        struct {
            char *var_name;
            struct ASTNode *value;
        } assignment;
        
        /* Array assignment */
        struct {
            char *array_name;
            struct ASTNode *index;
            struct ASTNode *value;
        } array_assign;
        
        /* If statement */
        struct {
            struct ASTNode *condition;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch;
        } if_stmt;
        
        /* While loop */
        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_loop;
        
        /* For loop */
        struct {
            struct ASTNode *init;
            struct ASTNode *condition;
            struct ASTNode *update;
            struct ASTNode *body;
        } for_loop;
        
        /* Do-while loop */
        struct {
            struct ASTNode *body;
            struct ASTNode *condition;
        } do_while;
        
        /* Switch statement */
        struct {
            struct ASTNode *expr;
            struct ASTNode *cases;
        } switch_stmt;
        
        /* Case statement */
        struct {
            struct ASTNode *value;
            struct ASTNode *stmts;
            struct ASTNode *next_case;
            int is_default;
        } case_stmt;
        
        /* Return statement */
        struct {
            struct ASTNode *value;
        } return_stmt;
        
        /* Block */
        struct {
            struct ASTNode *statements;
        } block;
        
        /* I/O */
        struct {
            char *var_name;
        } input;
        
        struct {
            struct ASTNode *expr;
        } output;
        
        /* Function definition */
        struct {
            char *return_type;
            char *func_name;
            struct ASTNode *params;
            struct ASTNode *body;
        } func_def;
        
        /* Parameter */
        struct {
            char *param_type;
            char *param_name;
            struct ASTNode *next;
        } param;
        
        /* Program */
        struct {
            struct ASTNode *functions;
        } program;
        
        /* Statement list */
        struct {
            struct ASTNode *stmt;
            struct ASTNode *next;
        } stmt_list;
    } data;
    
    /* Type information for expressions */
    char *expr_type;
} ASTNode;

/* AST constructor functions */
static ASTNode *new_ast_node(ASTNodeType type) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for AST node\n");
        exit(1);
    }
    memset(node, 0, sizeof(ASTNode));
    node->type = type;
    node->line_number = yylineno;
    node->expr_type = NULL;
    return node;
}

static ASTNode *ast_int_literal(int value) {
    ASTNode *node = new_ast_node(AST_INTEGER);
    node->data.int_literal.value = value;
    node->expr_type = strdup("int");
    return node;
}

static ASTNode *ast_float_literal(float value) {
    ASTNode *node = new_ast_node(AST_FLOAT);
    node->data.float_literal.value = value;
    node->expr_type = strdup("float");
    return node;
}

static ASTNode *ast_string_literal(const char *value) {
    ASTNode *node = new_ast_node(AST_STRING);
    node->data.string_literal.value = strdup(value);
    node->expr_type = strdup("string");
    return node;
}

static ASTNode *ast_char_literal(const char *value) {
    ASTNode *node = new_ast_node(AST_CHAR);
    node->data.char_literal.value = strdup(value);
    node->expr_type = strdup("char");
    return node;
}

static ASTNode *ast_identifier(const char *name) {
    ASTNode *node = new_ast_node(AST_IDENTIFIER);
    node->data.identifier.name = strdup(name);
    /* Type will be filled in by type checking pass */
    return node;
}

static ASTNode *ast_binary_op(const char *op, ASTNode *left, ASTNode *right) {
    ASTNode *node = new_ast_node(AST_BINARY_OP);
    node->data.binary_op.op = strdup(op);
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    return node;
}

static ASTNode *ast_unary_op(const char *op, ASTNode *operand) {
    ASTNode *node = new_ast_node(AST_UNARY_OP);
    node->data.unary_op.op = strdup(op);
    node->data.unary_op.operand = operand;
    return node;
}

static ASTNode *ast_assignment(const char *var_name, ASTNode *value) {
    ASTNode *node = new_ast_node(AST_ASSIGNMENT);
    node->data.assignment.var_name = strdup(var_name);
    node->data.assignment.value = value;
    return node;
}

static ASTNode *ast_declaration(const char *type, const char *name, int is_array, 
                                ASTNode *array_size, ASTNode *init_value) {
    ASTNode *node = new_ast_node(AST_DECLARATION);
    node->data.declaration.var_type = strdup(type);
    node->data.declaration.var_name = strdup(name);
    node->data.declaration.is_array = is_array;
    node->data.declaration.array_size = array_size;
    node->data.declaration.init_value = init_value;
    return node;
}

static ASTNode *ast_if_stmt(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *node = new_ast_node(AST_IF_STMT);
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.then_branch = then_branch;
    node->data.if_stmt.else_branch = else_branch;
    return node;
}

static ASTNode *ast_while_loop(ASTNode *condition, ASTNode *body) {
    ASTNode *node = new_ast_node(AST_WHILE_LOOP);
    node->data.while_loop.condition = condition;
    node->data.while_loop.body = body;
    return node;
}

static ASTNode *ast_for_loop(ASTNode *init, ASTNode *condition, ASTNode *update, ASTNode *body) {
    ASTNode *node = new_ast_node(AST_FOR_LOOP);
    node->data.for_loop.init = init;
    node->data.for_loop.condition = condition;
    node->data.for_loop.update = update;
    node->data.for_loop.body = body;
    return node;
}

static ASTNode *ast_return_stmt(ASTNode *value) {
    ASTNode *node = new_ast_node(AST_RETURN);
    node->data.return_stmt.value = value;
    return node;
}

static ASTNode *ast_block(ASTNode *statements) {
    ASTNode *node = new_ast_node(AST_BLOCK);
    node->data.block.statements = statements;
    return node;
}

static ASTNode *ast_stmt_list(ASTNode *stmt, ASTNode *next) {
    ASTNode *node = new_ast_node(AST_STMT_LIST);
    node->data.stmt_list.stmt = stmt;
    node->data.stmt_list.next = next;
    return node;
}

static ASTNode *ast_input(const char *var_name) {
    ASTNode *node = new_ast_node(AST_INPUT);
    node->data.input.var_name = strdup(var_name);
    return node;
}

static ASTNode *ast_output(ASTNode *expr) {
    ASTNode *node = new_ast_node(AST_OUTPUT);
    node->data.output.expr = expr;
    return node;
}

static ASTNode *ast_func_call(const char *func_name, ASTNode *args) {
    ASTNode *node = new_ast_node(AST_FUNCTION_CALL);
    node->data.func_call.func_name = strdup(func_name);
    node->data.func_call.args = args;
    return node;
}

static ASTNode *ast_array_access(const char *array_name, ASTNode *index) {
    ASTNode *node = new_ast_node(AST_ARRAY_ACCESS);
    node->data.array_access.array_name = strdup(array_name);
    node->data.array_access.index = index;
    return node;
}

/* Free AST tree (recursive) */
static void free_ast(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_BINARY_OP:
            free_ast(node->data.binary_op.left);
            free_ast(node->data.binary_op.right);
            free(node->data.binary_op.op);
            break;
        case AST_UNARY_OP:
            free_ast(node->data.unary_op.operand);
            free(node->data.unary_op.op);
            break;
        case AST_ASSIGNMENT:
            free_ast(node->data.assignment.value);
            free(node->data.assignment.var_name);
            break;
        case AST_IF_STMT:
            free_ast(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.then_branch);
            free_ast(node->data.if_stmt.else_branch);
            break;
        case AST_WHILE_LOOP:
            free_ast(node->data.while_loop.condition);
            free_ast(node->data.while_loop.body);
            break;
        case AST_STMT_LIST:
            free_ast(node->data.stmt_list.stmt);
            free_ast(node->data.stmt_list.next);
            break;
        /* Add more cases as needed */
    }
    
    if (node->expr_type) free(node->expr_type);
    free(node);
}

/* Print AST (for debugging) */
static void print_ast(ASTNode *node, int depth) {
    if (!node) return;
    
    int i;
    for (i = 0; i < depth; i++) fprintf(stderr, "  ");
    
    switch (node->type) {
        case AST_INTEGER:
            fprintf(stderr, "INT: %d\n", node->data.int_literal.value);
            break;
        case AST_FLOAT:
            fprintf(stderr, "FLOAT: %f\n", node->data.float_literal.value);
            break;
        case AST_IDENTIFIER:
            fprintf(stderr, "ID: %s\n", node->data.identifier.name);
            break;
        case AST_BINARY_OP:
            fprintf(stderr, "BINOP: %s\n", node->data.binary_op.op);
            print_ast(node->data.binary_op.left, depth + 1);
            print_ast(node->data.binary_op.right, depth + 1);
            break;
        case AST_ASSIGNMENT:
            fprintf(stderr, "ASSIGN: %s\n", node->data.assignment.var_name);
            print_ast(node->data.assignment.value, depth + 1);
            break;
        case AST_IF_STMT:
            fprintf(stderr, "IF\n");
            for (i = 0; i < depth + 1; i++) fprintf(stderr, "  ");
            fprintf(stderr, "Condition:\n");
            print_ast(node->data.if_stmt.condition, depth + 2);
            for (i = 0; i < depth + 1; i++) fprintf(stderr, "  ");
            fprintf(stderr, "Then:\n");
            print_ast(node->data.if_stmt.then_branch, depth + 2);
            if (node->data.if_stmt.else_branch) {
                for (i = 0; i < depth + 1; i++) fprintf(stderr, "  ");
                fprintf(stderr, "Else:\n");
                print_ast(node->data.if_stmt.else_branch, depth + 2);
            }
            break;
        case AST_STMT_LIST:
            fprintf(stderr, "STMT_LIST\n");
            print_ast(node->data.stmt_list.stmt, depth + 1);
            print_ast(node->data.stmt_list.next, depth);
            break;
        default:
            fprintf(stderr, "AST_NODE (type=%d)\n", node->type);
            break;
    }
}

/* ════════════════════════════════════════════════════════════════════════════
 * AST TRAVERSAL — Type Checking Pass
 * ════════════════════════════════════════════════════════════════════════════ */

static void ast_type_check(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_BINARY_OP:
            ast_type_check(node->data.binary_op.left);
            ast_type_check(node->data.binary_op.right);
            /* Determine result type based on operands */
            if (node->data.binary_op.left->expr_type && node->data.binary_op.right->expr_type) {
                node->expr_type = strdup(wider_type(
                    node->data.binary_op.left->expr_type,
                    node->data.binary_op.right->expr_type
                ));
            }
            break;
            
        case AST_UNARY_OP:
            ast_type_check(node->data.unary_op.operand);
            if (node->data.unary_op.operand->expr_type) {
                node->expr_type = strdup(node->data.unary_op.operand->expr_type);
            }
            break;
            
        case AST_IDENTIFIER:
            /* Look up type from symbol table */
            node->expr_type = strdup(lookup_type(node->data.identifier.name));
            break;
            
        case AST_ASSIGNMENT:
            ast_type_check(node->data.assignment.value);
            {
                const char *var_type = lookup_type(node->data.assignment.var_name);
                const char *expr_type = node->data.assignment.value->expr_type;
                check_type_compat(node->data.assignment.var_name, var_type, expr_type);
            }
            break;
            
        case AST_DECLARATION:
            if (node->data.declaration.init_value) {
                ast_type_check(node->data.declaration.init_value);
                check_type_compat(
                    node->data.declaration.var_name,
                    node->data.declaration.var_type,
                    node->data.declaration.init_value->expr_type
                );
            }
            /* Add to symbol table */
            add_symbol(
                node->data.declaration.var_name,
                node->data.declaration.var_type,
                0,  /* not a function */
                node->data.declaration.is_array
            );
            break;
            
        case AST_IF_STMT:
            ast_type_check(node->data.if_stmt.condition);
            ast_type_check(node->data.if_stmt.then_branch);
            ast_type_check(node->data.if_stmt.else_branch);
            break;
            
        case AST_WHILE_LOOP:
            ast_type_check(node->data.while_loop.condition);
            ast_type_check(node->data.while_loop.body);
            break;
            
        case AST_FOR_LOOP:
            ast_type_check(node->data.for_loop.init);
            ast_type_check(node->data.for_loop.condition);
            ast_type_check(node->data.for_loop.update);
            ast_type_check(node->data.for_loop.body);
            break;
            
        case AST_RETURN:
            if (node->data.return_stmt.value) {
                ast_type_check(node->data.return_stmt.value);
            }
            break;
            
        case AST_OUTPUT:
            ast_type_check(node->data.output.expr);
            break;
            
        case AST_BLOCK:
            ast_type_check(node->data.block.statements);
            break;
            
        case AST_STMT_LIST:
            ast_type_check(node->data.stmt_list.stmt);
            ast_type_check(node->data.stmt_list.next);
            break;
            
        case AST_ARRAY_ACCESS:
            ast_type_check(node->data.array_access.index);
            node->expr_type = strdup(lookup_type(node->data.array_access.array_name));
            break;
            
        case AST_FUNCTION_CALL:
            ast_type_check(node->data.func_call.args);
            /* Function return type would be looked up from symbol table */
            break;
            
        default:
            /* Literals and other nodes don't need type checking */
            break;
    }
}

/* ════════════════════════════════════════════════════════════════════════════
 * AST TRAVERSAL — TAC Generation Pass
 * ════════════════════════════════════════════════════════════════════════════ */

static char *ast_gen_tac(ASTNode *node) {
    if (!node) return strdup("");
    
    char *tac = NULL;
    char temp[256];
    
    switch (node->type) {
        case AST_INTEGER:
            snprintf(temp, sizeof(temp), "%d", node->data.int_literal.value);
            return strdup(temp);
            
        case AST_FLOAT:
            snprintf(temp, sizeof(temp), "%f", node->data.float_literal.value);
            return strdup(temp);
            
        case AST_IDENTIFIER:
            return strdup(node->data.identifier.name);
            
        case AST_BINARY_OP: {
            char *left_tac = ast_gen_tac(node->data.binary_op.left);
            char *right_tac = ast_gen_tac(node->data.binary_op.right);
            char *result = new_temp();
            
            tac_emit("", "%s = %s %s %s", result, left_tac, 
                     node->data.binary_op.op, right_tac);
            
            free(left_tac);
            free(right_tac);
            return result;
        }
        
        case AST_UNARY_OP: {
            char *operand_tac = ast_gen_tac(node->data.unary_op.operand);
            char *result = new_temp();
            
            tac_emit("", "%s = %s%s", result, node->data.unary_op.op, operand_tac);
            
            free(operand_tac);
            return result;
        }
        
        case AST_ASSIGNMENT: {
            char *value_tac = ast_gen_tac(node->data.assignment.value);
            
            tac_emit("", "%s = %s", node->data.assignment.var_name, value_tac);
            
            free(value_tac);
            return strdup(node->data.assignment.var_name);
        }
        
        case AST_IF_STMT: {
            char *cond_tac = ast_gen_tac(node->data.if_stmt.condition);
            char *else_label = new_label();
            char *end_label = new_label();
            
            tac_emit("", "if_false %s goto %s", cond_tac, else_label);
            free(cond_tac);
            
            ast_gen_tac(node->data.if_stmt.then_branch);
            
            if (node->data.if_stmt.else_branch) {
                tac_emit("", "goto %s", end_label);
                tac_emit("", "%s:", else_label);
                ast_gen_tac(node->data.if_stmt.else_branch);
                tac_emit("", "%s:", end_label);
            } else {
                tac_emit("", "%s:", else_label);
            }
            
            return strdup("");
        }
        
        case AST_WHILE_LOOP: {
            char *start_label = new_label();
            char *end_label = new_label();
            
            tac_emit("", "%s:", start_label);
            char *cond_tac = ast_gen_tac(node->data.while_loop.condition);
            tac_emit("", "if_false %s goto %s", cond_tac, end_label);
            free(cond_tac);
            
            ast_gen_tac(node->data.while_loop.body);
            
            tac_emit("", "goto %s", start_label);
            tac_emit("", "%s:", end_label);
            
            return strdup("");
        }
        
        case AST_STMT_LIST:
            ast_gen_tac(node->data.stmt_list.stmt);
            ast_gen_tac(node->data.stmt_list.next);
            return strdup("");
            
        case AST_BLOCK:
            return ast_gen_tac(node->data.block.statements);
            
        case AST_RETURN:
            if (node->data.return_stmt.value) {
                char *val_tac = ast_gen_tac(node->data.return_stmt.value);
                tac_emit("", "return %s", val_tac);
                free(val_tac);
            } else {
                tac_emit("", "return");
            }
            return strdup("");
            
        default:
            return strdup("");
    }
}

/* ════════════════════════════════════════════════════════════════════════════
 * AST TRAVERSAL — C Code Generation Pass
 * ════════════════════════════════════════════════════════════════════════════ */

static void ast_gen_code(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_DECLARATION: {
            const char *c_type = map_smpl_to_c_type(node->data.declaration.var_type);
            print_indent();
            fprintf(output, "%s %s", c_type, node->data.declaration.var_name);
            
            if (node->data.declaration.is_array && node->data.declaration.array_size) {
                if (node->data.declaration.array_size->type == AST_INTEGER) {
                    fprintf(output, "[%d]", node->data.declaration.array_size->data.int_literal.value);
                }
            }
            
            if (node->data.declaration.init_value) {
                fprintf(output, " = ");
                ast_gen_code(node->data.declaration.init_value);
            }
            
            fprintf(output, ";\n");
            break;
        }
        
        case AST_ASSIGNMENT:
            print_indent();
            fprintf(output, "%s = ", node->data.assignment.var_name);
            ast_gen_code(node->data.assignment.value);
            fprintf(output, ";\n");
            break;
            
        case AST_IF_STMT:
            print_indent();
            fprintf(output, "if (");
            ast_gen_code(node->data.if_stmt.condition);
            fprintf(output, ") {\n");
            indent_level++;
            ast_gen_code(node->data.if_stmt.then_branch);
            indent_level--;
            print_indent();
            fprintf(output, "}");
            
            if (node->data.if_stmt.else_branch) {
                fprintf(output, " else {\n");
                indent_level++;
                ast_gen_code(node->data.if_stmt.else_branch);
                indent_level--;
                print_indent();
                fprintf(output, "}\n");
            } else {
                fprintf(output, "\n");
            }
            break;
            
        case AST_WHILE_LOOP:
            print_indent();
            fprintf(output, "while (");
            ast_gen_code(node->data.while_loop.condition);
            fprintf(output, ") {\n");
            indent_level++;
            ast_gen_code(node->data.while_loop.body);
            indent_level--;
            print_indent();
            fprintf(output, "}\n");
            break;
            
        case AST_FOR_LOOP:
            print_indent();
            fprintf(output, "for (");
            if (node->data.for_loop.init) {
                /* Remove trailing semicolon/newline for init */
                ast_gen_code(node->data.for_loop.init);
            }
            fprintf(output, "; ");
            if (node->data.for_loop.condition) {
                ast_gen_code(node->data.for_loop.condition);
            }
            fprintf(output, "; ");
            if (node->data.for_loop.update) {
                ast_gen_code(node->data.for_loop.update);
            }
            fprintf(output, ") {\n");
            indent_level++;
            ast_gen_code(node->data.for_loop.body);
            indent_level--;
            print_indent();
            fprintf(output, "}\n");
            break;
            
        case AST_RETURN:
            print_indent();
            fprintf(output, "return");
            if (node->data.return_stmt.value) {
                fprintf(output, " ");
                ast_gen_code(node->data.return_stmt.value);
            }
            fprintf(output, ";\n");
            break;
            
        case AST_INPUT: {
            const char *var_type = lookup_type(node->data.input.var_name);
            const char *fmt = scanf_format_for_type(var_type);
            print_indent();
            fprintf(output, "scanf(\"%s\", &%s);\n", fmt, node->data.input.var_name);
            break;
        }
        
        case AST_OUTPUT: {
            print_indent();
            fprintf(output, "printf(");
            
            /* Determine format based on expression type */
            if (node->data.output.expr->expr_type) {
                const char *fmt = format_for_type(node->data.output.expr->expr_type);
                fprintf(output, "\"%s\\n\", ", fmt);
            } else {
                fprintf(output, "\"%%d\\n\", ");
            }
            
            ast_gen_code(node->data.output.expr);
            fprintf(output, ");\n");
            break;
        }
        
        case AST_BINARY_OP:
            fprintf(output, "(");
            ast_gen_code(node->data.binary_op.left);
            fprintf(output, " %s ", node->data.binary_op.op);
            ast_gen_code(node->data.binary_op.right);
            fprintf(output, ")");
            break;
            
        case AST_UNARY_OP:
            fprintf(output, "(%s", node->data.unary_op.op);
            ast_gen_code(node->data.unary_op.operand);
            fprintf(output, ")");
            break;
            
        case AST_INTEGER:
            fprintf(output, "%d", node->data.int_literal.value);
            break;
            
        case AST_FLOAT:
            fprintf(output, "%f", node->data.float_literal.value);
            break;
            
        case AST_STRING:
            fprintf(output, "%s", node->data.string_literal.value);
            break;
            
        case AST_CHAR:
            fprintf(output, "%s", node->data.char_literal.value);
            break;
            
        case AST_IDENTIFIER:
            fprintf(output, "%s", node->data.identifier.name);
            break;
            
        case AST_ARRAY_ACCESS:
            fprintf(output, "%s[", node->data.array_access.array_name);
            ast_gen_code(node->data.array_access.index);
            fprintf(output, "]");
            break;
            
        case AST_FUNCTION_CALL:
            fprintf(output, "%s(", node->data.func_call.func_name);
            /* Generate arguments */
            if (node->data.func_call.args) {
                ast_gen_code(node->data.func_call.args);
            }
            fprintf(output, ")");
            break;
            
        case AST_STMT_LIST:
            ast_gen_code(node->data.stmt_list.stmt);
            ast_gen_code(node->data.stmt_list.next);
            break;
            
        case AST_BLOCK:
            ast_gen_code(node->data.block.statements);
            break;
            
        case AST_BREAK:
            print_indent();
            fprintf(output, "break;\n");
            break;
            
        case AST_CONTINUE:
            print_indent();
            fprintf(output, "continue;\n");
            break;
            
        default:
            break;
    }
}


#line 1301 "smpl_parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "smpl_parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOK_MISSION = 3,                /* TOK_MISSION  */
  YYSYMBOL_TOK_LANDING = 4,                /* TOK_LANDING  */
  YYSYMBOL_TOK_LAUNCH = 5,                 /* TOK_LAUNCH  */
  YYSYMBOL_TOK_ABORT = 6,                  /* TOK_ABORT  */
  YYSYMBOL_TOK_CARGO_INT = 7,              /* TOK_CARGO_INT  */
  YYSYMBOL_TOK_CARGO_FLOAT = 8,            /* TOK_CARGO_FLOAT  */
  YYSYMBOL_TOK_CARGO_CHAR = 9,             /* TOK_CARGO_CHAR  */
  YYSYMBOL_TOK_CARGO_DOUBLE = 10,          /* TOK_CARGO_DOUBLE  */
  YYSYMBOL_TOK_VOID_SPACE = 11,            /* TOK_VOID_SPACE  */
  YYSYMBOL_TOK_LOAD = 12,                  /* TOK_LOAD  */
  YYSYMBOL_TOK_STORE = 13,                 /* TOK_STORE  */
  YYSYMBOL_TOK_CARGO_ARRAY = 14,           /* TOK_CARGO_ARRAY  */
  YYSYMBOL_TOK_COMBINE = 15,               /* TOK_COMBINE  */
  YYSYMBOL_TOK_REDUCE = 16,                /* TOK_REDUCE  */
  YYSYMBOL_TOK_AMPLIFY = 17,               /* TOK_AMPLIFY  */
  YYSYMBOL_TOK_SPLIT = 18,                 /* TOK_SPLIT  */
  YYSYMBOL_TOK_REMAINDER = 19,             /* TOK_REMAINDER  */
  YYSYMBOL_TOK_BOOST = 20,                 /* TOK_BOOST  */
  YYSYMBOL_TOK_DEGRADE = 21,               /* TOK_DEGRADE  */
  YYSYMBOL_TOK_EXCEEDS = 22,               /* TOK_EXCEEDS  */
  YYSYMBOL_TOK_BELOW = 23,                 /* TOK_BELOW  */
  YYSYMBOL_TOK_EXCEEDS_OR_EQUAL = 24,      /* TOK_EXCEEDS_OR_EQUAL  */
  YYSYMBOL_TOK_BELOW_OR_EQUAL = 25,        /* TOK_BELOW_OR_EQUAL  */
  YYSYMBOL_TOK_MATCHES = 26,               /* TOK_MATCHES  */
  YYSYMBOL_TOK_DIFFERS = 27,               /* TOK_DIFFERS  */
  YYSYMBOL_TOK_BOTH = 28,                  /* TOK_BOTH  */
  YYSYMBOL_TOK_EITHER = 29,                /* TOK_EITHER  */
  YYSYMBOL_TOK_NEGATE = 30,                /* TOK_NEGATE  */
  YYSYMBOL_TOK_CHECK_IF = 31,              /* TOK_CHECK_IF  */
  YYSYMBOL_TOK_ELSE_CHECK = 32,            /* TOK_ELSE_CHECK  */
  YYSYMBOL_TOK_OTHERWISE = 33,             /* TOK_OTHERWISE  */
  YYSYMBOL_TOK_PROTOCOL = 34,              /* TOK_PROTOCOL  */
  YYSYMBOL_TOK_SCENARIO = 35,              /* TOK_SCENARIO  */
  YYSYMBOL_TOK_DEFAULT_PROTOCOL = 36,      /* TOK_DEFAULT_PROTOCOL  */
  YYSYMBOL_TOK_ORBIT_WHILE = 37,           /* TOK_ORBIT_WHILE  */
  YYSYMBOL_TOK_SEQUENCE = 38,              /* TOK_SEQUENCE  */
  YYSYMBOL_TOK_REPEAT = 39,                /* TOK_REPEAT  */
  YYSYMBOL_TOK_UNTIL = 40,                 /* TOK_UNTIL  */
  YYSYMBOL_TOK_TERMINATE = 41,             /* TOK_TERMINATE  */
  YYSYMBOL_TOK_SKIP = 42,                  /* TOK_SKIP  */
  YYSYMBOL_TOK_FUNCTION = 43,              /* TOK_FUNCTION  */
  YYSYMBOL_TOK_RETURN_CARGO = 44,          /* TOK_RETURN_CARGO  */
  YYSYMBOL_TOK_RECEIVE = 45,               /* TOK_RECEIVE  */
  YYSYMBOL_TOK_TRANSMIT = 46,              /* TOK_TRANSMIT  */
  YYSYMBOL_TOK_INTEGER = 47,               /* TOK_INTEGER  */
  YYSYMBOL_TOK_FLOAT_NUM = 48,             /* TOK_FLOAT_NUM  */
  YYSYMBOL_TOK_CHAR_LITERAL = 49,          /* TOK_CHAR_LITERAL  */
  YYSYMBOL_TOK_STRING_LITERAL = 50,        /* TOK_STRING_LITERAL  */
  YYSYMBOL_TOK_IDENTIFIER = 51,            /* TOK_IDENTIFIER  */
  YYSYMBOL_TOK_SEMICOLON = 52,             /* TOK_SEMICOLON  */
  YYSYMBOL_TOK_COLON = 53,                 /* TOK_COLON  */
  YYSYMBOL_TOK_COMMA = 54,                 /* TOK_COMMA  */
  YYSYMBOL_TOK_LPAREN = 55,                /* TOK_LPAREN  */
  YYSYMBOL_TOK_RPAREN = 56,                /* TOK_RPAREN  */
  YYSYMBOL_TOK_LBRACKET = 57,              /* TOK_LBRACKET  */
  YYSYMBOL_TOK_RBRACKET = 58,              /* TOK_RBRACKET  */
  YYSYMBOL_TOK_LBRACE = 59,                /* TOK_LBRACE  */
  YYSYMBOL_TOK_RBRACE = 60,                /* TOK_RBRACE  */
  YYSYMBOL_YYACCEPT = 61,                  /* $accept  */
  YYSYMBOL_program = 62,                   /* program  */
  YYSYMBOL_mission_header = 63,            /* mission_header  */
  YYSYMBOL_function_list = 64,             /* function_list  */
  YYSYMBOL_statement_list = 65,            /* statement_list  */
  YYSYMBOL_statement = 66,                 /* statement  */
  YYSYMBOL_block_open = 67,                /* block_open  */
  YYSYMBOL_block = 68,                     /* block  */
  YYSYMBOL_data_type = 69,                 /* data_type  */
  YYSYMBOL_declaration_stmt = 70,          /* declaration_stmt  */
  YYSYMBOL_init_list = 71,                 /* init_list  */
  YYSYMBOL_assignment_stmt = 72,           /* assignment_stmt  */
  YYSYMBOL_boost_stmt = 73,                /* boost_stmt  */
  YYSYMBOL_degrade_stmt = 74,              /* degrade_stmt  */
  YYSYMBOL_expr = 75,                      /* expr  */
  YYSYMBOL_arg_list = 76,                  /* arg_list  */
  YYSYMBOL_if_header = 77,                 /* if_header  */
  YYSYMBOL_else_if_header = 78,            /* else_if_header  */
  YYSYMBOL_otherwise_header = 79,          /* otherwise_header  */
  YYSYMBOL_if_stmt = 80,                   /* if_stmt  */
  YYSYMBOL_else_chain = 81,                /* else_chain  */
  YYSYMBOL_switch_header = 82,             /* switch_header  */
  YYSYMBOL_switch_stmt = 83,               /* switch_stmt  */
  YYSYMBOL_case_list = 84,                 /* case_list  */
  YYSYMBOL_case_item = 85,                 /* case_item  */
  YYSYMBOL_86_1 = 86,                      /* $@1  */
  YYSYMBOL_87_2 = 87,                      /* $@2  */
  YYSYMBOL_while_header = 88,              /* while_header  */
  YYSYMBOL_while_loop = 89,                /* while_loop  */
  YYSYMBOL_for_header = 90,                /* for_header  */
  YYSYMBOL_for_loop = 91,                  /* for_loop  */
  YYSYMBOL_for_init = 92,                  /* for_init  */
  YYSYMBOL_for_update = 93,                /* for_update  */
  YYSYMBOL_do_header = 94,                 /* do_header  */
  YYSYMBOL_do_while_loop = 95,             /* do_while_loop  */
  YYSYMBOL_loop_control_stmt = 96,         /* loop_control_stmt  */
  YYSYMBOL_func_header_params = 97,        /* func_header_params  */
  YYSYMBOL_func_header_void = 98,          /* func_header_void  */
  YYSYMBOL_function_decl = 99,             /* function_decl  */
  YYSYMBOL_param_list = 100,               /* param_list  */
  YYSYMBOL_param = 101,                    /* param  */
  YYSYMBOL_return_stmt = 102,              /* return_stmt  */
  YYSYMBOL_io_stmt = 103,                  /* io_stmt  */
  YYSYMBOL_expr_stmt = 104                 /* expr_stmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   618

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  235

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   315


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  1318,  1318,  1324,  1333,  1342,  1343,  1351,  1352,  1356,
    1357,  1358,  1359,  1360,  1361,  1362,  1363,  1364,  1365,  1366,
    1367,  1368,  1369,  1370,  1378,  1387,  1401,  1402,  1403,  1404,
    1405,  1413,  1421,  1431,  1440,  1453,  1455,  1468,  1482,  1499,
    1514,  1544,  1564,  1584,  1611,  1638,  1657,  1667,  1677,  1687,
    1697,  1707,  1719,  1729,  1741,  1752,  1764,  1779,  1789,  1799,
    1805,  1815,  1822,  1829,  1839,  1841,  1854,  1868,  1888,  1902,
    1910,  1914,  1922,  1923,  1936,  1948,  1959,  1960,  1965,  1964,
    1974,  1973,  1990,  2005,  2017,  2033,  2044,  2052,  2067,  2069,
    2071,  2080,  2091,  2107,  2109,  2118,  2128,  2138,  2140,  2145,
    2147,  2152,  2165,  2172,  2190,  2211,  2233
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOK_MISSION",
  "TOK_LANDING", "TOK_LAUNCH", "TOK_ABORT", "TOK_CARGO_INT",
  "TOK_CARGO_FLOAT", "TOK_CARGO_CHAR", "TOK_CARGO_DOUBLE",
  "TOK_VOID_SPACE", "TOK_LOAD", "TOK_STORE", "TOK_CARGO_ARRAY",
  "TOK_COMBINE", "TOK_REDUCE", "TOK_AMPLIFY", "TOK_SPLIT", "TOK_REMAINDER",
  "TOK_BOOST", "TOK_DEGRADE", "TOK_EXCEEDS", "TOK_BELOW",
  "TOK_EXCEEDS_OR_EQUAL", "TOK_BELOW_OR_EQUAL", "TOK_MATCHES",
  "TOK_DIFFERS", "TOK_BOTH", "TOK_EITHER", "TOK_NEGATE", "TOK_CHECK_IF",
  "TOK_ELSE_CHECK", "TOK_OTHERWISE", "TOK_PROTOCOL", "TOK_SCENARIO",
  "TOK_DEFAULT_PROTOCOL", "TOK_ORBIT_WHILE", "TOK_SEQUENCE", "TOK_REPEAT",
  "TOK_UNTIL", "TOK_TERMINATE", "TOK_SKIP", "TOK_FUNCTION",
  "TOK_RETURN_CARGO", "TOK_RECEIVE", "TOK_TRANSMIT", "TOK_INTEGER",
  "TOK_FLOAT_NUM", "TOK_CHAR_LITERAL", "TOK_STRING_LITERAL",
  "TOK_IDENTIFIER", "TOK_SEMICOLON", "TOK_COLON", "TOK_COMMA",
  "TOK_LPAREN", "TOK_RPAREN", "TOK_LBRACKET", "TOK_RBRACKET", "TOK_LBRACE",
  "TOK_RBRACE", "$accept", "program", "mission_header", "function_list",
  "statement_list", "statement", "block_open", "block", "data_type",
  "declaration_stmt", "init_list", "assignment_stmt", "boost_stmt",
  "degrade_stmt", "expr", "arg_list", "if_header", "else_if_header",
  "otherwise_header", "if_stmt", "else_chain", "switch_header",
  "switch_stmt", "case_list", "case_item", "$@1", "$@2", "while_header",
  "while_loop", "for_header", "for_loop", "for_init", "for_update",
  "do_header", "do_while_loop", "loop_control_stmt", "func_header_params",
  "func_header_void", "function_decl", "param_list", "param",
  "return_stmt", "io_stmt", "expr_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-35)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      16,   -35,    87,    28,   -35,    16,    36,    36,   -35,   -35,
     -35,   -35,   -35,   -35,   -21,   -35,    88,   -35,   -35,   -35,
     -35,   -35,   -35,    -8,    35,    87,   363,   363,   363,   363,
     363,    33,    38,   363,    55,    65,    66,    68,   -35,    39,
      72,   353,    77,   363,   -35,   -35,   -35,   -35,     8,   363,
     -35,   -35,   -35,   -35,   -35,   -35,   472,    36,   -35,    32,
     -35,    36,   -35,    36,   -35,    36,   -35,   -35,   -35,   -35,
     -35,   -35,   191,   238,     2,   -35,     0,   -33,   337,   337,
     337,   337,   337,    90,    92,   -35,   363,   363,   363,     6,
     -35,   -35,   -35,   480,   102,   511,   363,   295,   363,   397,
     363,   363,   363,   363,   363,   363,   363,   363,   -35,    69,
      94,   103,    10,   -35,   -35,   -35,   117,   120,   -35,   -35,
     123,    -6,   -35,   126,    12,   363,   -35,   -35,   -35,   -35,
     -35,   -35,   -35,   405,   413,   421,    87,   162,   127,   -35,
     -35,   -35,   519,   -35,   574,    29,    11,   -35,   -35,   -35,
     -35,   -35,   137,   137,   124,    89,   125,   -35,    36,    36,
     -35,   129,   -35,   -35,   -35,   130,   -35,   -35,    87,   -35,
     121,   363,   -35,   142,   -35,   181,   -35,   136,   363,   363,
     -35,   363,   -35,   175,   363,    69,   -35,   -35,   -35,   363,
     -35,   144,   527,   -35,   -35,   176,   574,   558,   574,   363,
     456,   -35,   -35,   285,   464,   134,   -35,   363,    22,   566,
     -35,   285,   141,    14,   574,   143,   146,   185,   148,   -35,
     -35,   154,   -35,   -35,   -35,   363,   -35,   363,   574,   -34,
     574,   363,   150,   574,   -35
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,     0,     0,     7,     0,     0,     0,     5,    26,
      27,    28,    29,    30,     0,     1,     0,     7,     6,    24,
       7,    97,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,    59,    60,    61,    62,    55,     0,
       8,    22,     9,    10,    11,    12,     0,     0,    13,     0,
      14,     0,    15,     0,    16,     0,    17,    20,    21,    19,
      18,    23,     0,     0,     0,     3,     0,    55,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
      93,    94,   103,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,    69,
       0,     0,     0,    76,    83,    85,     0,     0,    25,    96,
       0,     0,    99,     0,     0,     0,    41,    42,    43,    44,
      45,    39,    40,     0,     0,     0,     0,     0,     0,   102,
     105,   104,     0,    58,    64,     0,     0,    63,    46,    47,
      48,    49,    50,    51,    52,    53,     0,    68,     0,     0,
      70,     0,    80,    75,    77,     0,     2,   101,     0,    95,
       0,     0,    31,     0,    66,     0,    82,     0,     0,     0,
      37,     0,    57,    56,     0,    71,    73,    78,     7,     0,
     100,     0,     0,    56,    74,     0,    87,     0,    65,     0,
       0,    72,     7,    81,     0,     0,    32,     0,     0,     0,
      67,    79,     0,     0,    86,     0,     0,     0,     0,    38,
      92,     0,    33,    88,    89,     0,    84,     0,    90,     0,
      35,     0,     0,    36,    34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -35,   -35,   209,   -35,   -12,   -35,   -35,    25,     4,   -35,
     -35,   -35,   -35,   -35,   -26,   -35,   -35,   -35,   -35,   -35,
      30,   -35,   -35,   -35,   104,   -35,   -35,   -35,   -35,   -35,
     -35,   -35,   -35,   -35,   -35,   -35,   -35,   -35,    44,   -35,
      49,   -35,   -35,   -35
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,     4,     5,    16,    50,    20,    51,   120,    52,
     229,    53,    54,    55,    56,   145,    57,   158,   159,    58,
     160,    59,    60,   112,   113,   202,   188,    61,    62,    63,
      64,   138,   218,    65,    66,    67,     6,     7,    68,   121,
     122,    69,    70,    71
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      78,    79,    80,    81,    82,    72,    14,    85,    73,     9,
      10,    11,    12,    13,   123,    93,   163,    95,   136,     1,
     231,    96,    97,    99,   125,   171,   232,   221,    15,    76,
      23,    21,    22,   100,   101,   102,   103,   104,   105,   106,
     107,    19,   215,   216,     8,   110,   111,    74,   168,    18,
     169,   124,   126,   127,   128,   129,   130,   137,   119,     2,
     133,   134,   135,    97,   172,    98,   222,   110,   111,   183,
     142,   144,   146,   217,   148,   149,   150,   151,   152,   153,
     154,   155,   109,   181,    83,   182,   114,    75,   115,    84,
     116,    90,    24,    19,     9,    10,    11,    12,    13,   173,
      25,   156,   157,    26,    27,    28,    29,    30,    31,    32,
      86,   100,   101,   102,   103,   104,   105,   106,    33,    34,
      87,    88,    35,    89,    91,    36,    37,    38,    94,    39,
      40,     2,    41,    42,    43,    44,    45,    46,    47,    48,
     177,   161,   131,    49,   132,   192,   100,   101,   102,   103,
     104,   105,   196,   197,   140,   198,   162,   165,   200,   100,
     101,   102,   103,   204,   100,   101,   102,   103,   104,   105,
     106,   107,   166,   209,   167,   178,   203,   170,   191,   179,
     184,   214,   187,   185,   186,   189,   194,   195,   199,   207,
     211,   205,   213,   220,   223,   117,    19,   224,   225,   228,
     193,   230,   234,    25,   226,   233,    26,    27,    28,    29,
      30,    31,    32,   227,    17,   201,   164,   190,     0,     0,
       0,    33,    34,     0,     0,    35,     0,     0,    36,    37,
      38,     0,    39,    40,     2,    41,    42,    43,    44,    45,
      46,    47,    48,    19,   118,     0,    49,     0,     0,     0,
      25,     0,     0,    26,    27,    28,    29,    30,    31,    32,
       0,     0,     0,     0,     0,     0,     0,     0,    33,    34,
       0,     0,    35,     0,     0,    36,    37,    38,     0,    39,
      40,     2,    41,    42,    43,    44,    45,    46,    47,    48,
      19,     0,     0,    49,     0,     0,     0,    25,     0,     0,
      26,    27,    28,    29,    30,    31,    32,     0,     0,     0,
      26,    27,    28,    29,    30,    33,    34,     0,     0,    35,
       0,     0,    36,    37,    38,    33,    39,    40,     2,    41,
      42,    43,    44,    45,    46,    47,    48,     0,     0,     0,
      49,     0,    44,    45,    46,    47,    77,     0,     0,     0,
      49,   143,    26,    27,    28,    29,    30,     0,     0,   100,
     101,   102,   103,   104,   105,   106,   107,    33,    26,    27,
      28,    29,    30,     0,     0,     0,     0,     0,    26,    27,
      28,    29,    30,    33,    44,    45,    46,    47,    77,     0,
       0,     0,    49,    33,     0,     0,     0,     0,     0,     0,
      44,    45,    46,    47,    77,    92,     0,     0,    49,     0,
      44,    45,    46,    47,    77,     0,     0,     0,    49,   100,
     101,   102,   103,   104,   105,   106,   107,   100,   101,   102,
     103,   104,   105,   106,   107,   100,   101,   102,   103,   104,
     105,   106,   107,   100,   101,   102,   103,   104,   105,   106,
     107,     0,     0,   147,     0,     0,     0,     0,     0,     0,
       0,   174,     0,     0,     0,     0,     0,     0,     0,   175,
       0,     0,     0,     0,     0,     0,     0,   176,   100,   101,
     102,   103,   104,   105,   106,   107,   100,   101,   102,   103,
     104,   105,   106,   107,   100,   101,   102,   103,   104,   105,
     106,   107,   100,   101,   102,   103,   104,   105,   106,   107,
       0,     0,   210,     0,     0,     0,     0,     0,     0,     0,
     212,     0,     0,     0,   108,     0,     0,     0,     0,     0,
       0,     0,   139,   100,   101,   102,   103,   104,   105,   106,
     107,   100,   101,   102,   103,   104,   105,   106,   107,   100,
     101,   102,   103,   104,   105,   106,   107,     0,     0,     0,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   206,
     100,   101,   102,   103,   104,   105,   106,   107,   100,   101,
     102,   103,   104,   105,   106,   107,   100,   101,   102,   103,
     104,   105,   106,   107,     0,     0,     0,     0,     0,     0,
     208,     0,     0,     0,     0,     0,     0,     0,   219
};

static const yytype_int16 yycheck[] =
{
      26,    27,    28,    29,    30,    17,     2,    33,    20,     7,
       8,     9,    10,    11,    14,    41,     6,    43,    12,     3,
      54,    13,    55,    49,    57,    13,    60,    13,     0,    25,
      51,     6,     7,    22,    23,    24,    25,    26,    27,    28,
      29,     5,    20,    21,     0,    35,    36,    55,    54,     5,
      56,    51,    78,    79,    80,    81,    82,    51,    56,    43,
      86,    87,    88,    55,    52,    57,    52,    35,    36,    58,
      96,    97,    98,    51,   100,   101,   102,   103,   104,   105,
     106,   107,    57,    54,    51,    56,    61,    52,    63,    51,
      65,    52,     4,     5,     7,     8,     9,    10,    11,   125,
      12,    32,    33,    15,    16,    17,    18,    19,    20,    21,
      55,    22,    23,    24,    25,    26,    27,    28,    30,    31,
      55,    55,    34,    55,    52,    37,    38,    39,    51,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
     136,    47,    52,    55,    52,   171,    22,    23,    24,    25,
      26,    27,   178,   179,    52,   181,    53,    40,   184,    22,
      23,    24,    25,   189,    22,    23,    24,    25,    26,    27,
      28,    29,    52,   199,    51,    13,   188,    51,    57,    52,
      55,   207,    53,   158,   159,    55,     5,    51,    13,    13,
     202,    47,    58,    52,    51,     4,     5,    51,    13,   225,
      58,   227,    52,    12,    56,   231,    15,    16,    17,    18,
      19,    20,    21,    59,     5,   185,   112,   168,    -1,    -1,
      -1,    30,    31,    -1,    -1,    34,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,     5,     6,    -1,    55,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,
      -1,    -1,    34,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
       5,    -1,    -1,    55,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    30,    31,    -1,    -1,    34,
      -1,    -1,    37,    38,    39,    30,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    -1,    -1,    -1,
      55,    -1,    47,    48,    49,    50,    51,    -1,    -1,    -1,
      55,    56,    15,    16,    17,    18,    19,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    30,    47,    48,    49,    50,    51,    -1,
      -1,    -1,    55,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    48,    49,    50,    51,    52,    -1,    -1,    55,    -1,
      47,    48,    49,    50,    51,    -1,    -1,    -1,    55,    22,
      23,    24,    25,    26,    27,    28,    29,    22,    23,    24,
      25,    26,    27,    28,    29,    22,    23,    24,    25,    26,
      27,    28,    29,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    22,    23,
      24,    25,    26,    27,    28,    29,    22,    23,    24,    25,
      26,    27,    28,    29,    22,    23,    24,    25,    26,    27,
      28,    29,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    52,    22,    23,    24,    25,    26,    27,    28,
      29,    22,    23,    24,    25,    26,    27,    28,    29,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      22,    23,    24,    25,    26,    27,    28,    29,    22,    23,
      24,    25,    26,    27,    28,    29,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    43,    62,    63,    64,    97,    98,    99,     7,
       8,     9,    10,    11,    69,     0,    65,    63,    99,     5,
      67,    68,    68,    51,     4,    12,    15,    16,    17,    18,
      19,    20,    21,    30,    31,    34,    37,    38,    39,    41,
      42,    44,    45,    46,    47,    48,    49,    50,    51,    55,
      66,    68,    70,    72,    73,    74,    75,    77,    80,    82,
      83,    88,    89,    90,    91,    94,    95,    96,    99,   102,
     103,   104,    65,    65,    55,    52,    69,    51,    75,    75,
      75,    75,    75,    51,    51,    75,    55,    55,    55,    55,
      52,    52,    52,    75,    51,    75,    13,    55,    57,    75,
      22,    23,    24,    25,    26,    27,    28,    29,    52,    68,
      35,    36,    84,    85,    68,    68,    68,     4,     6,    56,
      69,   100,   101,    14,    51,    57,    75,    75,    75,    75,
      75,    52,    52,    75,    75,    75,    12,    51,    92,    52,
      52,    52,    75,    56,    75,    76,    75,    56,    75,    75,
      75,    75,    75,    75,    75,    75,    32,    33,    78,    79,
      81,    47,    53,     6,    85,    40,    52,    51,    54,    56,
      51,    13,    52,    75,    56,    56,    56,    69,    13,    52,
      52,    54,    56,    58,    55,    68,    68,    53,    87,    55,
     101,    57,    75,    58,     5,    51,    75,    75,    75,    13,
      75,    81,    86,    65,    75,    47,    52,    13,    52,    75,
      56,    65,    56,    58,    75,    20,    21,    51,    93,    52,
      52,    13,    52,    51,    51,    13,    56,    59,    75,    71,
      75,    54,    60,    75,    52
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    61,    62,    62,    63,    64,    64,    65,    65,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    67,    68,    69,    69,    69,    69,
      69,    70,    70,    70,    70,    71,    71,    72,    72,    73,
      74,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    76,    76,    77,    78,    79,    80,
      80,    81,    81,    81,    82,    83,    84,    84,    86,    85,
      87,    85,    88,    89,    90,    91,    92,    92,    93,    93,
      93,    94,    95,    96,    96,    97,    98,    99,    99,   100,
     100,   101,   102,   102,   103,   103,   104
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     4,     1,     1,     2,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     4,     6,     8,    12,     1,     3,     4,     7,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     4,     4,     3,     1,
       1,     1,     1,     3,     1,     3,     4,     4,     1,     2,
       3,     2,     3,     2,     5,     3,     1,     2,     0,     5,
       0,     4,     4,     2,     8,     2,     5,     3,     2,     2,
       3,     1,     7,     2,     2,     6,     5,     2,     2,     1,
       3,     2,     3,     2,     3,     3,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: function_list mission_header statement_list TOK_LANDING TOK_SEMICOLON  */
#line 1319 "smpl_parser.y"
        {
            fprintf(output, "    return 0;\n}\n");
            tac_emit("return 0");
            tac_emit("end_program");
        }
#line 2910 "smpl_parser.tab.c"
    break;

  case 3: /* program: mission_header statement_list TOK_LANDING TOK_SEMICOLON  */
#line 1325 "smpl_parser.y"
        {
            fprintf(output, "    return 0;\n}\n");
            tac_emit("return 0");
            tac_emit("end_program");
        }
#line 2920 "smpl_parser.tab.c"
    break;

  case 4: /* mission_header: TOK_MISSION  */
#line 1334 "smpl_parser.y"
        {
            fprintf(output, "int main() {\n");
            indent_level = 1;
            tac_emit("begin_program");
        }
#line 2930 "smpl_parser.tab.c"
    break;

  case 24: /* block_open: TOK_LAUNCH  */
#line 1379 "smpl_parser.y"
        {
            fprintf(output, " {\n");
            indent_level++;
            enter_scope();
        }
#line 2940 "smpl_parser.tab.c"
    break;

  case 25: /* block: block_open statement_list TOK_ABORT  */
#line 1388 "smpl_parser.y"
        {
            leave_scope();
            indent_level--;
            print_indent();
            fprintf(output, "}\n");
        }
#line 2951 "smpl_parser.tab.c"
    break;

  case 26: /* data_type: TOK_CARGO_INT  */
#line 1401 "smpl_parser.y"
                       { (yyval.sval) = safe_strdup("int");    }
#line 2957 "smpl_parser.tab.c"
    break;

  case 27: /* data_type: TOK_CARGO_FLOAT  */
#line 1402 "smpl_parser.y"
                       { (yyval.sval) = safe_strdup("float");  }
#line 2963 "smpl_parser.tab.c"
    break;

  case 28: /* data_type: TOK_CARGO_CHAR  */
#line 1403 "smpl_parser.y"
                       { (yyval.sval) = safe_strdup("char");   }
#line 2969 "smpl_parser.tab.c"
    break;

  case 29: /* data_type: TOK_CARGO_DOUBLE  */
#line 1404 "smpl_parser.y"
                       { (yyval.sval) = safe_strdup("double"); }
#line 2975 "smpl_parser.tab.c"
    break;

  case 30: /* data_type: TOK_VOID_SPACE  */
#line 1405 "smpl_parser.y"
                       { (yyval.sval) = safe_strdup("void");   }
#line 2981 "smpl_parser.tab.c"
    break;

  case 31: /* declaration_stmt: TOK_LOAD data_type TOK_IDENTIFIER TOK_SEMICOLON  */
#line 1414 "smpl_parser.y"
        {
            add_symbol((yyvsp[-1].sval), (yyvsp[-2].sval), 0, 0);
            print_indent();
            fprintf(output, "%s %s;\n", (yyvsp[-2].sval), (yyvsp[-1].sval));
            tac_emit("declare %s %s", (yyvsp[-2].sval), (yyvsp[-1].sval));
            free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
        }
#line 2993 "smpl_parser.tab.c"
    break;

  case 32: /* declaration_stmt: TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON  */
#line 1422 "smpl_parser.y"
        {
            add_symbol((yyvsp[-3].sval), (yyvsp[-4].sval), 0, 0);
            check_type_compat((yyvsp[-3].sval), (yyvsp[-4].sval), (yyvsp[-1].typed).type);
            print_indent();
            fprintf(output, "%s %s = %s;\n", (yyvsp[-4].sval), (yyvsp[-3].sval), (yyvsp[-1].typed).code);
            tac_emit("%s = %s", (yyvsp[-3].sval), (yyvsp[-1].typed).tac);
            free((yyvsp[-4].sval)); free((yyvsp[-3].sval));
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
        }
#line 3007 "smpl_parser.tab.c"
    break;

  case 33: /* declaration_stmt: TOK_LOAD data_type TOK_CARGO_ARRAY TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_SEMICOLON  */
#line 1433 "smpl_parser.y"
        {
            add_symbol((yyvsp[-4].sval), (yyvsp[-6].sval), 0, 1);
            print_indent();
            fprintf(output, "%s %s[%d];\n", (yyvsp[-6].sval), (yyvsp[-4].sval), (yyvsp[-2].ival));
            tac_emit("declare_array %s %s[%d]", (yyvsp[-6].sval), (yyvsp[-4].sval), (yyvsp[-2].ival));
            free((yyvsp[-6].sval)); free((yyvsp[-4].sval));
        }
#line 3019 "smpl_parser.tab.c"
    break;

  case 34: /* declaration_stmt: TOK_LOAD data_type TOK_CARGO_ARRAY TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_STORE TOK_LBRACE init_list TOK_RBRACE TOK_SEMICOLON  */
#line 1443 "smpl_parser.y"
        {
            add_symbol((yyvsp[-8].sval), (yyvsp[-10].sval), 0, 1);
            print_indent();
            fprintf(output, "%s %s[%d] = {%s};\n", (yyvsp[-10].sval), (yyvsp[-8].sval), (yyvsp[-6].ival), (yyvsp[-2].sval));
            tac_emit("declare_array %s %s[%d] = {%s}", (yyvsp[-10].sval), (yyvsp[-8].sval), (yyvsp[-6].ival), (yyvsp[-2].sval));
            free((yyvsp[-10].sval)); free((yyvsp[-8].sval)); free((yyvsp[-2].sval));
        }
#line 3031 "smpl_parser.tab.c"
    break;

  case 35: /* init_list: expr  */
#line 1454 "smpl_parser.y"
        { (yyval.sval) = (yyvsp[0].typed).code; free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac); }
#line 3037 "smpl_parser.tab.c"
    break;

  case 36: /* init_list: init_list TOK_COMMA expr  */
#line 1456 "smpl_parser.y"
        {
            char *t = mkstr("%s, %s", (yyvsp[-2].sval), (yyvsp[0].typed).code);
            free((yyvsp[-2].sval)); free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
            (yyval.sval) = t;
        }
#line 3047 "smpl_parser.tab.c"
    break;

  case 37: /* assignment_stmt: TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON  */
#line 1469 "smpl_parser.y"
        {
            const char *vt = lookup_type((yyvsp[-3].sval));
            if (!vt)
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, (yyvsp[-3].sval));
            else
                check_type_compat((yyvsp[-3].sval), vt, (yyvsp[-1].typed).type);
            print_indent();
            fprintf(output, "%s = %s;\n", (yyvsp[-3].sval), (yyvsp[-1].typed).code);
            tac_emit("%s = %s", (yyvsp[-3].sval), (yyvsp[-1].typed).tac);
            free((yyvsp[-3].sval)); free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
        }
#line 3065 "smpl_parser.tab.c"
    break;

  case 38: /* assignment_stmt: TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET TOK_STORE expr TOK_SEMICOLON  */
#line 1483 "smpl_parser.y"
        {
            const char *vt = lookup_type((yyvsp[-6].sval));
            if (!vt)
                fprintf(stderr,
                    "Warning at line %d: Array '%s' used without declaration\n",
                    yylineno, (yyvsp[-6].sval));
            print_indent();
            fprintf(output, "%s[%s] = %s;\n", (yyvsp[-6].sval), (yyvsp[-4].typed).code, (yyvsp[-1].typed).code);
            tac_emit("%s[%s] = %s", (yyvsp[-6].sval), (yyvsp[-4].typed).tac, (yyvsp[-1].typed).tac);
            free((yyvsp[-6].sval));
            free((yyvsp[-4].typed).code); free((yyvsp[-4].typed).type); free((yyvsp[-4].typed).tac);
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
        }
#line 3083 "smpl_parser.tab.c"
    break;

  case 39: /* boost_stmt: TOK_BOOST TOK_IDENTIFIER TOK_SEMICOLON  */
#line 1500 "smpl_parser.y"
        {
            const char *vt = lookup_type((yyvsp[-1].sval));
            if (!vt)
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, (yyvsp[-1].sval));
            print_indent();
            fprintf(output, "%s++;\n", (yyvsp[-1].sval));
            tac_emit("%s = %s + 1", (yyvsp[-1].sval), (yyvsp[-1].sval));
            free((yyvsp[-1].sval));
        }
#line 3099 "smpl_parser.tab.c"
    break;

  case 40: /* degrade_stmt: TOK_DEGRADE TOK_IDENTIFIER TOK_SEMICOLON  */
#line 1515 "smpl_parser.y"
        {
            const char *vt = lookup_type((yyvsp[-1].sval));
            if (!vt)
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, (yyvsp[-1].sval));
            print_indent();
            fprintf(output, "%s--;\n", (yyvsp[-1].sval));
            tac_emit("%s = %s - 1", (yyvsp[-1].sval), (yyvsp[-1].sval));
            free((yyvsp[-1].sval));
        }
#line 3115 "smpl_parser.tab.c"
    break;

  case 41: /* expr: TOK_COMBINE expr expr  */
#line 1545 "smpl_parser.y"
        {
            char *r;
            if (try_fold_int((yyvsp[-1].typed).code, (yyvsp[0].typed).code, '+', &r)) {
                (yyval.typed).code = r; (yyval.typed).tac = strdup(r);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
                tac_emit("  ; fold %s + %s => %s", (yyvsp[-1].typed).tac, (yyvsp[0].typed).tac, r);
            } else if (try_algebraic_simplify((yyvsp[-1].typed).code, (yyvsp[0].typed).code, '+', &r)) {
                (yyval.typed).code = r; (yyval.typed).tac = strdup(r);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
            } else {
                (yyval.typed).code = mkstr("(%s + %s)", (yyvsp[-1].typed).code, (yyvsp[0].typed).code);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
                char *t = new_temp();
                tac_emit("%s = %s + %s", t, (yyvsp[-1].typed).tac, (yyvsp[0].typed).tac);
                (yyval.typed).tac = t;
            }
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3139 "smpl_parser.tab.c"
    break;

  case 42: /* expr: TOK_REDUCE expr expr  */
#line 1565 "smpl_parser.y"
        {
            char *r;
            if (try_fold_int((yyvsp[-1].typed).code, (yyvsp[0].typed).code, '-', &r)) {
                (yyval.typed).code = r; (yyval.typed).tac = strdup(r);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
                tac_emit("  ; fold %s - %s => %s", (yyvsp[-1].typed).tac, (yyvsp[0].typed).tac, r);
            } else if (try_algebraic_simplify((yyvsp[-1].typed).code, (yyvsp[0].typed).code, '-', &r)) {
                (yyval.typed).code = r; (yyval.typed).tac = strdup(r);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
            } else {
                (yyval.typed).code = mkstr("(%s - %s)", (yyvsp[-1].typed).code, (yyvsp[0].typed).code);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
                char *t = new_temp();
                tac_emit("%s = %s - %s", t, (yyvsp[-1].typed).tac, (yyvsp[0].typed).tac);
                (yyval.typed).tac = t;
            }
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3163 "smpl_parser.tab.c"
    break;

  case 43: /* expr: TOK_AMPLIFY expr expr  */
#line 1585 "smpl_parser.y"
        {
            char *r;
            if (try_fold_int((yyvsp[-1].typed).code, (yyvsp[0].typed).code, '*', &r)) {
                (yyval.typed).code = r; (yyval.typed).tac = strdup(r);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
                tac_emit("  ; fold %s * %s => %s", (yyvsp[-1].typed).tac, (yyvsp[0].typed).tac, r);
            } else if (try_strength_reduce((yyvsp[-1].typed).code, (yyvsp[0].typed).code,
                                            (yyvsp[-1].typed).type, (yyvsp[0].typed).type, '*', &r)) {
                (yyval.typed).code = r;
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
                char *t = new_temp();
                tac_emit("%s = %s  ; strength reduction", t, r);
                (yyval.typed).tac = t;
            } else if (try_algebraic_simplify((yyvsp[-1].typed).code, (yyvsp[0].typed).code, '*', &r)) {
                (yyval.typed).code = r; (yyval.typed).tac = strdup(r);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
            } else {
                (yyval.typed).code = mkstr("(%s * %s)", (yyvsp[-1].typed).code, (yyvsp[0].typed).code);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
                char *t = new_temp();
                tac_emit("%s = %s * %s", t, (yyvsp[-1].typed).tac, (yyvsp[0].typed).tac);
                (yyval.typed).tac = t;
            }
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3194 "smpl_parser.tab.c"
    break;

  case 44: /* expr: TOK_SPLIT expr expr  */
#line 1612 "smpl_parser.y"
        {
            char *r;
            if (try_fold_int((yyvsp[-1].typed).code, (yyvsp[0].typed).code, '/', &r)) {
                (yyval.typed).code = r; (yyval.typed).tac = strdup(r);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
                tac_emit("  ; fold %s / %s => %s", (yyvsp[-1].typed).tac, (yyvsp[0].typed).tac, r);
            } else if (try_strength_reduce((yyvsp[-1].typed).code, (yyvsp[0].typed).code,
                                            (yyvsp[-1].typed).type, (yyvsp[0].typed).type, '/', &r)) {
                (yyval.typed).code = r;
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
                char *t = new_temp();
                tac_emit("%s = %s  ; strength reduction", t, r);
                (yyval.typed).tac = t;
            } else if (try_algebraic_simplify((yyvsp[-1].typed).code, (yyvsp[0].typed).code, '/', &r)) {
                (yyval.typed).code = r; (yyval.typed).tac = strdup(r);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
            } else {
                (yyval.typed).code = mkstr("(%s / %s)", (yyvsp[-1].typed).code, (yyvsp[0].typed).code);
                (yyval.typed).type = safe_strdup(wider_type((yyvsp[-1].typed).type, (yyvsp[0].typed).type));
                char *t = new_temp();
                tac_emit("%s = %s / %s", t, (yyvsp[-1].typed).tac, (yyvsp[0].typed).tac);
                (yyval.typed).tac = t;
            }
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3225 "smpl_parser.tab.c"
    break;

  case 45: /* expr: TOK_REMAINDER expr expr  */
#line 1639 "smpl_parser.y"
        {
            char *r;
            if (try_fold_int((yyvsp[-1].typed).code, (yyvsp[0].typed).code, '%', &r)) {
                (yyval.typed).code = r; (yyval.typed).tac = strdup(r);
                (yyval.typed).type = safe_strdup("int");
                tac_emit("  ; fold %s %% %s => %s", (yyvsp[-1].typed).tac, (yyvsp[0].typed).tac, r);
            } else {
                (yyval.typed).code = mkstr("(%s %% %s)", (yyvsp[-1].typed).code, (yyvsp[0].typed).code);
                (yyval.typed).type = safe_strdup("int");
                char *t = new_temp();
                tac_emit("%s = %s %% %s", t, (yyvsp[-1].typed).tac, (yyvsp[0].typed).tac);
                (yyval.typed).tac = t;
            }
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3246 "smpl_parser.tab.c"
    break;

  case 46: /* expr: expr TOK_EXCEEDS expr  */
#line 1658 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("(%s > %s)", (yyvsp[-2].typed).code, (yyvsp[0].typed).code);
            (yyval.typed).type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s > %s", t, (yyvsp[-2].typed).tac, (yyvsp[0].typed).tac);
            (yyval.typed).tac = t;
            free((yyvsp[-2].typed).code); free((yyvsp[-2].typed).type); free((yyvsp[-2].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3260 "smpl_parser.tab.c"
    break;

  case 47: /* expr: expr TOK_BELOW expr  */
#line 1668 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("(%s < %s)", (yyvsp[-2].typed).code, (yyvsp[0].typed).code);
            (yyval.typed).type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s < %s", t, (yyvsp[-2].typed).tac, (yyvsp[0].typed).tac);
            (yyval.typed).tac = t;
            free((yyvsp[-2].typed).code); free((yyvsp[-2].typed).type); free((yyvsp[-2].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3274 "smpl_parser.tab.c"
    break;

  case 48: /* expr: expr TOK_EXCEEDS_OR_EQUAL expr  */
#line 1678 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("(%s >= %s)", (yyvsp[-2].typed).code, (yyvsp[0].typed).code);
            (yyval.typed).type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s >= %s", t, (yyvsp[-2].typed).tac, (yyvsp[0].typed).tac);
            (yyval.typed).tac = t;
            free((yyvsp[-2].typed).code); free((yyvsp[-2].typed).type); free((yyvsp[-2].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3288 "smpl_parser.tab.c"
    break;

  case 49: /* expr: expr TOK_BELOW_OR_EQUAL expr  */
#line 1688 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("(%s <= %s)", (yyvsp[-2].typed).code, (yyvsp[0].typed).code);
            (yyval.typed).type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s <= %s", t, (yyvsp[-2].typed).tac, (yyvsp[0].typed).tac);
            (yyval.typed).tac = t;
            free((yyvsp[-2].typed).code); free((yyvsp[-2].typed).type); free((yyvsp[-2].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3302 "smpl_parser.tab.c"
    break;

  case 50: /* expr: expr TOK_MATCHES expr  */
#line 1698 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("(%s == %s)", (yyvsp[-2].typed).code, (yyvsp[0].typed).code);
            (yyval.typed).type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s == %s", t, (yyvsp[-2].typed).tac, (yyvsp[0].typed).tac);
            (yyval.typed).tac = t;
            free((yyvsp[-2].typed).code); free((yyvsp[-2].typed).type); free((yyvsp[-2].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3316 "smpl_parser.tab.c"
    break;

  case 51: /* expr: expr TOK_DIFFERS expr  */
#line 1708 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("(%s != %s)", (yyvsp[-2].typed).code, (yyvsp[0].typed).code);
            (yyval.typed).type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s != %s", t, (yyvsp[-2].typed).tac, (yyvsp[0].typed).tac);
            (yyval.typed).tac = t;
            free((yyvsp[-2].typed).code); free((yyvsp[-2].typed).type); free((yyvsp[-2].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3330 "smpl_parser.tab.c"
    break;

  case 52: /* expr: expr TOK_BOTH expr  */
#line 1720 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("(%s && %s)", (yyvsp[-2].typed).code, (yyvsp[0].typed).code);
            (yyval.typed).type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s && %s", t, (yyvsp[-2].typed).tac, (yyvsp[0].typed).tac);
            (yyval.typed).tac = t;
            free((yyvsp[-2].typed).code); free((yyvsp[-2].typed).type); free((yyvsp[-2].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3344 "smpl_parser.tab.c"
    break;

  case 53: /* expr: expr TOK_EITHER expr  */
#line 1730 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("(%s || %s)", (yyvsp[-2].typed).code, (yyvsp[0].typed).code);
            (yyval.typed).type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s || %s", t, (yyvsp[-2].typed).tac, (yyvsp[0].typed).tac);
            (yyval.typed).tac = t;
            free((yyvsp[-2].typed).code); free((yyvsp[-2].typed).type); free((yyvsp[-2].typed).tac);
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3358 "smpl_parser.tab.c"
    break;

  case 54: /* expr: TOK_NEGATE expr  */
#line 1742 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("(!%s)", (yyvsp[0].typed).code);
            (yyval.typed).type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = !%s", t, (yyvsp[0].typed).tac);
            (yyval.typed).tac = t;
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3371 "smpl_parser.tab.c"
    break;

  case 55: /* expr: TOK_IDENTIFIER  */
#line 1753 "smpl_parser.y"
        {
            const char *t = lookup_type((yyvsp[0].sval));
            if (!t)
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, (yyvsp[0].sval));
            (yyval.typed).code = safe_strdup((yyvsp[0].sval));
            (yyval.typed).type = safe_strdup(t ? t : "int");
            (yyval.typed).tac  = safe_strdup((yyvsp[0].sval));
            free((yyvsp[0].sval));
        }
#line 3387 "smpl_parser.tab.c"
    break;

  case 56: /* expr: TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET  */
#line 1765 "smpl_parser.y"
        {
            const char *t = lookup_type((yyvsp[-3].sval));
            if (!t)
                fprintf(stderr,
                    "Warning at line %d: Array '%s' used without declaration\n",
                    yylineno, (yyvsp[-3].sval));
            (yyval.typed).code = mkstr("%s[%s]", (yyvsp[-3].sval), (yyvsp[-1].typed).code);
            (yyval.typed).type = safe_strdup(t ? t : "int");
            char *tmp = new_temp();
            tac_emit("%s = %s[%s]", tmp, (yyvsp[-3].sval), (yyvsp[-1].typed).tac);
            (yyval.typed).tac = tmp;
            free((yyvsp[-3].sval));
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
        }
#line 3406 "smpl_parser.tab.c"
    break;

  case 57: /* expr: TOK_IDENTIFIER TOK_LPAREN arg_list TOK_RPAREN  */
#line 1780 "smpl_parser.y"
        {
            const char *t = lookup_type((yyvsp[-3].sval));
            (yyval.typed).code = mkstr("%s(%s)", (yyvsp[-3].sval), (yyvsp[-1].sval));
            (yyval.typed).type = safe_strdup(t ? t : "int");
            char *tmp = new_temp();
            tac_emit("%s = call %s, %s", tmp, (yyvsp[-3].sval), (yyvsp[-1].sval));
            (yyval.typed).tac = tmp;
            free((yyvsp[-3].sval)); free((yyvsp[-1].sval));
        }
#line 3420 "smpl_parser.tab.c"
    break;

  case 58: /* expr: TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN  */
#line 1790 "smpl_parser.y"
        {
            const char *t = lookup_type((yyvsp[-2].sval));
            (yyval.typed).code = mkstr("%s()", (yyvsp[-2].sval));
            (yyval.typed).type = safe_strdup(t ? t : "int");
            char *tmp = new_temp();
            tac_emit("%s = call %s", tmp, (yyvsp[-2].sval));
            (yyval.typed).tac = tmp;
            free((yyvsp[-2].sval));
        }
#line 3434 "smpl_parser.tab.c"
    break;

  case 59: /* expr: TOK_INTEGER  */
#line 1800 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("%d", (yyvsp[0].ival));
            (yyval.typed).type = safe_strdup("int");
            (yyval.typed).tac  = mkstr("%d", (yyvsp[0].ival));
        }
#line 3444 "smpl_parser.tab.c"
    break;

  case 60: /* expr: TOK_FLOAT_NUM  */
#line 1806 "smpl_parser.y"
        {
            char buf[64];
            snprintf(buf, sizeof buf, "%g", (double)(yyvsp[0].fval));
            if (!strchr(buf, '.') && !strchr(buf, 'e'))
                strncat(buf, ".0", sizeof buf - strlen(buf) - 1);
            (yyval.typed).code = strdup(buf);
            (yyval.typed).type = safe_strdup("float");
            (yyval.typed).tac  = strdup(buf);
        }
#line 3458 "smpl_parser.tab.c"
    break;

  case 61: /* expr: TOK_CHAR_LITERAL  */
#line 1816 "smpl_parser.y"
        {
            (yyval.typed).code = safe_strdup((yyvsp[0].sval));
            (yyval.typed).type = safe_strdup("char");
            (yyval.typed).tac  = safe_strdup((yyvsp[0].sval));
            free((yyvsp[0].sval));
        }
#line 3469 "smpl_parser.tab.c"
    break;

  case 62: /* expr: TOK_STRING_LITERAL  */
#line 1823 "smpl_parser.y"
        {
            (yyval.typed).code = safe_strdup((yyvsp[0].sval));
            (yyval.typed).type = safe_strdup("string");
            (yyval.typed).tac  = safe_strdup((yyvsp[0].sval));
            free((yyvsp[0].sval));
        }
#line 3480 "smpl_parser.tab.c"
    break;

  case 63: /* expr: TOK_LPAREN expr TOK_RPAREN  */
#line 1830 "smpl_parser.y"
        {
            (yyval.typed).code = mkstr("(%s)", (yyvsp[-1].typed).code);
            (yyval.typed).type = safe_strdup((yyvsp[-1].typed).type);
            (yyval.typed).tac  = (yyvsp[-1].typed).tac;           /* reuse; don't free */
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type);
        }
#line 3491 "smpl_parser.tab.c"
    break;

  case 64: /* arg_list: expr  */
#line 1840 "smpl_parser.y"
        { (yyval.sval) = (yyvsp[0].typed).code; free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac); }
#line 3497 "smpl_parser.tab.c"
    break;

  case 65: /* arg_list: arg_list TOK_COMMA expr  */
#line 1842 "smpl_parser.y"
        {
            char *t = mkstr("%s, %s", (yyvsp[-2].sval), (yyvsp[0].typed).code);
            free((yyvsp[-2].sval)); free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
            (yyval.sval) = t;
        }
#line 3507 "smpl_parser.tab.c"
    break;

  case 66: /* if_header: TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN  */
#line 1855 "smpl_parser.y"
        {
            char *l_else = new_label();
            char *l_end  = new_label();
            tac_push(l_end);
            tac_push(l_else);
            tac_emit("if_false %s goto %s", (yyvsp[-1].typed).tac, l_else);
            print_indent();
            fprintf(output, "if (%s)", (yyvsp[-1].typed).code);
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
        }
#line 3522 "smpl_parser.tab.c"
    break;

  case 67: /* else_if_header: TOK_ELSE_CHECK TOK_LPAREN expr TOK_RPAREN  */
#line 1869 "smpl_parser.y"
        {
            /* close previous then-branch */
            char *l_prev = tac_pop();           /* old else label */
            char *l_end  = tac_pop();           /* end label      */
            tac_emit("goto %s", l_end);
            tac_emit("%s:", l_prev);
            free(l_prev);
            /* new else label for this branch */
            char *l_else = new_label();
            tac_push(l_end);
            tac_push(l_else);
            tac_emit("if_false %s goto %s", (yyvsp[-1].typed).tac, l_else);
            print_indent();
            fprintf(output, "else if (%s)", (yyvsp[-1].typed).code);
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
        }
#line 3543 "smpl_parser.tab.c"
    break;

  case 68: /* otherwise_header: TOK_OTHERWISE  */
#line 1889 "smpl_parser.y"
        {
            char *l_prev = tac_pop();
            char *l_end  = tac_pop();
            tac_emit("goto %s", l_end);
            tac_emit("%s:", l_prev);
            free(l_prev);
            tac_push(l_end);    /* still need end label */
            print_indent();
            fprintf(output, "else");
        }
#line 3558 "smpl_parser.tab.c"
    break;

  case 69: /* if_stmt: if_header block  */
#line 1903 "smpl_parser.y"
        {
            char *l_else = tac_pop();
            char *l_end  = tac_pop();
            tac_emit("%s:", l_else);
            tac_emit("%s:", l_end);
            free(l_else); free(l_end);
        }
#line 3570 "smpl_parser.tab.c"
    break;

  case 71: /* else_chain: else_if_header block  */
#line 1915 "smpl_parser.y"
        {
            char *l_else = tac_pop();
            char *l_end  = tac_pop();
            tac_emit("%s:", l_else);
            tac_emit("%s:", l_end);
            free(l_else); free(l_end);
        }
#line 3582 "smpl_parser.tab.c"
    break;

  case 73: /* else_chain: otherwise_header block  */
#line 1924 "smpl_parser.y"
        {
            char *l_end = tac_pop();
            tac_emit("%s:", l_end);
            free(l_end);
        }
#line 3592 "smpl_parser.tab.c"
    break;

  case 74: /* switch_header: TOK_PROTOCOL TOK_LPAREN expr TOK_RPAREN TOK_LAUNCH  */
#line 1937 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "switch (%s) {\n", (yyvsp[-2].typed).code);
            tac_emit("switch %s", (yyvsp[-2].typed).tac);
            free((yyvsp[-2].typed).code); free((yyvsp[-2].typed).type); free((yyvsp[-2].typed).tac);
            indent_level++;
            enter_scope();
        }
#line 3605 "smpl_parser.tab.c"
    break;

  case 75: /* switch_stmt: switch_header case_list TOK_ABORT  */
#line 1949 "smpl_parser.y"
        {
            leave_scope();
            indent_level--;
            print_indent();
            fprintf(output, "}\n");
            tac_emit("end_switch");
        }
#line 3617 "smpl_parser.tab.c"
    break;

  case 78: /* $@1: %empty  */
#line 1965 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "case %d:\n", (yyvsp[-1].ival));
            tac_emit("case %d:", (yyvsp[-1].ival));
            indent_level++;
        }
#line 3628 "smpl_parser.tab.c"
    break;

  case 79: /* case_item: TOK_SCENARIO TOK_INTEGER TOK_COLON $@1 statement_list  */
#line 1972 "smpl_parser.y"
        { indent_level--; }
#line 3634 "smpl_parser.tab.c"
    break;

  case 80: /* $@2: %empty  */
#line 1974 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "default:\n");
            tac_emit("default:");
            indent_level++;
        }
#line 3645 "smpl_parser.tab.c"
    break;

  case 81: /* case_item: TOK_DEFAULT_PROTOCOL TOK_COLON $@2 statement_list  */
#line 1981 "smpl_parser.y"
        { indent_level--; }
#line 3651 "smpl_parser.tab.c"
    break;

  case 82: /* while_header: TOK_ORBIT_WHILE TOK_LPAREN expr TOK_RPAREN  */
#line 1991 "smpl_parser.y"
        {
            char *l_start = new_label();
            char *l_end   = new_label();
            tac_push(l_end);
            tac_push(l_start);
            tac_emit("%s:", l_start);
            tac_emit("if_false %s goto %s", (yyvsp[-1].typed).tac, l_end);
            print_indent();
            fprintf(output, "while (%s)", (yyvsp[-1].typed).code);
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
        }
#line 3667 "smpl_parser.tab.c"
    break;

  case 83: /* while_loop: while_header block  */
#line 2006 "smpl_parser.y"
        {
            char *l_start = tac_pop();
            char *l_end   = tac_pop();
            tac_emit("goto %s", l_start);
            tac_emit("%s:", l_end);
            free(l_start); free(l_end);
        }
#line 3679 "smpl_parser.tab.c"
    break;

  case 84: /* for_header: TOK_SEQUENCE TOK_LPAREN for_init TOK_SEMICOLON expr TOK_SEMICOLON for_update TOK_RPAREN  */
#line 2018 "smpl_parser.y"
        {
            char *l_start = new_label();
            char *l_end   = new_label();
            tac_push(l_end);
            tac_push(l_start);
            tac_emit("%s  ; for init", (yyvsp[-5].sval));
            tac_emit("%s:", l_start);
            tac_emit("if_false %s goto %s", (yyvsp[-3].typed).tac, l_end);
            print_indent();
            fprintf(output, "for (%s; %s; %s)", (yyvsp[-5].sval), (yyvsp[-3].typed).code, (yyvsp[-1].sval));
            free((yyvsp[-5].sval)); free((yyvsp[-3].typed).code); free((yyvsp[-3].typed).type); free((yyvsp[-3].typed).tac); free((yyvsp[-1].sval));
        }
#line 3696 "smpl_parser.tab.c"
    break;

  case 85: /* for_loop: for_header block  */
#line 2034 "smpl_parser.y"
        {
            char *l_start = tac_pop();
            char *l_end   = tac_pop();
            tac_emit("goto %s", l_start);
            tac_emit("%s:", l_end);
            free(l_start); free(l_end);
        }
#line 3708 "smpl_parser.tab.c"
    break;

  case 86: /* for_init: TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr  */
#line 2045 "smpl_parser.y"
        {
            add_symbol((yyvsp[-2].sval), (yyvsp[-3].sval), 0, 0);
            check_type_compat((yyvsp[-2].sval), (yyvsp[-3].sval), (yyvsp[0].typed).type);
            (yyval.sval) = mkstr("%s %s = %s", (yyvsp[-3].sval), (yyvsp[-2].sval), (yyvsp[0].typed).code);
            free((yyvsp[-3].sval)); free((yyvsp[-2].sval));
            free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3720 "smpl_parser.tab.c"
    break;

  case 87: /* for_init: TOK_IDENTIFIER TOK_STORE expr  */
#line 2053 "smpl_parser.y"
        {
            const char *vt = lookup_type((yyvsp[-2].sval));
            if (!vt)
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, (yyvsp[-2].sval));
            else
                check_type_compat((yyvsp[-2].sval), vt, (yyvsp[0].typed).type);
            (yyval.sval) = mkstr("%s = %s", (yyvsp[-2].sval), (yyvsp[0].typed).code);
            free((yyvsp[-2].sval)); free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3736 "smpl_parser.tab.c"
    break;

  case 88: /* for_update: TOK_BOOST TOK_IDENTIFIER  */
#line 2068 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s++", (yyvsp[0].sval)); free((yyvsp[0].sval)); }
#line 3742 "smpl_parser.tab.c"
    break;

  case 89: /* for_update: TOK_DEGRADE TOK_IDENTIFIER  */
#line 2070 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s--", (yyvsp[0].sval)); free((yyvsp[0].sval)); }
#line 3748 "smpl_parser.tab.c"
    break;

  case 90: /* for_update: TOK_IDENTIFIER TOK_STORE expr  */
#line 2072 "smpl_parser.y"
        {
            (yyval.sval) = mkstr("%s = %s", (yyvsp[-2].sval), (yyvsp[0].typed).code);
            free((yyvsp[-2].sval)); free((yyvsp[0].typed).code); free((yyvsp[0].typed).type); free((yyvsp[0].typed).tac);
        }
#line 3757 "smpl_parser.tab.c"
    break;

  case 91: /* do_header: TOK_REPEAT  */
#line 2081 "smpl_parser.y"
        {
            char *l_start = new_label();
            tac_push(l_start);
            tac_emit("%s:", l_start);
            print_indent();
            fprintf(output, "do");
        }
#line 3769 "smpl_parser.tab.c"
    break;

  case 92: /* do_while_loop: do_header block TOK_UNTIL TOK_LPAREN expr TOK_RPAREN TOK_SEMICOLON  */
#line 2092 "smpl_parser.y"
        {
            char *l_start = tac_pop();
            tac_emit("if_true %s goto %s", (yyvsp[-2].typed).tac, l_start);
            free(l_start);
            print_indent();
            fprintf(output, "while (%s);\n", (yyvsp[-2].typed).code);
            free((yyvsp[-2].typed).code); free((yyvsp[-2].typed).type); free((yyvsp[-2].typed).tac);
        }
#line 3782 "smpl_parser.tab.c"
    break;

  case 93: /* loop_control_stmt: TOK_TERMINATE TOK_SEMICOLON  */
#line 2108 "smpl_parser.y"
        { print_indent(); fprintf(output, "break;\n");    tac_emit("break"); }
#line 3788 "smpl_parser.tab.c"
    break;

  case 94: /* loop_control_stmt: TOK_SKIP TOK_SEMICOLON  */
#line 2110 "smpl_parser.y"
        { print_indent(); fprintf(output, "continue;\n"); tac_emit("continue"); }
#line 3794 "smpl_parser.tab.c"
    break;

  case 95: /* func_header_params: TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN param_list TOK_RPAREN  */
#line 2119 "smpl_parser.y"
        {
            add_symbol((yyvsp[-3].sval), (yyvsp[-4].sval), 1, 0);
            fprintf(output, "%s %s(%s)", (yyvsp[-4].sval), (yyvsp[-3].sval), (yyvsp[-1].sval));
            tac_emit("function %s %s(%s)", (yyvsp[-4].sval), (yyvsp[-3].sval), (yyvsp[-1].sval));
            free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-1].sval));
        }
#line 3805 "smpl_parser.tab.c"
    break;

  case 96: /* func_header_void: TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN  */
#line 2129 "smpl_parser.y"
        {
            add_symbol((yyvsp[-2].sval), (yyvsp[-3].sval), 1, 0);
            fprintf(output, "%s %s(void)", (yyvsp[-3].sval), (yyvsp[-2].sval));
            tac_emit("function %s %s()", (yyvsp[-3].sval), (yyvsp[-2].sval));
            free((yyvsp[-3].sval)); free((yyvsp[-2].sval));
        }
#line 3816 "smpl_parser.tab.c"
    break;

  case 97: /* function_decl: func_header_params block  */
#line 2139 "smpl_parser.y"
        { fprintf(output, "\n"); tac_emit("end_function\n"); }
#line 3822 "smpl_parser.tab.c"
    break;

  case 98: /* function_decl: func_header_void block  */
#line 2141 "smpl_parser.y"
        { fprintf(output, "\n"); tac_emit("end_function\n"); }
#line 3828 "smpl_parser.tab.c"
    break;

  case 99: /* param_list: param  */
#line 2146 "smpl_parser.y"
        { (yyval.sval) = (yyvsp[0].sval); }
#line 3834 "smpl_parser.tab.c"
    break;

  case 100: /* param_list: param_list TOK_COMMA param  */
#line 2148 "smpl_parser.y"
        { char *t = mkstr("%s, %s", (yyvsp[-2].sval), (yyvsp[0].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].sval)); (yyval.sval) = t; }
#line 3840 "smpl_parser.tab.c"
    break;

  case 101: /* param: data_type TOK_IDENTIFIER  */
#line 2153 "smpl_parser.y"
        {
            add_symbol((yyvsp[0].sval), (yyvsp[-1].sval), 0, 0);
            (yyval.sval) = mkstr("%s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 3850 "smpl_parser.tab.c"
    break;

  case 102: /* return_stmt: TOK_RETURN_CARGO expr TOK_SEMICOLON  */
#line 2166 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "return %s;\n", (yyvsp[-1].typed).code);
            tac_emit("return %s", (yyvsp[-1].typed).tac);
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
        }
#line 3861 "smpl_parser.tab.c"
    break;

  case 103: /* return_stmt: TOK_RETURN_CARGO TOK_SEMICOLON  */
#line 2173 "smpl_parser.y"
        { print_indent(); fprintf(output, "return;\n"); tac_emit("return"); }
#line 3867 "smpl_parser.tab.c"
    break;

  case 104: /* io_stmt: TOK_TRANSMIT expr TOK_SEMICOLON  */
#line 2191 "smpl_parser.y"
        {
            print_indent();
            const char *e = (yyvsp[-1].typed).code;
            const char *tp = (yyvsp[-1].typed).type;
            if (strcmp(tp, "string") == 0 && e[0] == '"') {
                size_t len = strlen(e);
                if (len >= 2 && e[len-1] == '"') {
                    char *inner = strndup(e + 1, len - 2);
                    fprintf(output, "printf(\"%s\\n\");\n", inner);
                    free(inner);
                } else {
                    fprintf(output, "printf(%s);\n", e);
                }
            } else {
                const char *fmt = format_for_type(tp);
                fprintf(output, "printf(\"%s\\n\", %s);\n", fmt, e);
            }
            tac_emit("print %s", (yyvsp[-1].typed).tac);
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
        }
#line 3892 "smpl_parser.tab.c"
    break;

  case 105: /* io_stmt: TOK_RECEIVE TOK_IDENTIFIER TOK_SEMICOLON  */
#line 2212 "smpl_parser.y"
        {
            const char *vt = lookup_type((yyvsp[-1].sval));
            if (!vt) {
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, (yyvsp[-1].sval));
                vt = "int";
            }
            print_indent();
            const char *fmt = scanf_format_for_type(vt);
            fprintf(output, "scanf(\"%s\", &%s);\n", fmt, (yyvsp[-1].sval));
            tac_emit("read %s", (yyvsp[-1].sval));
            free((yyvsp[-1].sval));
        }
#line 3911 "smpl_parser.tab.c"
    break;

  case 106: /* expr_stmt: expr TOK_SEMICOLON  */
#line 2234 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "%s;\n", (yyvsp[-1].typed).code);
            tac_emit("%s", (yyvsp[-1].typed).tac);
            free((yyvsp[-1].typed).code); free((yyvsp[-1].typed).type); free((yyvsp[-1].typed).tac);
        }
#line 3922 "smpl_parser.tab.c"
    break;


#line 3926 "smpl_parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 2242 "smpl_parser.y"


/* ════════════════════════════════════════════════════════════════════════════
 * MAIN — Drives the compiler
 * ════════════════════════════════════════════════════════════════════════════ */

/* ════════════════════════════════════════════════════════════════════════════
 * AST DEMONSTRATION — Example of AST usage
 * ════════════════════════════════════════════════════════════════════════════ */

static void demonstrate_ast(void) {
    fprintf(stderr, "\n========================================\n");
    fprintf(stderr, "AST DEMONSTRATION\n");
    fprintf(stderr, "========================================\n\n");
    
    /* Create a simple AST example: int x = 5 + 3; */
    ASTNode *five = ast_int_literal(5);
    ASTNode *three = ast_int_literal(3);
    ASTNode *add_expr = ast_binary_op("+", five, three);
    ASTNode *decl = ast_declaration("int", "x", 0, NULL, add_expr);
    
    /* Show AST structure */
    fprintf(stderr, "1. AST Structure for: int x = 5 + 3;\n");
    fprintf(stderr, "-----------------------------------\n");
    print_ast(decl, 0);
    
    /* Perform type checking */
    fprintf(stderr, "\n2. Type Checking Pass:\n");
    fprintf(stderr, "-----------------------------------\n");
    ast_type_check(decl);
    fprintf(stderr, "Type checking completed successfully.\n");
    fprintf(stderr, "Expression type: %s\n", add_expr->expr_type ? add_expr->expr_type : "unknown");
    
    /* Generate TAC */
    fprintf(stderr, "\n3. TAC Generation Pass:\n");
    fprintf(stderr, "-----------------------------------\n");
    reset_temp();
    reset_label();
    
    /* For demonstration, use a temporary TAC output */
    FILE *saved_tac = tac_output;
    tac_output = stderr;
    
    ast_gen_tac(decl);
    
    tac_output = saved_tac;
    
    /* Generate C code */
    fprintf(stderr, "\n4. C Code Generation Pass:\n");
    fprintf(stderr, "-----------------------------------\n");
    FILE *saved_output = output;
    output = stderr;
    indent_level = 1;
    
    ast_gen_code(decl);
    
    output = saved_output;
    indent_level = 0;
    
    fprintf(stderr,  "\n========================================\n");
    fprintf(stderr, "AST demonstration completed\n");
    fprintf(stderr, "========================================\n\n");
    
    /* Clean up */
    free_ast(decl);
}

int main(int argc, char **argv) {

    /* Check for AST demonstration flag */
    if (argc >= 2 && strcmp(argv[1], "--ast-demo") == 0) {
        /* Initialize dummy output for demonstration */
        output = fopen("nul", "w");  /* Windows null device */
        if (!output) output = fopen("/dev/null", "w");  /* Unix/Linux */
        if (!output) output = stdout;
        
        demonstrate_ast();
        
        if (output != stdout) fclose(output);
        return 0;
    }

    if (argc < 2) {
        printf(
            "SMPL Compiler — Space Mission Programming Language\n"
            "Author : MD. Jahid Hasan Jim | Roll: 2107054 | KUET CSE\n\n"
            "Usage  : %s <input.smpl> [output.c]\n"
            "         %s --ast-demo           (show AST demonstration)\n"
            "\n"
            "  input.smpl  — SMPL source file\n"
            "  output.c    — (optional) destination C file; default: stdout\n"
            "  --ast-demo  — demonstrate Abstract Syntax Tree functionality\n",
            argv[0], argv[0]);
        return 1;
    }

    /* Open SMPL source */
    FILE *src = fopen(argv[1], "r");
    if (!src) {
        fprintf(stderr, "Error: Cannot open '%s'\n", argv[1]);
        return 1;
    }

    /* Open C output */
    if (argc >= 3) {
        output = fopen(argv[2], "w");
        if (!output) {
            fprintf(stderr, "Error: Cannot create '%s'\n", argv[2]);
            fclose(src);
            return 1;
        }
        /* Create TAC output file alongside the C output */
        char tac_path[1024];
        strncpy(tac_path, argv[2], sizeof(tac_path) - 6);
        tac_path[sizeof(tac_path) - 6] = '\0';
        char *dot = strrchr(tac_path, '.');
        if (dot) strcpy(dot, ".tac");
        else     strcat(tac_path, ".tac");
        tac_output = fopen(tac_path, "w");
        if (tac_output) {
            fprintf(tac_output,
                "; ==========================================\n"
                "; Three-Address Code (TAC)\n"
                "; Intermediate Representation\n"
                "; Generated by SMPL Compiler\n"
                "; Source: %s\n"
                "; ==========================================\n\n",
                argv[1]);
        }
    } else {
        output = stdout;
        tac_output = NULL;
    }

    token_output = NULL;

    fprintf(output, "#include <stdio.h>\n");

    yyin = src;
    int rc = yyparse();

    fclose(src);
    if (output != stdout) fclose(output);
    if (tac_output) fclose(tac_output);

    if (rc == 0) {
        fprintf(stderr, "Compilation successful.\n");
        if (argc >= 3)
            fprintf(stderr, "  C output  : %s\n", argv[2]);
    } else {
        fprintf(stderr, "Compilation failed — see errors above.\n");
    }

    return rc;
}
