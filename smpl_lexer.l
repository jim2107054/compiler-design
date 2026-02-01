%{
/*
 * SMPL (Space Mission Programming Language) Lexer
 * Flex specification file for lexical analysis
 * 
 * This lexer tokenizes SMPL source code and outputs tokens
 * in a format suitable for parsing with Bison.
 * 
 * Author: JIM2107054
 * Date: 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Token type enumeration */
typedef enum {
    /* Program Structure */
    TOK_MISSION = 256,
    TOK_LANDING,
    TOK_LAUNCH,
    TOK_ABORT,
    
    /* Data Types */
    TOK_CARGO_INT,
    TOK_CARGO_FLOAT,
    TOK_CARGO_CHAR,
    TOK_CARGO_DOUBLE,
    TOK_VOID_SPACE,
    
    /* Declaration & Assignment */
    TOK_LOAD,
    TOK_STORE,
    TOK_CARGO_ARRAY,
    
    /* Arithmetic Operators */
    TOK_COMBINE,
    TOK_REDUCE,
    TOK_AMPLIFY,
    TOK_SPLIT,
    TOK_REMAINDER,
    TOK_BOOST,
    TOK_DEGRADE,
    
    /* Relational Operators */
    TOK_EXCEEDS,
    TOK_BELOW,
    TOK_EXCEEDS_OR_EQUAL,
    TOK_BELOW_OR_EQUAL,
    TOK_MATCHES,
    TOK_DIFFERS,
    
    /* Logical Operators */
    TOK_BOTH,
    TOK_EITHER,
    TOK_NEGATE,
    
    /* Conditional Statements */
    TOK_CHECK_IF,
    TOK_ELSE_CHECK,
    TOK_OTHERWISE,
    
    /* Switch-Case */
    TOK_PROTOCOL,
    TOK_SCENARIO,
    TOK_DEFAULT_PROTOCOL,
    
    /* Loop Constructs */
    TOK_ORBIT_WHILE,
    TOK_SEQUENCE,
    TOK_REPEAT,
    TOK_UNTIL,
    
    /* Loop Control */
    TOK_TERMINATE,
    TOK_SKIP,
    
    /* Functions */
    TOK_FUNCTION,
    TOK_RETURN_CARGO,
    
    /* I/O */
    TOK_RECEIVE,
    TOK_TRANSMIT,
    
    /* Literals & Identifiers */
    TOK_INTEGER,
    TOK_FLOAT_NUM,
    TOK_CHAR_LITERAL,
    TOK_STRING_LITERAL,
    TOK_IDENTIFIER,
    
    /* Punctuation */
    TOK_SEMICOLON,
    TOK_COLON,
    TOK_COMMA,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_LBRACE,
    TOK_RBRACE
} TokenType;

/* Line number tracking */
int line_num = 1;

/* Output file */
FILE *token_output;

/* Function to get token name */
const char* get_token_name(int token) {
    switch(token) {
        case TOK_MISSION: return "MISSION";
        case TOK_LANDING: return "LANDING";
        case TOK_LAUNCH: return "LAUNCH";
        case TOK_ABORT: return "ABORT";
        case TOK_CARGO_INT: return "CARGO_INT";
        case TOK_CARGO_FLOAT: return "CARGO_FLOAT";
        case TOK_CARGO_CHAR: return "CARGO_CHAR";
        case TOK_CARGO_DOUBLE: return "CARGO_DOUBLE";
        case TOK_VOID_SPACE: return "VOID_SPACE";
        case TOK_LOAD: return "LOAD";
        case TOK_STORE: return "STORE";
        case TOK_CARGO_ARRAY: return "CARGO_ARRAY";
        case TOK_COMBINE: return "COMBINE";
        case TOK_REDUCE: return "REDUCE";
        case TOK_AMPLIFY: return "AMPLIFY";
        case TOK_SPLIT: return "SPLIT";
        case TOK_REMAINDER: return "REMAINDER";
        case TOK_BOOST: return "BOOST";
        case TOK_DEGRADE: return "DEGRADE";
        case TOK_EXCEEDS: return "EXCEEDS";
        case TOK_BELOW: return "BELOW";
        case TOK_EXCEEDS_OR_EQUAL: return "EXCEEDS_OR_EQUAL";
        case TOK_BELOW_OR_EQUAL: return "BELOW_OR_EQUAL";
        case TOK_MATCHES: return "MATCHES";
        case TOK_DIFFERS: return "DIFFERS";
        case TOK_BOTH: return "BOTH";
        case TOK_EITHER: return "EITHER";
        case TOK_NEGATE: return "NEGATE";
        case TOK_CHECK_IF: return "CHECK_IF";
        case TOK_ELSE_CHECK: return "ELSE_CHECK";
        case TOK_OTHERWISE: return "OTHERWISE";
        case TOK_PROTOCOL: return "PROTOCOL";
        case TOK_SCENARIO: return "SCENARIO";
        case TOK_DEFAULT_PROTOCOL: return "DEFAULT_PROTOCOL";
        case TOK_ORBIT_WHILE: return "ORBIT_WHILE";
        case TOK_SEQUENCE: return "SEQUENCE";
        case TOK_REPEAT: return "REPEAT";
        case TOK_UNTIL: return "UNTIL";
        case TOK_TERMINATE: return "TERMINATE";
        case TOK_SKIP: return "SKIP";
        case TOK_FUNCTION: return "FUNCTION";
        case TOK_RETURN_CARGO: return "RETURN_CARGO";
        case TOK_RECEIVE: return "RECEIVE";
        case TOK_TRANSMIT: return "TRANSMIT";
        case TOK_INTEGER: return "INTEGER";
        case TOK_FLOAT_NUM: return "FLOAT_NUM";
        case TOK_CHAR_LITERAL: return "CHAR_LITERAL";
        case TOK_STRING_LITERAL: return "STRING_LITERAL";
        case TOK_IDENTIFIER: return "IDENTIFIER";
        case TOK_SEMICOLON: return "SEMICOLON";
        case TOK_COLON: return "COLON";
        case TOK_COMMA: return "COMMA";
        case TOK_LPAREN: return "LPAREN";
        case TOK_RPAREN: return "RPAREN";
        case TOK_LBRACKET: return "LBRACKET";
        case TOK_RBRACKET: return "RBRACKET";
        case TOK_LBRACE: return "LBRACE";
        case TOK_RBRACE: return "RBRACE";
        default: return "UNKNOWN";
    }
}

/* Function to output token */
void output_token(int token, const char* lexeme) {
    fprintf(token_output, "%-20s %-25s %d\n", get_token_name(token), lexeme, line_num);
}

%}

