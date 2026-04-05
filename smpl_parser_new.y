%{
/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  SMPL Parser — Space Mission Programming Language
 *  Bison specification file: Modular Architecture v2.0
 * ═══════════════════════════════════════════════════════════════════════════
 *  Author : MD. Jahid Hasan Jim
 *  Roll   : 2107054
 *  Course : CSE 3212 — Compiler Design Lab
 *  Inst   : Khulna University of Engineering & Technology (KUET)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  MODULAR ARCHITECTURE with clean separation:
 *    ✓ Parser builds Abstract Syntax Tree (AST) only
 *    ✓ Symbol Table managed by separate module  
 *    ✓ Semantic Analysis performed in separate pass
 *    ✓ Runtime Execution (interpreter) in semantic module
 *    ✓ TAC generation in separate IR module
 *    ✓ C code generation in separate codegen module
 *
 *  Build:
 *    bison -d smpl_parser_new.y
 *    flex  smpl_lexer.l
 *    gcc src/*.c smpl_parser_new.tab.c lex.yy.c -o smpl_compiler
 *
 *  Run:
 *    ./smpl_compiler --execute program.smpl       # Interpret
 *    ./smpl_compiler program.smpl program.c       # Compile to C
 *    ./smpl_compiler -t program.smpl program.tac  # Generate TAC
 * ═══════════════════════════════════════════════════════════════════════════
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "ast.h"

/* ─── Lexer Interface ────────────────────────────────────────────────────── */
extern int   yylex(void);
extern int   yylineno;
extern char *yytext;
extern FILE *yyin;

/* ─── Global AST Root ────────────────────────────────────────────────────── */
ASTNode *g_program_ast = NULL;

/* ─── Forward Declarations ───────────────────────────────────────────────── */
const char *smpl_type_to_string(SMPLType type);

/* ─── Error Handler ──────────────────────────────────────────────────────── */
void yyerror(const char *msg) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yylineno, msg);
}

/* ─── AST Constructor Wrappers (add line numbers automatically) ─────────── */
#define NEW_INT(val) ast_int_literal((val), yylineno)
#define NEW_FLOAT(val) ast_float_literal((val), yylineno)
#define NEW_CHAR(val) ast_char_literal((val), yylineno)
#define NEW_STRING(val) ast_string_literal((val), yylineno)
#define NEW_ID(name) ast_identifier((name), yylineno)
#define NEW_BINOP(op, l, r) ast_binary_op((op), (l), (r), yylineno)
#define NEW_UNOP(op, operand) ast_unary_op((op), (operand), yylineno)
#define NEW_DECL(type, name, is_arr, size, init) ast_declaration(smpl_type_to_string(type), (name), (is_arr), (size), (init), yylineno)
#define NEW_ASSIGN(name, val) ast_assignment((name), (val), yylineno)
#define NEW_ARRAY_ASSIGN(name, idx, val) ast_array_assignment((name), (idx), (val), yylineno)
#define NEW_IF(cond, then_br, else_br) ast_if_stmt((cond), (then_br), (else_br), yylineno)
#define NEW_WHILE(cond, body) ast_while_loop((cond), (body), yylineno)
#define NEW_BLOCK(stmts) ast_block((stmts), yylineno)
#define NEW_STMT_LIST(stmt, next) ast_stmt_list((stmt), (next))
#define NEW_BREAK() ast_break(yylineno)
#define NEW_CONTINUE() ast_continue(yylineno)
#define NEW_RETURN(expr) ast_return((expr), yylineno)
#define NEW_OUTPUT(expr) ast_output((expr), yylineno)
#define NEW_INPUT(target) ast_input((target), yylineno)
#define NEW_FUNC_CALL(name, args) ast_function_call((name), (args), yylineno)
#define NEW_ARRAY_ACCESS(name, idx) ast_array_access((name), (idx), yylineno)
#define NEW_PROGRAM(funcs, main) ast_program((funcs), yylineno)

