%{
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

static void reset_temp(void)  { temp_count = 0;  }
static void reset_label(void) { label_count = 0; }

static const char *map_smpl_to_c_type(const char *smpl_type) {
    /* SMPL types already map directly to C types */
    if (!smpl_type) return "int";
    return smpl_type;
}

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

%}

/* ════════════════════════════════════════════════════════════════════════════
 * BISON DECLARATIONS
 * ════════════════════════════════════════════════════════════════════════════ */

%define parse.error verbose

/* Semantic value union */
%union {
    int   ival;
    float fval;
    char *sval;
    struct { char *code; char *type; char *tac; } typed;
    struct ASTNode *ast;  /* AST node pointer */
}

/* ── Tokens ─────────────────────────────────────────────────────────────── */

/* Program structure */
%token TOK_MISSION TOK_LANDING TOK_LAUNCH TOK_ABORT

/* Data types */
%token TOK_CARGO_INT TOK_CARGO_FLOAT TOK_CARGO_CHAR TOK_CARGO_DOUBLE TOK_VOID_SPACE

/* Declaration / assignment */
%token TOK_LOAD TOK_STORE TOK_CARGO_ARRAY

/* Arithmetic (prefix in SMPL) */
%token TOK_COMBINE TOK_REDUCE TOK_AMPLIFY TOK_SPLIT TOK_REMAINDER
%token TOK_BOOST TOK_DEGRADE

/* Relational (infix in SMPL) */
%token TOK_EXCEEDS TOK_BELOW TOK_EXCEEDS_OR_EQUAL TOK_BELOW_OR_EQUAL
%token TOK_MATCHES TOK_DIFFERS

/* Logical (infix / prefix) */
%token TOK_BOTH TOK_EITHER TOK_NEGATE

/* Conditionals */
%token TOK_CHECK_IF TOK_ELSE_CHECK TOK_OTHERWISE

/* Switch-case */
%token TOK_PROTOCOL TOK_SCENARIO TOK_DEFAULT_PROTOCOL

/* Loops */
%token TOK_ORBIT_WHILE TOK_SEQUENCE TOK_REPEAT TOK_UNTIL

/* Loop control */
%token TOK_TERMINATE TOK_SKIP

/* Functions */
%token TOK_FUNCTION TOK_RETURN_CARGO

/* I/O */
%token TOK_RECEIVE TOK_TRANSMIT

/* Literals & identifiers (carry semantic values) */
%token <ival> TOK_INTEGER
%token <fval> TOK_FLOAT_NUM
%token <sval> TOK_CHAR_LITERAL TOK_STRING_LITERAL TOK_IDENTIFIER

/* Punctuation */
%token TOK_SEMICOLON TOK_COLON TOK_COMMA
%token TOK_LPAREN TOK_RPAREN
%token TOK_LBRACKET TOK_RBRACKET
%token TOK_LBRACE TOK_RBRACE

/* ── Non-terminal type annotations ─────────────────────────────────────── */
%type <typed> expr
%type <sval>  data_type for_init for_update param param_list arg_list init_list
%type <ast>   ast_program ast_stmt ast_stmt_list ast_expr ast_declaration

/* ── Operator precedence (lowest → highest) ─────────────────────────────── */
%left  TOK_EITHER
%left  TOK_BOTH
%left  TOK_MATCHES TOK_DIFFERS
%left  TOK_EXCEEDS TOK_BELOW TOK_EXCEEDS_OR_EQUAL TOK_BELOW_OR_EQUAL
%left  TOK_COMBINE TOK_REDUCE
%left  TOK_AMPLIFY TOK_SPLIT TOK_REMAINDER
%right TOK_NEGATE

%%

/* ════════════════════════════════════════════════════════════════════════════
 * TOP-LEVEL PROGRAM
 * ════════════════════════════════════════════════════════════════════════════ */

