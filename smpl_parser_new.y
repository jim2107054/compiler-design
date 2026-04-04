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
#include "ast.h"
#include "types.h"

/* ─── Lexer Interface ────────────────────────────────────────────────────── */
extern int   yylex(void);
extern int   yylineno;
extern char *yytext;
extern FILE *yyin;

/* ─── Global AST Root ────────────────────────────────────────────────────── */
ASTNode *g_program_ast = NULL;

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
#define NEW_ARRAY_ASSIGN(name, idx, val) ast_array_assign((name), (idx), (val), yylineno)
#define NEW_IF(cond, then_br, else_br) ast_if_stmt((cond), (then_br), (else_br), yylineno)
#define NEW_WHILE(cond, body) ast_while_loop((cond), (body), yylineno)
#define NEW_BLOCK(stmts) ast_block((stmts), yylineno)
#define NEW_STMT_LIST(stmt, next) ast_stmt_list((stmt), (next))
#define NEW_BREAK() ast_break(yylineno)
#define NEW_CONTINUE() ast_continue(yylineno)
#define NEW_RETURN(expr) ast_return((expr), yylineno)
#define NEW_OUTPUT(expr) ast_output((expr), yylineno)
#define NEW_INPUT(target) ast_input((target), yylineno)
#define NEW_FUNC_CALL(name, args) ast_func_call((name), (args), yylineno)
#define NEW_ARRAY_ACCESS(name, idx) ast_array_access((name), (idx), yylineno)
#define NEW_PROGRAM(funcs, main) ast_program((funcs), yylineno)

static const char *smpl_type_to_string(SMPLType type) {
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

%}

/* ═══════════════════════════════════════════════════════════════════════════
 * BISON DECLARATIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

%define parse.error verbose

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
%type <type_val>  data_type
%type <sval>      for_init for_update
%type <ival>      array_size_opt

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
            ASTNode *func_list_node = $1;
            ASTNode *main_body = ast_stmt_list($3, NULL);
            $$ = ast_program(func_list_node, main_body);
            g_program_ast = $$;
        }
    | mission_header statement_list TOK_LANDING TOK_SEMICOLON
        {
            ASTNode *main_body = ast_stmt_list($2, NULL);
            $$ = ast_program(NULL, main_body);
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
            $$ = ast_stmt_list($2, $1);
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
            if ($1 == NULL) {
                $$ = ast_stmt_list($2, NULL);
            } else {
                $$ = ast_stmt_list($2, $1);
            }
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
            $$ = ast_block($2);
        }
    ;

/* ─── Expression Statement ───────────────────────────────────────────────── */

expr_stmt
    : expr TOK_SEMICOLON
        {
            $$ = $1;
        }
    ;

/* ─── Declarations ────────────────────────────────────────────────────────── */

declaration_stmt
    : TOK_LOAD data_type TOK_IDENTIFIER TOK_SEMICOLON
        {
            $$ = ast_declaration($2, $3, 0, NULL, NULL);
            free($3);
        }
    | TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON
        {
            $$ = ast_declaration($2, $3, 0, NULL, $5);
            free($3);
        }
    | TOK_LOAD TOK_CARGO_ARRAY data_type TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_SEMICOLON
        {
            $$ = ast_declaration($3, $4, 1, ast_int_literal($6), NULL);
            free($4);
        }
    | TOK_LOAD TOK_CARGO_ARRAY data_type TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_STORE TOK_LBRACE arg_list TOK_RBRACE TOK_SEMICOLON
        {
            $$ = ast_declaration($3, $4, 1, ast_int_literal($6), $10);
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
            $$ = ast_assignment($1, $3);
            free($1);
        }
    | TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET TOK_STORE expr TOK_SEMICOLON
        {
            $$ = ast_array_assign($1, $3, $6);
            free($1);
        }
    ;

boost_stmt
    : TOK_BOOST TOK_IDENTIFIER TOK_SEMICOLON
        {
            /* x++ equivalent: x = x + 1 */
            ASTNode *var = ast_identifier($2);
            ASTNode *one = ast_int_literal(1);
            ASTNode *add = ast_binary_op("+", var, one);
            $$ = ast_assignment($2, add);
            free($2);
        }
    ;