const char *smpl_type_to_string(SMPLType type) {
    switch(type) {
        case SMPL_INT: return "int";
        case SMPL_FLOAT: return "float";
        case SMPL_CHAR: return "char";
        case SMPL_DOUBLE: return "double";
        case SMPL_VOID: return "void";
        case SMPL_STRING: return "string";
        default: return "int";
    }
}

/* Wrapper functions to add line numbers automatically */
static inline ASTNode* wrap_declaration(SMPLType type, const char* name, int is_array, ASTNode* size, ASTNode* init) {
    return ast_declaration(smpl_type_to_string(type), name, is_array, size, init, yylineno);
}

static inline ASTNode* wrap_assignment(const char* name, ASTNode* value) {
    return ast_assignment(name, value, yylineno);
}

static inline ASTNode* wrap_array_assignment(const char* name, ASTNode* index, ASTNode* value) {
    return ast_array_assignment(name, index, value, yylineno);
}

static inline ASTNode* wrap_if_stmt(ASTNode* cond, ASTNode* then_br, ASTNode* else_br) {
    return ast_if_stmt(cond, then_br, else_br, yylineno);
}

static inline ASTNode* wrap_while_loop(ASTNode* cond, ASTNode* body) {
    return ast_while_loop(cond, body, yylineno);
}

static inline ASTNode* wrap_block(ASTNode* stmts) {
    return ast_block(stmts, yylineno);
}

static inline ASTNode* wrap_int_literal(int value) {
    return ast_int_literal(value, yylineno);
}

static inline ASTNode* wrap_float_literal(float value) {
    return ast_float_literal(value, yylineno);
}

static inline ASTNode* wrap_char_literal(const char* value) {
    return ast_char_literal(value, yylineno);
}

static inline ASTNode* wrap_string_literal(const char* value) {
    return ast_string_literal(value, yylineno);
}

static inline ASTNode* wrap_identifier(const char* name) {
    return ast_identifier(name, yylineno);
}

static inline ASTNode* wrap_binary_op(const char* op, ASTNode* left, ASTNode* right) {
    return ast_binary_op(op, left, right, yylineno);
}

static inline ASTNode* wrap_unary_op(const char* op, ASTNode* operand) {
    return ast_unary_op(op, operand, yylineno);
}

static inline ASTNode* wrap_function_call(const char* name, ASTNode* args) {
    return ast_function_call(name, args, yylineno);
}

static inline ASTNode* wrap_array_access(const char* name, ASTNode* index) {
    return ast_array_access(name, index, yylineno);
}

static inline ASTNode* wrap_break() {
    return ast_break(yylineno);
}

static inline ASTNode* wrap_continue() {
    return ast_continue(yylineno);
}

static inline ASTNode* wrap_return(ASTNode* value) {
    return ast_return(value, yylineno);
}

static inline ASTNode* wrap_input(const char* var_name) {
    return ast_input(var_name, yylineno);
}

static inline ASTNode* wrap_output(ASTNode* expr) {
    return ast_output(expr, yylineno);
}

static inline ASTNode* wrap_function_def(SMPLType ret_type, const char* name, ASTNode* params, ASTNode* body) {
    return ast_function_def(smpl_type_to_string(ret_type), name, params, body, yylineno);
}

static inline ASTNode* wrap_param(SMPLType type, const char* name) {
    return ast_param(smpl_type_to_string(type), name, yylineno);
}

static inline ASTNode* wrap_program(ASTNode* stmts) {
    return ast_program(stmts, yylineno);
}

static inline ASTNode* wrap_switch_stmt(ASTNode* expr, ASTNode* cases) {
    return ast_switch_stmt(expr, cases, yylineno);
}

static inline ASTNode* wrap_case_stmt(ASTNode* value, ASTNode* stmts, int is_default) {
    return ast_case_stmt(value, stmts, is_default, yylineno);
}