program
    : function_list mission_header statement_list TOK_LANDING TOK_SEMICOLON
        {
            fprintf(output, "    return 0;\n}\n");
            tac_emit("return 0");
            tac_emit("end_program");
        }
    | mission_header statement_list TOK_LANDING TOK_SEMICOLON
        {
            fprintf(output, "    return 0;\n}\n");
            tac_emit("return 0");
            tac_emit("end_program");
        }
    ;

mission_header
    : TOK_MISSION
        {
            fprintf(output, "int main() {\n");
            indent_level = 1;
            tac_emit("begin_program");
        }
    ;

function_list
    : function_decl
    | function_list function_decl
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * STATEMENT LIST
 * ════════════════════════════════════════════════════════════════════════════ */

statement_list
    : /* empty */
    | statement_list statement
    ;

statement
    : declaration_stmt
    | assignment_stmt
    | boost_stmt
    | degrade_stmt
    | if_stmt
    | switch_stmt
    | while_loop
    | for_loop
    | do_while_loop
    | io_stmt
    | return_stmt
    | loop_control_stmt
    | function_decl
    | block
    | expr_stmt
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * BLOCK  →  LAUNCH statement_list ABORT
 * ════════════════════════════════════════════════════════════════════════════ */

block_open
    : TOK_LAUNCH
        {
            fprintf(output, " {\n");
            indent_level++;
            enter_scope();
        }
    ;

block
    : block_open statement_list TOK_ABORT
        {
            leave_scope();
            indent_level--;
            print_indent();
            fprintf(output, "}\n");
        }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * DATA TYPE
 * ════════════════════════════════════════════════════════════════════════════ */

data_type
    : TOK_CARGO_INT    { $$ = safe_strdup("int");    }
    | TOK_CARGO_FLOAT  { $$ = safe_strdup("float");  }
    | TOK_CARGO_CHAR   { $$ = safe_strdup("char");   }
    | TOK_CARGO_DOUBLE { $$ = safe_strdup("double"); }
    | TOK_VOID_SPACE   { $$ = safe_strdup("void");   }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * DECLARATIONS
 * ════════════════════════════════════════════════════════════════════════════ */

declaration_stmt
    : TOK_LOAD data_type TOK_IDENTIFIER TOK_SEMICOLON
        {
            add_symbol($3, $2, 0, 0);
            print_indent();
            fprintf(output, "%s %s;\n", $2, $3);
            tac_emit("declare %s %s", $2, $3);
            free($2); free($3);
        }
    | TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON
        {
            add_symbol($3, $2, 0, 0);
            check_type_compat($3, $2, $5.type);
            print_indent();
            fprintf(output, "%s %s = %s;\n", $2, $3, $5.code);
            tac_emit("%s = %s", $3, $5.tac);
            free($2); free($3);
            free($5.code); free($5.type); free($5.tac);
        }
    | TOK_LOAD data_type TOK_CARGO_ARRAY TOK_IDENTIFIER
      TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_SEMICOLON
        {
            add_symbol($4, $2, 0, 1);
            print_indent();
            fprintf(output, "%s %s[%d];\n", $2, $4, $6);
            tac_emit("declare_array %s %s[%d]", $2, $4, $6);
            free($2); free($4);
        }
    | TOK_LOAD data_type TOK_CARGO_ARRAY TOK_IDENTIFIER
      TOK_LBRACKET TOK_INTEGER TOK_RBRACKET
      TOK_STORE TOK_LBRACE init_list TOK_RBRACE TOK_SEMICOLON
        {
            add_symbol($4, $2, 0, 1);
            print_indent();
            fprintf(output, "%s %s[%d] = {%s};\n", $2, $4, $6, $10);
            tac_emit("declare_array %s %s[%d] = {%s}", $2, $4, $6, $10);
            free($2); free($4); free($10);
        }
    ;