degrade_stmt
    : TOK_DEGRADE TOK_IDENTIFIER TOK_SEMICOLON
        {
            /* x-- equivalent: x = x - 1 */
            ASTNode *var = ast_identifier($2);
            ASTNode *one = ast_int_literal(1);
            ASTNode *sub = ast_binary_op("-", var, one);
            $$ = ast_assignment($2, sub);
            free($2);
        }
    ;

/* ─── Control Flow: IF ────────────────────────────────────────────────────── */

if_stmt
    : TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN statement
        {
            $$ = ast_if_stmt($3, $5, NULL);
        }
    | TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN statement TOK_ELSE_CHECK statement
        {
            $$ = ast_if_stmt($3, $5, $7);
        }
    | TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN statement TOK_OTHERWISE statement
        {
            $$ = ast_if_stmt($3, $5, $7);
        }
    ;

/* ─── Control Flow: SWITCH ─────────────────────────────────────────────────── */

switch_stmt
    : TOK_PROTOCOL TOK_LPAREN expr TOK_RPAREN TOK_LAUNCH case_list TOK_ABORT
        {
            $$ = ast_switch_stmt($3, $6);
        }
    ;

case_list
    : case_stmt
        {
            $$ = ast_stmt_list($1, NULL);
        }
    | case_list case_stmt
        {
            $$ = ast_stmt_list($2, $1);
        }
    ;

case_stmt
    : TOK_SCENARIO TOK_INTEGER TOK_COLON statement_list
        {
            $$ = ast_case_stmt(ast_int_literal($2), $4);
        }
    | TOK_SCENARIO TOK_CHAR_LITERAL TOK_COLON statement_list
        {
            /* Convert char literal to int */
            $$ = ast_case_stmt(ast_char_literal($2[0]), $4);
            free($2);
        }
    | TOK_DEFAULT_PROTOCOL TOK_COLON statement_list
        {
            $$ = ast_default_case($3);
        }
    ;

/* ─── Loops: WHILE ─────────────────────────────────────────────────────────── */

while_loop
    : TOK_ORBIT_WHILE TOK_LPAREN expr TOK_RPAREN statement
        {
            $$ = ast_while_loop($3, $5);
        }
    ;

/* ─── Loops: FOR ───────────────────────────────────────────────────────────── */

for_loop
    : TOK_SEQUENCE TOK_LPAREN for_init TOK_SEMICOLON expr TOK_SEMICOLON for_update TOK_RPAREN statement
        {
            /* For simplicity, parse init/update as strings and create simple nodes */
            /* In a full implementation, these should be proper AST sub-trees */
            ASTNode *init = ast_for_init($3);
            ASTNode *update = ast_for_update($7);
            $$ = ast_for_loop(init, $5, update, $9);
            if ($3) free($3);
            if ($7) free($7);
        }
    ;

for_init
    : /* empty */                                { $$ = NULL; }
    | TOK_IDENTIFIER TOK_STORE expr              { $$ = strdup($1); free($1); }
    | TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr 
        { $$ = strdup($3); free($3); }
    ;

for_update
    : /* empty */                                { $$ = NULL; }
    | TOK_IDENTIFIER TOK_STORE expr              { $$ = strdup($1); free($1); }
    | TOK_BOOST TOK_IDENTIFIER                   { $$ = strdup($2); free($2); }
    | TOK_DEGRADE TOK_IDENTIFIER                 { $$ = strdup($2); free($2); }
    ;

/* ─── Loops: DO-WHILE ──────────────────────────────────────────────────────── */

do_while_loop
    : TOK_REPEAT statement TOK_UNTIL TOK_LPAREN expr TOK_RPAREN TOK_SEMICOLON
        {
            $$ = ast_do_while_loop($5, $2);
        }
    ;

/* ─── Loop Control ──────────────────────────────────────────────────────────── */

loop_control_stmt
    : TOK_TERMINATE TOK_SEMICOLON
        {
            $$ = ast_break();
        }
    | TOK_SKIP TOK_SEMICOLON
        {
            $$ = ast_continue();
        }
    ;

