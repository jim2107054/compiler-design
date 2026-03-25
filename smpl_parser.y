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
 *  Build:
 *    bison -d smpl_parser.y          => smpl_parser.tab.c + smpl_parser.tab.h
 *    flex  smpl_lexer.l              => lex.yy.c
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
extern FILE *token_output;   /* diagnostic token file declared in lexer     */

/* ── Output / indentation ───────────────────────────────────────────────── */
FILE *output        = NULL;  /* target C source file                        */
int   indent_level  = 0;     /* number of open { } blocks                   */

/* Print N×4-space indentation */
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

/* ── Constant folding helper for prefix arithmetic ──────────────────────── */
/*   Returns 1 and sets *result if both strings are pure integers            */
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
    return 1;
}

/* ── Error handler ──────────────────────────────────────────────────────── */
void yyerror(const char *msg) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yylineno, msg);
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

%}

/* ════════════════════════════════════════════════════════════════════════════
 * BISON DECLARATIONS
 * ════════════════════════════════════════════════════════════════════════════ */

%define parse.error verbose

/* Semantic value union */
%union {
    int   ival;   /* TOK_INTEGER                                             */
    float fval;   /* TOK_FLOAT_NUM                                           */
    char *sval;   /* identifiers, string/char literals, synthesised C code   */
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
%type <sval> expr data_type for_init for_update param param_list arg_list init_list

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
 *
 *   program → function_list MISSION statement_list LANDING ;
 *           | MISSION statement_list LANDING ;
 *
 * The #include <stdio.h> is emitted by main() before yyparse().
 * Functions before MISSION are emitted at the top of the file.
 * The MISSION token triggers "int main() {".
 * The LANDING token triggers "    return 0;\n}\n".
 * ════════════════════════════════════════════════════════════════════════════ */

program
    : function_list mission_header statement_list TOK_LANDING TOK_SEMICOLON
        { fprintf(output, "    return 0;\n}\n"); }
    | mission_header statement_list TOK_LANDING TOK_SEMICOLON
        { fprintf(output, "    return 0;\n}\n"); }
    ;

/* Separate rule so we can use a mid-rule action cleanly */
mission_header
    : TOK_MISSION
        {
            fprintf(output, "int main() {\n");
            indent_level = 1;
        }
    ;

/* Optional leading function declarations */
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
 *         →  {   …   }
 * ════════════════════════════════════════════════════════════════════════════ */

block_open
    : TOK_LAUNCH
        { fprintf(output, " {\n"); indent_level++; }
    ;

block
    : block_open statement_list TOK_ABORT
        { indent_level--; print_indent(); fprintf(output, "}\n"); }
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
 *
 *   LOAD data_type IDENTIFIER ;
 *   LOAD data_type IDENTIFIER STORE expr ;
 *   LOAD data_type CARGO_ARRAY IDENTIFIER [ INTEGER ] ;
 *   LOAD data_type CARGO_ARRAY IDENTIFIER [ INTEGER ] STORE { init_list } ;
 * ════════════════════════════════════════════════════════════════════════════ */

declaration_stmt
    : TOK_LOAD data_type TOK_IDENTIFIER TOK_SEMICOLON
        {
            print_indent();
            fprintf(output, "%s %s;\n", $2, $3);
            free($2); free($3);
        }
    | TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON
        {
            print_indent();
            fprintf(output, "%s %s = %s;\n", $2, $3, $5);
            free($2); free($3); free($5);
        }
    | TOK_LOAD data_type TOK_CARGO_ARRAY TOK_IDENTIFIER
      TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_SEMICOLON
        {
            print_indent();
            fprintf(output, "%s %s[%d];\n", $2, $4, $6);
            free($2); free($4);
        }
    | TOK_LOAD data_type TOK_CARGO_ARRAY TOK_IDENTIFIER
      TOK_LBRACKET TOK_INTEGER TOK_RBRACKET
      TOK_STORE TOK_LBRACE init_list TOK_RBRACE TOK_SEMICOLON
        {
            print_indent();
            fprintf(output, "%s %s[%d] = {%s};\n", $2, $4, $6, $10);
            free($2); free($4); free($10);
        }
    ;

init_list
    : expr
        { $$ = $1; }
    | init_list TOK_COMMA expr
        { char *t = mkstr("%s, %s", $1, $3); free($1); free($3); $$ = t; }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * ASSIGNMENT
 *
 *   IDENTIFIER STORE expr ;
 *   IDENTIFIER [ expr ] STORE expr ;
 * ════════════════════════════════════════════════════════════════════════════ */

assignment_stmt
    : TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON
        {
            print_indent();
            fprintf(output, "%s = %s;\n", $1, $3);
            free($1); free($3);
        }
    | TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET TOK_STORE expr TOK_SEMICOLON
        {
            print_indent();
            fprintf(output, "%s[%s] = %s;\n", $1, $3, $6);
            free($1); free($3); free($6);
        }
    ;

boost_stmt
    : TOK_BOOST TOK_IDENTIFIER TOK_SEMICOLON
        { print_indent(); fprintf(output, "%s++;\n", $2); free($2); }
    ;

degrade_stmt
    : TOK_DEGRADE TOK_IDENTIFIER TOK_SEMICOLON
        { print_indent(); fprintf(output, "%s--;\n", $2); free($2); }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * EXPRESSIONS
 *
 * Arithmetic operators are PREFIX:  COMBINE a b  →  (a + b)
 * Relational / logical operators are INFIX: a EXCEEDS b  →  (a > b)
 *
 * Constant-folding optimisation: integer-literal operands of prefix
 * arithmetic operators are evaluated at compile-time.
 * ════════════════════════════════════════════════════════════════════════════ */

expr
    /* ── Prefix arithmetic (SMPL keyword style) ──────────────────────── */
    : TOK_COMBINE expr expr
        {
            char *r;
            if (try_fold_int($2, $3, '+', &r)) { free($2); free($3); $$ = r; }
            else { $$ = mkstr("(%s + %s)", $2, $3); free($2); free($3); }
        }
    | TOK_REDUCE expr expr
        {
            char *r;
            if (try_fold_int($2, $3, '-', &r)) { free($2); free($3); $$ = r; }
            else { $$ = mkstr("(%s - %s)", $2, $3); free($2); free($3); }
        }
    | TOK_AMPLIFY expr expr
        {
            char *r;
            if (try_fold_int($2, $3, '*', &r)) { free($2); free($3); $$ = r; }
            else { $$ = mkstr("(%s * %s)", $2, $3); free($2); free($3); }
        }
    | TOK_SPLIT expr expr
        {
            char *r;
            if (try_fold_int($2, $3, '/', &r)) { free($2); free($3); $$ = r; }
            else { $$ = mkstr("(%s / %s)", $2, $3); free($2); free($3); }
        }
    | TOK_REMAINDER expr expr
        {
            char *r;
            if (try_fold_int($2, $3, '%', &r)) { free($2); free($3); $$ = r; }
            else { $$ = mkstr("(%s %% %s)", $2, $3); free($2); free($3); }
        }

    /* ── Infix relational ─────────────────────────────────────────────── */
    | expr TOK_EXCEEDS          expr { $$ = mkstr("(%s > %s)",  $1,$3); free($1);free($3); }
    | expr TOK_BELOW            expr { $$ = mkstr("(%s < %s)",  $1,$3); free($1);free($3); }
    | expr TOK_EXCEEDS_OR_EQUAL expr { $$ = mkstr("(%s >= %s)", $1,$3); free($1);free($3); }
    | expr TOK_BELOW_OR_EQUAL   expr { $$ = mkstr("(%s <= %s)", $1,$3); free($1);free($3); }
    | expr TOK_MATCHES          expr { $$ = mkstr("(%s == %s)", $1,$3); free($1);free($3); }
    | expr TOK_DIFFERS          expr { $$ = mkstr("(%s != %s)", $1,$3); free($1);free($3); }

    /* ── Infix logical ────────────────────────────────────────────────── */
    | expr TOK_BOTH  expr { $$ = mkstr("(%s && %s)", $1,$3); free($1);free($3); }
    | expr TOK_EITHER expr { $$ = mkstr("(%s || %s)", $1,$3); free($1);free($3); }

    /* ── Unary ────────────────────────────────────────────────────────── */
    | TOK_NEGATE expr { $$ = mkstr("(!%s)", $2); free($2); }

    /* ── Primary atoms ────────────────────────────────────────────────── */
    | TOK_IDENTIFIER
        { $$ = safe_strdup($1); free($1); }
    | TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET
        { $$ = mkstr("%s[%s]", $1, $3); free($1); free($3); }
    | TOK_IDENTIFIER TOK_LPAREN arg_list TOK_RPAREN
        { $$ = mkstr("%s(%s)", $1, $3); free($1); free($3); }
    | TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN
        { $$ = mkstr("%s()", $1); free($1); }
    | TOK_INTEGER
        { $$ = mkstr("%d", $1); }
    | TOK_FLOAT_NUM
        {
            char buf[64];
            snprintf(buf, sizeof buf, "%g", (double)$1);
            /* Ensure it contains a decimal point so it reads as a float */
            if (!strchr(buf, '.') && !strchr(buf, 'e'))
                strncat(buf, ".0", sizeof buf - strlen(buf) - 1);
            $$ = strdup(buf);
        }
    | TOK_CHAR_LITERAL
        { $$ = safe_strdup($1); free($1); }
    | TOK_STRING_LITERAL
        { $$ = safe_strdup($1); free($1); }
    | TOK_LPAREN expr TOK_RPAREN
        { $$ = mkstr("(%s)", $2); free($2); }
    ;

arg_list
    : expr
        { $$ = $1; }
    | arg_list TOK_COMMA expr
        { char *t = mkstr("%s, %s", $1, $3); free($1); free($3); $$ = t; }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * CONDITIONAL STATEMENTS
 *
 *   CHECK_IF ( expr ) block
 *   CHECK_IF ( expr ) block else_chain
 *   ELSE_CHECK ( expr ) block [else_chain]
 *   OTHERWISE block
 * ════════════════════════════════════════════════════════════════════════════ */

if_header
    : TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN
        { print_indent(); fprintf(output, "if (%s)", $3); free($3); }
    ;

else_if_header
    : TOK_ELSE_CHECK TOK_LPAREN expr TOK_RPAREN
        { print_indent(); fprintf(output, "else if (%s)", $3); free($3); }
    ;

otherwise_header
    : TOK_OTHERWISE
        { print_indent(); fprintf(output, "else"); }
    ;

if_stmt
    : if_header block
    | if_header block else_chain
    ;

else_chain
    : else_if_header block
    | else_if_header block else_chain
    | otherwise_header block
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * SWITCH STATEMENT
 *
 *   PROTOCOL ( expr ) LAUNCH case_list ABORT
 *   →  switch (expr) { … }
 * ════════════════════════════════════════════════════════════════════════════ */

switch_header
    : TOK_PROTOCOL TOK_LPAREN expr TOK_RPAREN TOK_LAUNCH
        {
            print_indent();
            fprintf(output, "switch (%s) {\n", $3);
            free($3);
            indent_level++;
        }
    ;

switch_stmt
    : switch_header case_list TOK_ABORT
        { indent_level--; print_indent(); fprintf(output, "}\n"); }
    ;

case_list
    : case_item
    | case_list case_item
    ;

case_item
    : TOK_SCENARIO TOK_INTEGER TOK_COLON
        { print_indent(); fprintf(output, "case %d:\n", $2); indent_level++; }
      statement_list
        { indent_level--; }
    | TOK_DEFAULT_PROTOCOL TOK_COLON
        { print_indent(); fprintf(output, "default:\n"); indent_level++; }
      statement_list
        { indent_level--; }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * LOOP STATEMENTS
 * ════════════════════════════════════════════════════════════════════════════ */

/* ── ORBIT_WHILE ─────────────────────────────────────────────────────────── */
while_header
    : TOK_ORBIT_WHILE TOK_LPAREN expr TOK_RPAREN
        { print_indent(); fprintf(output, "while (%s)", $3); free($3); }
    ;

while_loop
    : while_header block
    ;

/* ── SEQUENCE (for loop) ─────────────────────────────────────────────────── */
for_header
    : TOK_SEQUENCE TOK_LPAREN for_init TOK_SEMICOLON expr TOK_SEMICOLON for_update TOK_RPAREN
        {
            print_indent();
            fprintf(output, "for (%s; %s; %s)", $3, $5, $7);
            free($3); free($5); free($7);
        }
    ;

for_loop
    : for_header block
    ;

for_init
    : TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr
        { $$ = mkstr("%s %s = %s", $2, $3, $5); free($2); free($3); free($5); }
    | TOK_IDENTIFIER TOK_STORE expr
        { $$ = mkstr("%s = %s", $1, $3); free($1); free($3); }
    ;

for_update
    : TOK_BOOST TOK_IDENTIFIER
        { $$ = mkstr("%s++", $2); free($2); }
    | TOK_DEGRADE TOK_IDENTIFIER
        { $$ = mkstr("%s--", $2); free($2); }
    | TOK_IDENTIFIER TOK_STORE expr
        { $$ = mkstr("%s = %s", $1, $3); free($1); free($3); }
    ;

/* ── REPEAT … UNTIL (do-while) ──────────────────────────────────────────── */
do_header
    : TOK_REPEAT
        { print_indent(); fprintf(output, "do"); }
    ;

do_while_loop
    : do_header block TOK_UNTIL TOK_LPAREN expr TOK_RPAREN TOK_SEMICOLON
        { print_indent(); fprintf(output, "while (%s);\n", $5); free($5); }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * LOOP CONTROL
 * ════════════════════════════════════════════════════════════════════════════ */

loop_control_stmt
    : TOK_TERMINATE TOK_SEMICOLON
        { print_indent(); fprintf(output, "break;\n"); }
    | TOK_SKIP TOK_SEMICOLON
        { print_indent(); fprintf(output, "continue;\n"); }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * FUNCTION DECLARATIONS
 *
 *   FUNCTION data_type IDENTIFIER ( param_list ) block
 *   FUNCTION data_type IDENTIFIER () block
 * ════════════════════════════════════════════════════════════════════════════ */

func_header_params
    : TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN param_list TOK_RPAREN
        { fprintf(output, "%s %s(%s)", $2, $3, $5); free($2); free($3); free($5); }
    ;

func_header_void
    : TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN
        { fprintf(output, "%s %s(void)", $2, $3); free($2); free($3); }
    ;

function_decl
    : func_header_params block { fprintf(output, "\n"); }
    | func_header_void   block { fprintf(output, "\n"); }
    ;

param_list
    : param
        { $$ = $1; }
    | param_list TOK_COMMA param
        { char *t = mkstr("%s, %s", $1, $3); free($1); free($3); $$ = t; }
    ;

param
    : data_type TOK_IDENTIFIER
        { $$ = mkstr("%s %s", $1, $2); free($1); free($2); }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * RETURN STATEMENT
 * ════════════════════════════════════════════════════════════════════════════ */

return_stmt
    : TOK_RETURN_CARGO expr TOK_SEMICOLON
        { print_indent(); fprintf(output, "return %s;\n", $2); free($2); }
    | TOK_RETURN_CARGO TOK_SEMICOLON
        { print_indent(); fprintf(output, "return;\n"); }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * I/O STATEMENTS
 *
 *   TRANSMIT expr ;
 *     - if expr is a string literal → printf("…\n");
 *     - if expr contains '.'        → printf("%f\n", expr);
 *     - otherwise (default)         → printf("%d\n", expr);
 *
 *   RECEIVE IDENTIFIER ;            → scanf("%d", &IDENTIFIER);
 * ════════════════════════════════════════════════════════════════════════════ */

io_stmt
    : TOK_TRANSMIT expr TOK_SEMICOLON
        {
            print_indent();
            const char *e = $2;
            if (e[0] == '"') {
                /* Strip surrounding quotes and add \n inside the format str */
                size_t len = strlen(e);
                if (len >= 2 && e[len-1] == '"') {
                    char *inner = strndup(e + 1, len - 2);
                    fprintf(output, "printf(\"%s\\n\");\n", inner);
                    free(inner);
                } else {
                    /* Malformed string — emit as-is */
                    fprintf(output, "printf(%s);\n", e);
                }
            } else if (strchr(e, '.')) {
                /* Looks like a float expression */
                fprintf(output, "printf(\"%%f\\n\", %s);\n", e);
            } else {
                /* Default: integer */
                fprintf(output, "printf(\"%%d\\n\", %s);\n", e);
            }
            free($2);
        }
    | TOK_RECEIVE TOK_IDENTIFIER TOK_SEMICOLON
        {
            print_indent();
            fprintf(output, "scanf(\"%%d\", &%s);\n", $2);
            free($2);
        }
    ;

/* ════════════════════════════════════════════════════════════════════════════
 * EXPRESSION STATEMENT  (function call or standalone expression)
 * ════════════════════════════════════════════════════════════════════════════ */

expr_stmt
    : expr TOK_SEMICOLON
        { print_indent(); fprintf(output, "%s;\n", $1); free($1); }
    ;

%%

/* ════════════════════════════════════════════════════════════════════════════
 * MAIN — Drives the compiler
 *
 * The lexer's own main() must be compiled out; the integrated binary
 * uses this one.  The lexer's token_output can be set here if desired.
 * ════════════════════════════════════════════════════════════════════════════ */

int main(int argc, char **argv) {

    if (argc < 2) {
        printf(
            "SMPL Compiler — Space Mission Programming Language\n"
            "Author : MD. Jahid Hasan Jim | Roll: 2107054 | KUET CSE\n\n"
            "Usage  : %s <input.smpl> [output.c]\n"
            "\n"
            "  input.smpl  — SMPL source file\n"
            "  output.c    — (optional) destination C file; default: stdout\n",
            argv[0]);
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
    } else {
        output = stdout;
    }

    /* Suppress token diagnostics in the integrated compiler.
       Set to stderr (or a file) here if you want token tracing. */
    token_output = NULL;

    /* Emit the C standard header before any grammar action runs */
    fprintf(output, "#include <stdio.h>\n");

    /* Connect Flex to the source file and parse */
    yyin = src;
    int rc = yyparse();

    fclose(src);
    if (output != stdout) fclose(output);

    fprintf(stderr, rc == 0
        ? "Compilation successful.\n"
        : "Compilation failed — see errors above.\n");

    return rc;
}