init_list
    : expr
        { $$ = $1.code; free($1.type); free($1.tac); }
    | init_list TOK_COMMA expr
        {
            char *t = mkstr("%s, %s", $1, $3.code);
            free($1); free($3.code); free($3.type); free($3.tac);
            $$ = t;
        }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * ASSIGNMENT
 * ════════════════════════════════════════════════════════════════════════════ */

assignment_stmt
    : TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON
        {
            const char *vt = lookup_type($1);
            if (!vt)
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, $1);
            else
                check_type_compat($1, vt, $3.type);
            print_indent();
            fprintf(output, "%s = %s;\n", $1, $3.code);
            tac_emit("%s = %s", $1, $3.tac);
            free($1); free($3.code); free($3.type); free($3.tac);
        }
    | TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET TOK_STORE expr TOK_SEMICOLON
        {
            const char *vt = lookup_type($1);
            if (!vt)
                fprintf(stderr,
                    "Warning at line %d: Array '%s' used without declaration\n",
                    yylineno, $1);
            print_indent();
            fprintf(output, "%s[%s] = %s;\n", $1, $3.code, $6.code);
            tac_emit("%s[%s] = %s", $1, $3.tac, $6.tac);
            free($1);
            free($3.code); free($3.type); free($3.tac);
            free($6.code); free($6.type); free($6.tac);
        }
    ;

boost_stmt
    : TOK_BOOST TOK_IDENTIFIER TOK_SEMICOLON
        {
            const char *vt = lookup_type($2);
            if (!vt)
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, $2);
            print_indent();
            fprintf(output, "%s++;\n", $2);
            tac_emit("%s = %s + 1", $2, $2);
            free($2);
        }
    ;

degrade_stmt
    : TOK_DEGRADE TOK_IDENTIFIER TOK_SEMICOLON
        {
            const char *vt = lookup_type($2);
            if (!vt)
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, $2);
            print_indent();
            fprintf(output, "%s--;\n", $2);
            tac_emit("%s = %s - 1", $2, $2);
            free($2);
        }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * EXPRESSIONS
 *
 * Arithmetic operators are PREFIX:  COMBINE a b  →  (a + b)
 * Relational / logical are INFIX:   a EXCEEDS b  →  (a > b)
 *
 * Each rule propagates:
 *   .code  —  C source fragment
 *   .type  —  result type string
 *   .tac   —  TAC temp / literal holding the value
 *
 * Optimisation passes run on prefix arithmetic (fold → strength → algebra).
 * ════════════════════════════════════════════════════════════════════════════ */