/* ─── I/O Statements ────────────────────────────────────────────────────────── */

io_stmt
    : TOK_RECEIVE TOK_LPAREN TOK_IDENTIFIER TOK_RPAREN TOK_SEMICOLON
        {
            $$ = ast_input(ast_identifier($3));
            free($3);
        }
    | TOK_TRANSMIT TOK_LPAREN expr TOK_RPAREN TOK_SEMICOLON
        {
            $$ = ast_output($3);
        }
    ;

/* ─── Function Declaration ──────────────────────────────────────────────────── */

function_decl
    : TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN param_list TOK_RPAREN block
        {
            $$ = ast_function($2, $3, $5, $7);
            free($3);
        }
    | TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN block
        {
            $$ = ast_function($2, $3, NULL, $6);
            free($3);
        }
    ;

param_list
    : data_type TOK_IDENTIFIER
        {
            $$ = ast_param($1, $2);
            free($2);
        }
    | param_list TOK_COMMA data_type TOK_IDENTIFIER
        {
            $$ = ast_stmt_list(ast_param($3, $4), $1);
            free($4);
        }
    ;

return_stmt
    : TOK_RETURN_CARGO expr TOK_SEMICOLON
        {
            $$ = ast_return($2);
        }
    | TOK_RETURN_CARGO TOK_SEMICOLON
        {
            $$ = ast_return(NULL);
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
            $$ = ast_int_literal($1);
        }
    | TOK_FLOAT_NUM
        {
            $$ = ast_float_literal($1);
        }
    | TOK_CHAR_LITERAL
        {
            $$ = ast_char_literal($1[0]);
            free($1);
        }
    | TOK_STRING_LITERAL
        {
            $$ = ast_string_literal($1);
            free($1);
        }
    | TOK_IDENTIFIER
        {
            $$ = ast_identifier($1);
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
            $$ = ast_unary_op("!", $2);
        }
    ;

binary_expr
    : TOK_COMBINE expr expr
        {
            $$ = ast_binary_op("+", $2, $3);
        }
    | TOK_REDUCE expr expr
        {
            $$ = ast_binary_op("-", $2, $3);
        }
    | TOK_AMPLIFY expr expr
        {
            $$ = ast_binary_op("*", $2, $3);
        }
    | TOK_SPLIT expr expr
        {
            $$ = ast_binary_op("/", $2, $3);
        }
    | TOK_REMAINDER expr expr
        {
            $$ = ast_binary_op("%", $2, $3);
        }
    | expr TOK_EXCEEDS expr
        {
            $$ = ast_binary_op(">", $1, $3);
        }
    | expr TOK_BELOW expr
        {
            $$ = ast_binary_op("<", $1, $3);
        }
    | expr TOK_EXCEEDS_OR_EQUAL expr
        {
            $$ = ast_binary_op(">=", $1, $3);
        }
    | expr TOK_BELOW_OR_EQUAL expr
        {
            $$ = ast_binary_op("<=", $1, $3);
        }
    | expr TOK_MATCHES expr
        {
            $$ = ast_binary_op("==", $1, $3);
        }
    | expr TOK_DIFFERS expr
        {
            $$ = ast_binary_op("!=", $1, $3);
        }
    | expr TOK_BOTH expr
        {
            $$ = ast_binary_op("&&", $1, $3);
        }
    | expr TOK_EITHER expr
        {
            $$ = ast_binary_op("||", $1, $3);
        }
    ;

/* ─── Function Call ───────────────────────────────────────────────────────── */

function_call
    : TOK_IDENTIFIER TOK_LPAREN arg_list TOK_RPAREN
        {
            $$ = ast_func_call($1, $3);
            free($1);
        }
    | TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN
        {
            $$ = ast_func_call($1, NULL);
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
            $$ = ast_stmt_list($3, $1);
        }
    ;

/* ─── Array Access ─────────────────────────────────────────────────────────── */

array_access
    : TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET
        {
            $$ = ast_array_access($1, $3);
            free($1);
        }
    ;

%%

/* ═══════════════════════════════════════════════════════════════════════════
 * END OF GRAMMAR
 * ═══════════════════════════════════════════════════════════════════════════ */