static inline ASTNode* wrap_do_while(ASTNode* body, ASTNode* cond) {
    return ast_do_while(body, cond, yylineno);
}

static inline ASTNode* wrap_for_loop(ASTNode* init, ASTNode* cond, ASTNode* update, ASTNode* body) {
    return ast_for_loop(init, cond, update, body, yylineno);
}

%}

/* ═══════════════════════════════════════════════════════════════════════════
 * BISON DECLARATIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

%define parse.error verbose

%code requires {
    #include "ast.h"
    #include "types.h"
    
    typedef enum {
        SMPL_INT,
        SMPL_FLOAT,
        SMPL_CHAR,
        SMPL_DOUBLE,
        SMPL_VOID,
        SMPL_STRING
    } SMPLType;
}

/* Semantic value union */
%union {
    int           ival;
    float         fval;
    double        dval;
    char          cval;
    char         *sval;
    SMPLType      type_val;
    ASTNode      *ast;
}

/* ─── Terminal Tokens (from lexer) ───────────────────────────────────────── */

/* Program structure */
%token TOK_MISSION TOK_LANDING TOK_LAUNCH TOK_ABORT

/* Data types */
%token TOK_CARGO_INT TOK_CARGO_FLOAT TOK_CARGO_CHAR TOK_CARGO_DOUBLE TOK_VOID_SPACE

/* Declaration / assignment */
%token TOK_LOAD TOK_STORE TOK_CARGO_ARRAY

/* Arithmetic operators */
%token TOK_COMBINE TOK_REDUCE TOK_AMPLIFY TOK_SPLIT TOK_REMAINDER
%token TOK_BOOST TOK_DEGRADE

/* Relational operators */
%token TOK_EXCEEDS TOK_BELOW TOK_EXCEEDS_OR_EQUAL TOK_BELOW_OR_EQUAL
%token TOK_MATCHES TOK_DIFFERS

/* Logical operators */
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

/* Literals & identifiers */
%token <ival> TOK_INTEGER
%token <fval> TOK_FLOAT_NUM
%token <sval> TOK_CHAR_LITERAL TOK_STRING_LITERAL TOK_IDENTIFIER

/* Punctuation */
%token TOK_SEMICOLON TOK_COLON TOK_COMMA
%token TOK_LPAREN TOK_RPAREN
%token TOK_LBRACKET TOK_RBRACKET
%token TOK_LBRACE TOK_RBRACE

/* ─── Non-Terminal Types ──────────────────────────────────────────────────── */
%type <ast>       program mission_header function_list
%type <ast>       statement_list statement declaration_stmt
%type <ast>       assignment_stmt expr_stmt if_stmt switch_stmt
%type <ast>       while_loop for_loop do_while_loop case_list case_stmt
%type <ast>       loop_control_stmt io_stmt return_stmt function_decl
%type <ast>       block expr primary_expr unary_expr binary_expr
%type <ast>       function_call arg_list array_access
%type <ast>       boost_stmt degrade_stmt param_list
%type <ast>       else_part for_init for_update
%type <type_val>  data_type

/* ─── Operator Precedence (Lowest → Highest) ─────────────────────────────── */
%left  TOK_EITHER
%left  TOK_BOTH
%left  TOK_MATCHES TOK_DIFFERS
%left  TOK_EXCEEDS TOK_BELOW TOK_EXCEEDS_OR_EQUAL TOK_BELOW_OR_EQUAL
%left  TOK_COMBINE TOK_REDUCE
%left  TOK_AMPLIFY TOK_SPLIT TOK_REMAINDER
%right TOK_NEGATE

%%

/* ═══════════════════════════════════════════════════════════════════════════
 * GRAMMAR RULES
 * ═══════════════════════════════════════════════════════════════════════════ */

/* ─── Top-Level Program ────────────────────────────────────────────────── */