expr
    /* ── Prefix arithmetic ──────────────────────────────────────────────── */
    : TOK_COMBINE expr expr
        {
            char *r;
            if (try_fold_int($2.code, $3.code, '+', &r)) {
                $$.code = r; $$.tac = strdup(r);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
                tac_emit("  ; fold %s + %s => %s", $2.tac, $3.tac, r);
            } else if (try_algebraic_simplify($2.code, $3.code, '+', &r)) {
                $$.code = r; $$.tac = strdup(r);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
            } else {
                $$.code = mkstr("(%s + %s)", $2.code, $3.code);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
                char *t = new_temp();
                tac_emit("%s = %s + %s", t, $2.tac, $3.tac);
                $$.tac = t;
            }
            free($2.code); free($2.type); free($2.tac);
            free($3.code); free($3.type); free($3.tac);
        }
    | TOK_REDUCE expr expr
        {
            char *r;
            if (try_fold_int($2.code, $3.code, '-', &r)) {
                $$.code = r; $$.tac = strdup(r);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
                tac_emit("  ; fold %s - %s => %s", $2.tac, $3.tac, r);
            } else if (try_algebraic_simplify($2.code, $3.code, '-', &r)) {
                $$.code = r; $$.tac = strdup(r);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
            } else {
                $$.code = mkstr("(%s - %s)", $2.code, $3.code);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
                char *t = new_temp();
                tac_emit("%s = %s - %s", t, $2.tac, $3.tac);
                $$.tac = t;
            }
            free($2.code); free($2.type); free($2.tac);
            free($3.code); free($3.type); free($3.tac);
        }
    | TOK_AMPLIFY expr expr
        {
            char *r;
            if (try_fold_int($2.code, $3.code, '*', &r)) {
                $$.code = r; $$.tac = strdup(r);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
                tac_emit("  ; fold %s * %s => %s", $2.tac, $3.tac, r);
            } else if (try_strength_reduce($2.code, $3.code,
                                            $2.type, $3.type, '*', &r)) {
                $$.code = r;
                $$.type = safe_strdup(wider_type($2.type, $3.type));
                char *t = new_temp();
                tac_emit("%s = %s  ; strength reduction", t, r);
                $$.tac = t;
            } else if (try_algebraic_simplify($2.code, $3.code, '*', &r)) {
                $$.code = r; $$.tac = strdup(r);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
            } else {
                $$.code = mkstr("(%s * %s)", $2.code, $3.code);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
                char *t = new_temp();
                tac_emit("%s = %s * %s", t, $2.tac, $3.tac);
                $$.tac = t;
            }
            free($2.code); free($2.type); free($2.tac);
            free($3.code); free($3.type); free($3.tac);
        }
    | TOK_SPLIT expr expr
        {
            char *r;
            if (try_fold_int($2.code, $3.code, '/', &r)) {
                $$.code = r; $$.tac = strdup(r);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
                tac_emit("  ; fold %s / %s => %s", $2.tac, $3.tac, r);
            } else if (try_strength_reduce($2.code, $3.code,
                                            $2.type, $3.type, '/', &r)) {
                $$.code = r;
                $$.type = safe_strdup(wider_type($2.type, $3.type));
                char *t = new_temp();
                tac_emit("%s = %s  ; strength reduction", t, r);
                $$.tac = t;
            } else if (try_algebraic_simplify($2.code, $3.code, '/', &r)) {
                $$.code = r; $$.tac = strdup(r);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
            } else {
                $$.code = mkstr("(%s / %s)", $2.code, $3.code);
                $$.type = safe_strdup(wider_type($2.type, $3.type));
                char *t = new_temp();
                tac_emit("%s = %s / %s", t, $2.tac, $3.tac);
                $$.tac = t;
            }
            free($2.code); free($2.type); free($2.tac);
            free($3.code); free($3.type); free($3.tac);
        }
    | TOK_REMAINDER expr expr
        {
            char *r;
            if (try_fold_int($2.code, $3.code, '%', &r)) {
                $$.code = r; $$.tac = strdup(r);
                $$.type = safe_strdup("int");
                tac_emit("  ; fold %s %% %s => %s", $2.tac, $3.tac, r);
            } else {
                $$.code = mkstr("(%s %% %s)", $2.code, $3.code);
                $$.type = safe_strdup("int");
                char *t = new_temp();
                tac_emit("%s = %s %% %s", t, $2.tac, $3.tac);
                $$.tac = t;
            }
            free($2.code); free($2.type); free($2.tac);
            free($3.code); free($3.type); free($3.tac);
        }

    /* ── Infix relational ─────────────────────────────────────────────── */
    | expr TOK_EXCEEDS expr
        {
            $$.code = mkstr("(%s > %s)", $1.code, $3.code);
            $$.type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s > %s", t, $1.tac, $3.tac);
            $$.tac = t;
            free($1.code); free($1.type); free($1.tac);
            free($3.code); free($3.type); free($3.tac);
        }
    | expr TOK_BELOW expr
        {
            $$.code = mkstr("(%s < %s)", $1.code, $3.code);
            $$.type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s < %s", t, $1.tac, $3.tac);
            $$.tac = t;
            free($1.code); free($1.type); free($1.tac);
            free($3.code); free($3.type); free($3.tac);
        }
    | expr TOK_EXCEEDS_OR_EQUAL expr
        {
            $$.code = mkstr("(%s >= %s)", $1.code, $3.code);
            $$.type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s >= %s", t, $1.tac, $3.tac);
            $$.tac = t;
            free($1.code); free($1.type); free($1.tac);
            free($3.code); free($3.type); free($3.tac);
        }
    | expr TOK_BELOW_OR_EQUAL expr
        {
            $$.code = mkstr("(%s <= %s)", $1.code, $3.code);
            $$.type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s <= %s", t, $1.tac, $3.tac);
            $$.tac = t;
            free($1.code); free($1.type); free($1.tac);
            free($3.code); free($3.type); free($3.tac);
        }
    | expr TOK_MATCHES expr
        {
            $$.code = mkstr("(%s == %s)", $1.code, $3.code);
            $$.type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s == %s", t, $1.tac, $3.tac);
            $$.tac = t;
            free($1.code); free($1.type); free($1.tac);
            free($3.code); free($3.type); free($3.tac);
        }
    | expr TOK_DIFFERS expr
        {
            $$.code = mkstr("(%s != %s)", $1.code, $3.code);
            $$.type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s != %s", t, $1.tac, $3.tac);
            $$.tac = t;
            free($1.code); free($1.type); free($1.tac);
            free($3.code); free($3.type); free($3.tac);
        }

    /* ── Infix logical ────────────────────────────────────────────────── */
    | expr TOK_BOTH expr
        {
            $$.code = mkstr("(%s && %s)", $1.code, $3.code);
            $$.type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s && %s", t, $1.tac, $3.tac);
            $$.tac = t;
            free($1.code); free($1.type); free($1.tac);
            free($3.code); free($3.type); free($3.tac);
        }
    | expr TOK_EITHER expr
        {
            $$.code = mkstr("(%s || %s)", $1.code, $3.code);
            $$.type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = %s || %s", t, $1.tac, $3.tac);
            $$.tac = t;
            free($1.code); free($1.type); free($1.tac);
            free($3.code); free($3.type); free($3.tac);
        }

    /* ── Unary ────────────────────────────────────────────────────────── */
    | TOK_NEGATE expr
        {
            $$.code = mkstr("(!%s)", $2.code);
            $$.type = safe_strdup("int");
            char *t = new_temp();
            tac_emit("%s = !%s", t, $2.tac);
            $$.tac = t;
            free($2.code); free($2.type); free($2.tac);
        }

    /* ── Primary atoms ────────────────────────────────────────────────── */
    | TOK_IDENTIFIER
        {
            const char *t = lookup_type($1);
            if (!t)
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, $1);
            $$.code = safe_strdup($1);
            $$.type = safe_strdup(t ? t : "int");
            $$.tac  = safe_strdup($1);
            free($1);
        }
    | TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET
        {
            const char *t = lookup_type($1);
            if (!t)
                fprintf(stderr,
                    "Warning at line %d: Array '%s' used without declaration\n",
                    yylineno, $1);
            $$.code = mkstr("%s[%s]", $1, $3.code);
            $$.type = safe_strdup(t ? t : "int");
            char *tmp = new_temp();
            tac_emit("%s = %s[%s]", tmp, $1, $3.tac);
            $$.tac = tmp;
            free($1);
            free($3.code); free($3.type); free($3.tac);
        }
    | TOK_IDENTIFIER TOK_LPAREN arg_list TOK_RPAREN
        {
            const char *t = lookup_type($1);
            $$.code = mkstr("%s(%s)", $1, $3);
            $$.type = safe_strdup(t ? t : "int");
            char *tmp = new_temp();
            tac_emit("%s = call %s, %s", tmp, $1, $3);
            $$.tac = tmp;
            free($1); free($3);
        }
    | TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN
        {
            const char *t = lookup_type($1);
            $$.code = mkstr("%s()", $1);
            $$.type = safe_strdup(t ? t : "int");
            char *tmp = new_temp();
            tac_emit("%s = call %s", tmp, $1);
            $$.tac = tmp;
            free($1);
        }
    | TOK_INTEGER
        {
            $$.code = mkstr("%d", $1);
            $$.type = safe_strdup("int");
            $$.tac  = mkstr("%d", $1);
        }
    | TOK_FLOAT_NUM
        {
            char buf[64];
            snprintf(buf, sizeof buf, "%g", (double)$1);
            if (!strchr(buf, '.') && !strchr(buf, 'e'))
                strncat(buf, ".0", sizeof buf - strlen(buf) - 1);
            $$.code = strdup(buf);
            $$.type = safe_strdup("float");
            $$.tac  = strdup(buf);
        }
    | TOK_CHAR_LITERAL
        {
            $$.code = safe_strdup($1);
            $$.type = safe_strdup("char");
            $$.tac  = safe_strdup($1);
            free($1);
        }
    | TOK_STRING_LITERAL
        {
            $$.code = safe_strdup($1);
            $$.type = safe_strdup("string");
            $$.tac  = safe_strdup($1);
            free($1);
        }
    | TOK_LPAREN expr TOK_RPAREN
        {
            $$.code = mkstr("(%s)", $2.code);
            $$.type = safe_strdup($2.type);
            $$.tac  = $2.tac;           /* reuse; don't free */
            free($2.code); free($2.type);
        }
    ;