/* Flex options */
%option noyywrap
%option yylineno

/* Regular expression definitions */
DIGIT       [0-9]
LETTER      [a-zA-Z]
IDENTIFIER  ({LETTER}|_)({LETTER}|{DIGIT}|_)*
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+"."{DIGIT}+
WHITESPACE  [ \t\r]+
NEWLINE     \n

%%

    /* ==================== KEYWORDS ==================== */

    /* Program Structure */
"MISSION"               { output_token(TOK_MISSION, yytext); return TOK_MISSION; }
"LANDING"               { output_token(TOK_LANDING, yytext); return TOK_LANDING; }
"LAUNCH"                { output_token(TOK_LAUNCH, yytext); return TOK_LAUNCH; }
"ABORT"                 { output_token(TOK_ABORT, yytext); return TOK_ABORT; }

    /* Data Types */
"CARGO_INT"             { output_token(TOK_CARGO_INT, yytext); return TOK_CARGO_INT; }
"CARGO_FLOAT"           { output_token(TOK_CARGO_FLOAT, yytext); return TOK_CARGO_FLOAT; }
"CARGO_CHAR"            { output_token(TOK_CARGO_CHAR, yytext); return TOK_CARGO_CHAR; }
"CARGO_DOUBLE"          { output_token(TOK_CARGO_DOUBLE, yytext); return TOK_CARGO_DOUBLE; }
"VOID_SPACE"            { output_token(TOK_VOID_SPACE, yytext); return TOK_VOID_SPACE; }

    /* Declaration & Assignment */