program
    : function_list mission_header statement_list TOK_LANDING TOK_SEMICOLON
        {
            /* Combine functions and main body into one list */
            ASTNode *all = $1;
            ASTNode *body = $3;
            /* Walk to end of function list and append main body */
            if (all && body) {
                ASTNode *last = all;
                while (last->type == AST_STMT_LIST && last->data.stmt_list.next)
                    last = last->data.stmt_list.next;
                if (last->type == AST_STMT_LIST)
                    last->data.stmt_list.next = body;
                else
                    all = ast_stmt_list(all, body);
            } else if (!all) {
                all = body;
            }
            $$ = wrap_program(all);
            g_program_ast = $$;
        }
    | mission_header statement_list TOK_LANDING TOK_SEMICOLON
        {
            $$ = wrap_program($2);
            g_program_ast = $$;
        }
    ;

mission_header
    : TOK_MISSION
        {
            $$ = NULL; /* Mission header processed, no AST node needed */
        }
    ;

function_list
    : function_decl
        {
            $$ = ast_stmt_list($1, NULL);
        }
    | function_list function_decl
        {
            $$ = ast_append_stmt($1, $2);
        }
    ;

/* ─── Statement List ────────────────────────────────────────────────────── */

statement_list
    : /* empty */
        {
            $$ = NULL;
        }
    | statement_list statement
        {
            /* Append statement to maintain proper order */
            $$ = ast_append_stmt($1, $2);
        }
    ;

statement
    : declaration_stmt    { $$ = $1; }
    | assignment_stmt     { $$ = $1; }
    | boost_stmt          { $$ = $1; }
    | degrade_stmt        { $$ = $1; }
    | if_stmt             { $$ = $1; }
    | switch_stmt         { $$ = $1; }
    | while_loop          { $$ = $1; }
    | for_loop            { $$ = $1; }
    | do_while_loop       { $$ = $1; }
    | io_stmt             { $$ = $1; }
    | return_stmt         { $$ = $1; }
    | loop_control_stmt   { $$ = $1; }
    | block               { $$ = $1; }
    | expr_stmt           { $$ = $1; }
    ;

/* ─── Block (Scope) ──────────────────────────────────────────────────────── */

block
    : TOK_LAUNCH statement_list TOK_ABORT
        {
            $$ = ast_block($2, yylineno);
        }
    ;

/* ─── Expression Statement ───────────────────────────────────────────────── */

expr_stmt
    : expr TOK_SEMICOLON
        {
            $$ = ast_expr_stmt($1, yylineno);
        }
    ;

/* ─── Declarations ────────────────────────────────────────────────────────── */

declaration_stmt
    : TOK_LOAD data_type TOK_IDENTIFIER TOK_SEMICOLON
        {
            $$ = ast_declaration(smpl_type_to_string($2), $3, 0, NULL, NULL, yylineno);
            free($3);
        }
    | TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON
        {
            $$ = ast_declaration(smpl_type_to_string($2), $3, 0, NULL, $5, yylineno);
            free($3);
        }
    | TOK_LOAD TOK_CARGO_ARRAY data_type TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_SEMICOLON
        {
            $$ = ast_declaration(smpl_type_to_string($3), $4, 1, ast_int_literal($6, yylineno), NULL, yylineno);
            free($4);
        }
    | TOK_LOAD TOK_CARGO_ARRAY data_type TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_STORE TOK_LBRACE arg_list TOK_RBRACE TOK_SEMICOLON
        {
            $$ = ast_declaration(smpl_type_to_string($3), $4, 1, ast_int_literal($6, yylineno), $10, yylineno);
            free($4);
        }
    | TOK_LOAD data_type TOK_CARGO_ARRAY TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_SEMICOLON
        {
            $$ = ast_declaration(smpl_type_to_string($2), $4, 1, ast_int_literal($6, yylineno), NULL, yylineno);
            free($4);
        }
    | TOK_LOAD data_type TOK_CARGO_ARRAY TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_STORE TOK_LBRACE arg_list TOK_RBRACE TOK_SEMICOLON
        {
            $$ = ast_declaration(smpl_type_to_string($2), $4, 1, ast_int_literal($6, yylineno), $10, yylineno);
            free($4);
        }
    ;