arg_list
    : expr
        { $$ = $1.code; free($1.type); free($1.tac); }
    | arg_list TOK_COMMA expr
        {
            char *t = mkstr("%s, %s", $1, $3.code);
            free($1); free($3.code); free($3.type); free($3.tac);
            $$ = t;
        }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * CONDITIONAL STATEMENTS
 * ════════════════════════════════════════════════════════════════════════════ */

if_header
    : TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN
        {
            char *l_else = new_label();
            char *l_end  = new_label();
            tac_push(l_end);
            tac_push(l_else);
            tac_emit("if_false %s goto %s", $3.tac, l_else);
            print_indent();
            fprintf(output, "if (%s)", $3.code);
            free($3.code); free($3.type); free($3.tac);
        }
    ;

else_if_header
    : TOK_ELSE_CHECK TOK_LPAREN expr TOK_RPAREN
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
            tac_emit("if_false %s goto %s", $3.tac, l_else);
            print_indent();
            fprintf(output, "else if (%s)", $3.code);
            free($3.code); free($3.type); free($3.tac);
        }
    ;

otherwise_header
    : TOK_OTHERWISE
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
    ;

if_stmt
    : if_header block
        {
            char *l_else = tac_pop();
            char *l_end  = tac_pop();
            tac_emit("%s:", l_else);
            tac_emit("%s:", l_end);
            free(l_else); free(l_end);
        }
    | if_header block else_chain
    ;