"LOAD"                  { output_token(TOK_LOAD, yytext); return TOK_LOAD; }
"STORE"                 { output_token(TOK_STORE, yytext); return TOK_STORE; }
"CARGO_ARRAY"           { output_token(TOK_CARGO_ARRAY, yytext); return TOK_CARGO_ARRAY; }

    /* Arithmetic Operators */
"COMBINE"               { output_token(TOK_COMBINE, yytext); return TOK_COMBINE; }
"REDUCE"                { output_token(TOK_REDUCE, yytext); return TOK_REDUCE; }
"AMPLIFY"               { output_token(TOK_AMPLIFY, yytext); return TOK_AMPLIFY; }
"SPLIT"                 { output_token(TOK_SPLIT, yytext); return TOK_SPLIT; }
"REMAINDER"             { output_token(TOK_REMAINDER, yytext); return TOK_REMAINDER; }
"BOOST"                 { output_token(TOK_BOOST, yytext); return TOK_BOOST; }
"DEGRADE"               { output_token(TOK_DEGRADE, yytext); return TOK_DEGRADE; }

    /* Relational Operators - longer patterns first */
"EXCEEDS_OR_EQUAL"      { output_token(TOK_EXCEEDS_OR_EQUAL, yytext); return TOK_EXCEEDS_OR_EQUAL; }
"BELOW_OR_EQUAL"        { output_token(TOK_BELOW_OR_EQUAL, yytext); return TOK_BELOW_OR_EQUAL; }
"EXCEEDS"               { output_token(TOK_EXCEEDS, yytext); return TOK_EXCEEDS; }
"BELOW"                 { output_token(TOK_BELOW, yytext); return TOK_BELOW; }
"MATCHES"               { output_token(TOK_MATCHES, yytext); return TOK_MATCHES; }
"DIFFERS"               { output_token(TOK_DIFFERS, yytext); return TOK_DIFFERS; }

    /* Logical Operators */
"BOTH"                  { output_token(TOK_BOTH, yytext); return TOK_BOTH; }
"EITHER"                { output_token(TOK_EITHER, yytext); return TOK_EITHER; }
"NEGATE"                { output_token(TOK_NEGATE, yytext); return TOK_NEGATE; }

    /* Conditional Statements */
"CHECK_IF"              { output_token(TOK_CHECK_IF, yytext); return TOK_CHECK_IF; }
"ELSE_CHECK"            { output_token(TOK_ELSE_CHECK, yytext); return TOK_ELSE_CHECK; }
"OTHERWISE"             { output_token(TOK_OTHERWISE, yytext); return TOK_OTHERWISE; }

    /* Switch-Case - longer patterns first */
"DEFAULT_PROTOCOL"      { output_token(TOK_DEFAULT_PROTOCOL, yytext); return TOK_DEFAULT_PROTOCOL; }
"PROTOCOL"              { output_token(TOK_PROTOCOL, yytext); return TOK_PROTOCOL; }
"SCENARIO"              { output_token(TOK_SCENARIO, yytext); return TOK_SCENARIO; }

    /* Loop Constructs */
"ORBIT_WHILE"           { output_token(TOK_ORBIT_WHILE, yytext); return TOK_ORBIT_WHILE; }
"SEQUENCE"              { output_token(TOK_SEQUENCE, yytext); return TOK_SEQUENCE; }
"REPEAT"                { output_token(TOK_REPEAT, yytext); return TOK_REPEAT; }
"UNTIL"                 { output_token(TOK_UNTIL, yytext); return TOK_UNTIL; }

    /* Loop Control */
"TERMINATE"             { output_token(TOK_TERMINATE, yytext); return TOK_TERMINATE; }
"SKIP"                  { output_token(TOK_SKIP, yytext); return TOK_SKIP; }

    /* Functions */
"FUNCTION"              { output_token(TOK_FUNCTION, yytext); return TOK_FUNCTION; }
"RETURN_CARGO"          { output_token(TOK_RETURN_CARGO, yytext); return TOK_RETURN_CARGO; }

    /* I/O */