data_type
    : TOK_CARGO_INT      { $$ = SMPL_INT;    }
    | TOK_CARGO_FLOAT    { $$ = SMPL_FLOAT;  }
    | TOK_CARGO_CHAR     { $$ = SMPL_CHAR;   }
    | TOK_CARGO_DOUBLE   { $$ = SMPL_DOUBLE; }
    | TOK_VOID_SPACE     { $$ = SMPL_VOID;   }
    ;

/* ─── Assignment ──────────────────────────────────────────────────────────── */

assignment_stmt
    : TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON
        {
            $$ = wrap_assignment($1, $3);
            free($1);
        }
    | TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET TOK_STORE expr TOK_SEMICOLON
        {
            $$ = wrap_array_assignment($1, $3, $6);
            free($1);
        }
    ;

boost_stmt
    : TOK_BOOST TOK_IDENTIFIER TOK_SEMICOLON
        {
            /* x++ equivalent: x = x + 1 */
            ASTNode *var = ast_identifier($2, yylineno);
            ASTNode *one = ast_int_literal(1, yylineno);
            ASTNode *add = ast_binary_op("+", var, one, yylineno);
            $$ = ast_assignment($2, add, yylineno);
            free($2);
        }
    ;

degrade_stmt
    : TOK_DEGRADE TOK_IDENTIFIER TOK_SEMICOLON
        {
            /* x-- equivalent: x = x - 1 */
            ASTNode *var = ast_identifier($2, yylineno);
            ASTNode *one = ast_int_literal(1, yylineno);
            ASTNode *sub = ast_binary_op("-", var, one, yylineno);
            $$ = ast_assignment($2, sub, yylineno);
            free($2);
        }
    ;

/* ─── Control Flow: IF ────────────────────────────────────────────────────── */

if_stmt
    : TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN statement else_part
        {
            $$ = wrap_if_stmt($3, $5, $6);
        }
    ;

else_part
    : /* empty */
        {
            $$ = NULL;
        }
    | TOK_ELSE_CHECK TOK_LPAREN expr TOK_RPAREN statement else_part
        {
            $$ = wrap_if_stmt($3, $5, $6);
        }
    | TOK_OTHERWISE statement
        {
            $$ = $2;
        }
    ;

/* ─── Control Flow: SWITCH ─────────────────────────────────────────────────── */

switch_stmt
    : TOK_PROTOCOL TOK_LPAREN expr TOK_RPAREN TOK_LAUNCH case_list TOK_ABORT
        {
            $$ = ast_switch_stmt($3, $6, yylineno);
        }
    ;

case_list
    : case_stmt
        {
            $$ = ast_stmt_list($1, NULL);
        }
    | case_list case_stmt
        {
            $$ = ast_append_stmt($1, $2);
        }
    ;

case_stmt
    : TOK_SCENARIO TOK_INTEGER TOK_COLON statement_list
        {
            $$ = ast_case_stmt(ast_int_literal($2, yylineno), $4, 0, yylineno);
        }
    | TOK_SCENARIO TOK_CHAR_LITERAL TOK_COLON statement_list
        {
            /* Convert char literal to int */
            $$ = ast_case_stmt(ast_char_literal($2, yylineno), $4, 0, yylineno);
            free($2);
        }
    | TOK_DEFAULT_PROTOCOL TOK_COLON statement_list
        {
            $$ = ast_case_stmt(NULL, $3, 1, yylineno);
        }
    ;

/* ─── Loops: WHILE ─────────────────────────────────────────────────────────── */