else_chain
    : else_if_header block
        {
            char *l_else = tac_pop();
            char *l_end  = tac_pop();
            tac_emit("%s:", l_else);
            tac_emit("%s:", l_end);
            free(l_else); free(l_end);
        }
    | else_if_header block else_chain
    | otherwise_header block
        {
            char *l_end = tac_pop();
            tac_emit("%s:", l_end);
            free(l_end);
        }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * SWITCH STATEMENT
 * ════════════════════════════════════════════════════════════════════════════ */

switch_header
    : TOK_PROTOCOL TOK_LPAREN expr TOK_RPAREN TOK_LAUNCH
        {
            print_indent();
            fprintf(output, "switch (%s) {\n", $3.code);
            tac_emit("switch %s", $3.tac);
            free($3.code); free($3.type); free($3.tac);
            indent_level++;
            enter_scope();
        }
    ;

switch_stmt
    : switch_header case_list TOK_ABORT
        {
            leave_scope();
            indent_level--;
            print_indent();
            fprintf(output, "}\n");
            tac_emit("end_switch");
        }
    ;

case_list
    : case_item
    | case_list case_item
    ;

case_item
    : TOK_SCENARIO TOK_INTEGER TOK_COLON
        {
            print_indent();
            fprintf(output, "case %d:\n", $2);
            tac_emit("case %d:", $2);
            indent_level++;
        }
      statement_list
        { indent_level--; }
    | TOK_DEFAULT_PROTOCOL TOK_COLON
        {
            print_indent();
            fprintf(output, "default:\n");
            tac_emit("default:");
            indent_level++;
        }
      statement_list
        { indent_level--; }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * LOOP STATEMENTS
 * ════════════════════════════════════════════════════════════════════════════ */