"RECEIVE"               { output_token(TOK_RECEIVE, yytext); return TOK_RECEIVE; }
"TRANSMIT"              { output_token(TOK_TRANSMIT, yytext); return TOK_TRANSMIT; }

    /* ==================== LITERALS ==================== */

    /* Float literal (must come before integer) */
{FLOAT}                 { output_token(TOK_FLOAT_NUM, yytext); return TOK_FLOAT_NUM; }

    /* Integer literal */
{INTEGER}               { output_token(TOK_INTEGER, yytext); return TOK_INTEGER; }

    /* Character literal */
\'([^'\\]|\\.)?\'       { output_token(TOK_CHAR_LITERAL, yytext); return TOK_CHAR_LITERAL; }

    /* String literal */
\"([^"\\]|\\.)*\"       { output_token(TOK_STRING_LITERAL, yytext); return TOK_STRING_LITERAL; }

    /* ==================== IDENTIFIERS ==================== */
{IDENTIFIER}            { output_token(TOK_IDENTIFIER, yytext); return TOK_IDENTIFIER; }

    /* ==================== PUNCTUATION ==================== */
";"                     { output_token(TOK_SEMICOLON, ";"); return TOK_SEMICOLON; }
":"                     { output_token(TOK_COLON, ":"); return TOK_COLON; }
","                     { output_token(TOK_COMMA, ","); return TOK_COMMA; }
"("                     { output_token(TOK_LPAREN, "("); return TOK_LPAREN; }
")"                     { output_token(TOK_RPAREN, ")"); return TOK_RPAREN; }
"["                     { output_token(TOK_LBRACKET, "["); return TOK_LBRACKET; }
"]"                     { output_token(TOK_RBRACKET, "]"); return TOK_RBRACKET; }
"{"                     { output_token(TOK_LBRACE, "{"); return TOK_LBRACE; }
"}"                     { output_token(TOK_RBRACE, "}"); return TOK_RBRACE; }

    /* ==================== COMMENTS ==================== */
    /* Single-line comment */
"//".*                  { /* Ignore single-line comments */ }

    /* Multi-line comment */
"/*"([^*]|\*+[^*/])*\*+"/"  { 
                            /* Count newlines in multi-line comments */
                            char *p = yytext;
                            while (*p) {
                                if (*p == '\n') line_num++;
                                p++;
                            }
                        }

    /* ==================== WHITESPACE ==================== */
{WHITESPACE}            { /* Ignore whitespace */ }
{NEWLINE}               { line_num++; }

    /* ==================== ERROR HANDLING ==================== */
.                       { 
                            fprintf(stderr, "Lexical Error at line %d: Unrecognized character '%s'\n", line_num, yytext);
                        }

%%

/* Main function */
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <input_file> [output_file]\n", argv[0]);
        printf("  input_file  - SMPL source file to tokenize\n");
        printf("  output_file - (optional) Output file for tokens (default: stdout)\n");
        return 1;
    }
    
    /* Open input file */
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", argv[1]);
        return 1;
    }
    
    /* Open output file or use stdout */
    if (argc >= 3) {
        token_output = fopen(argv[2], "w");
        if (!token_output) {
            fprintf(stderr, "Error: Cannot open output file '%s'\n", argv[2]);
            fclose(input);
            return 1;
        }
    } else {
        token_output = stdout;
    }
    
    /* Print header */
    fprintf(token_output, "================================================================================\n");
    fprintf(token_output, "                    SMPL LEXICAL ANALYZER OUTPUT\n");
    fprintf(token_output, "================================================================================\n");
    fprintf(token_output, "Input file: %s\n", argv[1]);
    fprintf(token_output, "--------------------------------------------------------------------------------\n");
    fprintf(token_output, "%-20s %-25s %s\n", "TOKEN TYPE", "LEXEME", "LINE");
    fprintf(token_output, "--------------------------------------------------------------------------------\n");
    
    /* Set input */
    yyin = input;
    
    /* Tokenize */
    while (yylex() != 0);
    
    /* Print footer */
    fprintf(token_output, "--------------------------------------------------------------------------------\n");
    fprintf(token_output, "Lexical analysis completed. Total lines processed: %d\n", line_num);
    fprintf(token_output, "================================================================================\n");
    
    /* Cleanup */
    fclose(input);
    if (token_output != stdout) {
        fclose(token_output);
    }
    
    return 0;
}