while_loop
    : TOK_ORBIT_WHILE TOK_LPAREN expr TOK_RPAREN statement
        {
            $$ = ast_while_loop($3, $5, yylineno);
        }
    ;

/* ─── Loops: FOR ───────────────────────────────────────────────────────────── */

for_loop
    : TOK_SEQUENCE TOK_LPAREN for_init TOK_SEMICOLON expr TOK_SEMICOLON for_update TOK_RPAREN statement
        {
            $$ = ast_for_loop($3, $5, $7, $9, yylineno);
        }
    ;

for_init
    : /* empty */
        { $$ = NULL; }
    | TOK_IDENTIFIER TOK_STORE expr
        {
            $$ = ast_assignment($1, $3, yylineno);
            free($1);
        }
    | TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr
        {
            $$ = ast_declaration(smpl_type_to_string($2), $3, 0, NULL, $5, yylineno);
            free($3);
        }
    ;

for_update
    : /* empty */
        { $$ = NULL; }
    | TOK_IDENTIFIER TOK_STORE expr
        {
            $$ = ast_assignment($1, $3, yylineno);
            free($1);
        }
    | TOK_BOOST TOK_IDENTIFIER
        {
            ASTNode *var = ast_identifier($2, yylineno);
            ASTNode *one = ast_int_literal(1, yylineno);
            ASTNode *add = ast_binary_op("+", var, one, yylineno);
            $$ = ast_assignment($2, add, yylineno);
            free($2);
        }
    | TOK_DEGRADE TOK_IDENTIFIER
        {
            ASTNode *var = ast_identifier($2, yylineno);
            ASTNode *one = ast_int_literal(1, yylineno);
            ASTNode *sub = ast_binary_op("-", var, one, yylineno);
            $$ = ast_assignment($2, sub, yylineno);
            free($2);
        }
    ;

/* ─── Loops: DO-WHILE ──────────────────────────────────────────────────────── */

do_while_loop
    : TOK_REPEAT statement TOK_UNTIL TOK_LPAREN expr TOK_RPAREN TOK_SEMICOLON
        {
            $$ = ast_do_while($2, $5, yylineno);
        }
    ;

/* ─── Loop Control ──────────────────────────────────────────────────────────── */

loop_control_stmt
    : TOK_TERMINATE TOK_SEMICOLON
        {
            $$ = ast_break(yylineno);
        }
    | TOK_SKIP TOK_SEMICOLON
        {
            $$ = ast_continue(yylineno);
        }
    ;

/* ─── I/O Statements ────────────────────────────────────────────────────────── */

io_stmt
    : TOK_RECEIVE TOK_LPAREN TOK_IDENTIFIER TOK_RPAREN TOK_SEMICOLON
        {
            $$ = ast_input($3, yylineno);
            free($3);
        }
    | TOK_TRANSMIT expr TOK_SEMICOLON
        {
            $$ = ast_output($2, yylineno);
        }
    ;

/* ─── Function Declaration ──────────────────────────────────────────────────── */

function_decl
    : TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN param_list TOK_RPAREN block
        {
            $$ = ast_function_def(smpl_type_to_string($2), $3, $5, $7, yylineno);
            free($3);
        }
    | TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN block
        {
            $$ = ast_function_def(smpl_type_to_string($2), $3, NULL, $6, yylineno);
            free($3);
        }
    ;

param_list
    : data_type TOK_IDENTIFIER
        {
            $$ = ast_param(smpl_type_to_string($1), $2, yylineno);
            free($2);
        }
    | param_list TOK_COMMA data_type TOK_IDENTIFIER
        {
            $$ = ast_append_stmt($1, ast_param(smpl_type_to_string($3), $4, yylineno));
            free($4);
        }
    ;

return_stmt
    : TOK_RETURN_CARGO expr TOK_SEMICOLON
        {
            $$ = ast_return($2, yylineno);
        }
    | TOK_RETURN_CARGO TOK_SEMICOLON
        {
            $$ = ast_return(NULL, yylineno);
        }
    ;