/* ── ORBIT_WHILE ─────────────────────────────────────────────────────────── */
while_header
    : TOK_ORBIT_WHILE TOK_LPAREN expr TOK_RPAREN
        {
            char *l_start = new_label();
            char *l_end   = new_label();
            tac_push(l_end);
            tac_push(l_start);
            tac_emit("%s:", l_start);
            tac_emit("if_false %s goto %s", $3.tac, l_end);
            print_indent();
            fprintf(output, "while (%s)", $3.code);
            free($3.code); free($3.type); free($3.tac);
        }
    ;

while_loop
    : while_header block
        {
            char *l_start = tac_pop();
            char *l_end   = tac_pop();
            tac_emit("goto %s", l_start);
            tac_emit("%s:", l_end);
            free(l_start); free(l_end);
        }
    ;

/* ── SEQUENCE (for loop) ─────────────────────────────────────────────────── */
for_header
    : TOK_SEQUENCE TOK_LPAREN for_init TOK_SEMICOLON expr TOK_SEMICOLON for_update TOK_RPAREN
        {
            char *l_start = new_label();
            char *l_end   = new_label();
            tac_push(l_end);
            tac_push(l_start);
            tac_emit("%s  ; for init", $3);
            tac_emit("%s:", l_start);
            tac_emit("if_false %s goto %s", $5.tac, l_end);
            print_indent();
            fprintf(output, "for (%s; %s; %s)", $3, $5.code, $7);
            free($3); free($5.code); free($5.type); free($5.tac); free($7);
        }
    ;

for_loop
    : for_header block
        {
            char *l_start = tac_pop();
            char *l_end   = tac_pop();
            tac_emit("goto %s", l_start);
            tac_emit("%s:", l_end);
            free(l_start); free(l_end);
        }
    ;

for_init
    : TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr
        {
            add_symbol($3, $2, 0, 0);
            check_type_compat($3, $2, $5.type);
            $$ = mkstr("%s %s = %s", $2, $3, $5.code);
            free($2); free($3);
            free($5.code); free($5.type); free($5.tac);
        }
    | TOK_IDENTIFIER TOK_STORE expr
        {
            const char *vt = lookup_type($1);
            if (!vt)
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, $1);
            else
                check_type_compat($1, vt, $3.type);
            $$ = mkstr("%s = %s", $1, $3.code);
            free($1); free($3.code); free($3.type); free($3.tac);
        }
    ;

for_update
    : TOK_BOOST TOK_IDENTIFIER
        { $$ = mkstr("%s++", $2); free($2); }
    | TOK_DEGRADE TOK_IDENTIFIER
        { $$ = mkstr("%s--", $2); free($2); }
    | TOK_IDENTIFIER TOK_STORE expr
        {
            $$ = mkstr("%s = %s", $1, $3.code);
            free($1); free($3.code); free($3.type); free($3.tac);
        }
    ;

/* ── REPEAT … UNTIL (do-while) ──────────────────────────────────────────── */
do_header
    : TOK_REPEAT
        {
            char *l_start = new_label();
            tac_push(l_start);
            tac_emit("%s:", l_start);
            print_indent();
            fprintf(output, "do");
        }
    ;

do_while_loop
    : do_header block TOK_UNTIL TOK_LPAREN expr TOK_RPAREN TOK_SEMICOLON
        {
            char *l_start = tac_pop();
            tac_emit("if_true %s goto %s", $5.tac, l_start);
            free(l_start);
            print_indent();
            fprintf(output, "while (%s);\n", $5.code);
            free($5.code); free($5.type); free($5.tac);
        }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * LOOP CONTROL
 * ════════════════════════════════════════════════════════════════════════════ */

loop_control_stmt
    : TOK_TERMINATE TOK_SEMICOLON
        { print_indent(); fprintf(output, "break;\n");    tac_emit("break"); }
    | TOK_SKIP TOK_SEMICOLON
        { print_indent(); fprintf(output, "continue;\n"); tac_emit("continue"); }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * FUNCTION DECLARATIONS
 * ════════════════════════════════════════════════════════════════════════════ */

func_header_params
    : TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN param_list TOK_RPAREN
        {
            add_symbol($3, $2, 1, 0);
            fprintf(output, "%s %s(%s)", $2, $3, $5);
            tac_emit("function %s %s(%s)", $2, $3, $5);
            free($2); free($3); free($5);
        }
    ;

func_header_void
    : TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN
        {
            add_symbol($3, $2, 1, 0);
            fprintf(output, "%s %s(void)", $2, $3);
            tac_emit("function %s %s()", $2, $3);
            free($2); free($3);
        }
    ;

function_decl
    : func_header_params block
        { fprintf(output, "\n"); tac_emit("end_function\n"); }
    | func_header_void   block
        { fprintf(output, "\n"); tac_emit("end_function\n"); }
    ;

param_list
    : param
        { $$ = $1; }
    | param_list TOK_COMMA param
        { char *t = mkstr("%s, %s", $1, $3); free($1); free($3); $$ = t; }
    ;

param
    : data_type TOK_IDENTIFIER
        {
            add_symbol($2, $1, 0, 0);
            $$ = mkstr("%s %s", $1, $2);
            free($1); free($2);
        }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * RETURN STATEMENT
 * ════════════════════════════════════════════════════════════════════════════ */

return_stmt
    : TOK_RETURN_CARGO expr TOK_SEMICOLON
        {
            print_indent();
            fprintf(output, "return %s;\n", $2.code);
            tac_emit("return %s", $2.tac);
            free($2.code); free($2.type); free($2.tac);
        }
    | TOK_RETURN_CARGO TOK_SEMICOLON
        { print_indent(); fprintf(output, "return;\n"); tac_emit("return"); }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * I/O STATEMENTS  (type-aware format specifiers)
 *
 *   TRANSMIT expr ;
 *     - string literal  →  printf("…\n");
 *     - float / double  →  printf("%f\n", expr);
 *     - char            →  printf("%c\n", expr);
 *     - default (int)   →  printf("%d\n", expr);
 *
 *   RECEIVE IDENTIFIER ;
 *     - uses symbol-table type for scanf format
 * ════════════════════════════════════════════════════════════════════════════ */

io_stmt
    : TOK_TRANSMIT expr TOK_SEMICOLON
        {
            print_indent();
            const char *e = $2.code;
            const char *tp = $2.type;
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
            tac_emit("print %s", $2.tac);
            free($2.code); free($2.type); free($2.tac);
        }
    | TOK_RECEIVE TOK_IDENTIFIER TOK_SEMICOLON
        {
            const char *vt = lookup_type($2);
            if (!vt) {
                fprintf(stderr,
                    "Warning at line %d: Variable '%s' used without declaration\n",
                    yylineno, $2);
                vt = "int";
            }
            print_indent();
            const char *fmt = scanf_format_for_type(vt);
            fprintf(output, "scanf(\"%s\", &%s);\n", fmt, $2);
            tac_emit("read %s", $2);
            free($2);
        }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * EXPRESSION STATEMENT
 * ════════════════════════════════════════════════════════════════════════════ */

expr_stmt
    : expr TOK_SEMICOLON
        {
            print_indent();
            fprintf(output, "%s;\n", $1.code);
            tac_emit("%s", $1.tac);
            free($1.code); free($1.type); free($1.tac);
        }
    ;

%%

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