/* ─── Expressions ─────────────────────────────────────────────────────────── */

expr
    : primary_expr       { $$ = $1; }
    | unary_expr         { $$ = $1; }
    | binary_expr        { $$ = $1; }
    | function_call      { $$ = $1; }
    | array_access       { $$ = $1; }
    ;

primary_expr
    : TOK_INTEGER
        {
            $$ = ast_int_literal($1, yylineno);
        }
    | TOK_FLOAT_NUM
        {
            $$ = ast_float_literal($1, yylineno);
        }
    | TOK_CHAR_LITERAL
        {
            $$ = ast_char_literal($1, yylineno);
            free($1);
        }
    | TOK_STRING_LITERAL
        {
            $$ = ast_string_literal($1, yylineno);
            free($1);
        }
    | TOK_IDENTIFIER
        {
            $$ = ast_identifier($1, yylineno);
            free($1);
        }
    | TOK_LPAREN expr TOK_RPAREN
        {
            $$ = $2;
        }
    ;

unary_expr
    : TOK_NEGATE expr
        {
            $$ = ast_unary_op("!", $2, yylineno);
        }
    ;

binary_expr
    : TOK_COMBINE expr expr
        {
            $$ = ast_binary_op("+", $2, $3, yylineno);
        }
    | TOK_REDUCE expr expr
        {
            $$ = ast_binary_op("-", $2, $3, yylineno);
        }
    | TOK_AMPLIFY expr expr
        {
            $$ = ast_binary_op("*", $2, $3, yylineno);
        }
    | TOK_SPLIT expr expr
        {
            $$ = ast_binary_op("/", $2, $3, yylineno);
        }
    | TOK_REMAINDER expr expr
        {
            $$ = ast_binary_op("%", $2, $3, yylineno);
        }
    | expr TOK_EXCEEDS expr
        {
            $$ = ast_binary_op(">", $1, $3, yylineno);
        }
    | expr TOK_BELOW expr
        {
            $$ = ast_binary_op("<", $1, $3, yylineno);
        }
    | expr TOK_EXCEEDS_OR_EQUAL expr
        {
            $$ = ast_binary_op(">=", $1, $3, yylineno);
        }
    | expr TOK_BELOW_OR_EQUAL expr
        {
            $$ = ast_binary_op("<=", $1, $3, yylineno);
        }
    | expr TOK_MATCHES expr
        {
            $$ = ast_binary_op("==", $1, $3, yylineno);
        }
    | expr TOK_DIFFERS expr
        {
            $$ = ast_binary_op("!=", $1, $3, yylineno);
        }
    | expr TOK_BOTH expr
        {
            $$ = ast_binary_op("&&", $1, $3, yylineno);
        }
    | expr TOK_EITHER expr
        {
            $$ = ast_binary_op("||", $1, $3, yylineno);
        }
    ;

/* ─── Function Call ───────────────────────────────────────────────────────── */

function_call
    : TOK_IDENTIFIER TOK_LPAREN arg_list TOK_RPAREN
        {
            $$ = ast_function_call($1, $3, yylineno);
            free($1);
        }
    | TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN
        {
            $$ = ast_function_call($1, NULL, yylineno);
            free($1);
        }
    ;

arg_list
    : expr
        {
            $$ = ast_stmt_list($1, NULL);
        }
    | arg_list TOK_COMMA expr
        {
            $$ = ast_append_stmt($1, $3);
        }
    ;

/* ─── Array Access ─────────────────────────────────────────────────────────── */

array_access
    : TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET
        {
            $$ = ast_array_access($1, $3, yylineno);
            free($1);
        }
    ;

%%

/* ═══════════════════════════════════════════════════════════════════════════
 * END OF GRAMMAR
 * ═══════════════════════════════════════════════════════════════════════════ */
