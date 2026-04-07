%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Include the Bison-generated header FIRST.
 * This provides the token #defines (TOK_MISSION = 256, etc.) and
 * declares YYSTYPE / yylval so we can fill semantic values.
 *
 * When building the lexer standalone (flex smpl_lexer.l only), this header
 * won't exist yet; wrap inside an availability guard if you need that mode.
 */
#include "smpl_parser_new.tab.h"

/* Line number tracking (yylineno from %option yylineno is the canonical one;
   we keep line_num as an alias used in output_token) */
int line_num = 1;

/* Output file for token diagnostics */
FILE *token_output;

/* ── Helper: print a token to the diagnostics file ─────────────────────── */
void output_token(int token, const char *lexeme) {
    if (token_output)
        fprintf(token_output, "Token: %-25d  Lexeme: %-25s  Line: %d\n",
                token, lexeme, yylineno);
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
"MISSION"               { output_token(TOK_MISSION, yytext);  return TOK_MISSION;  }
"LANDING"               { output_token(TOK_LANDING, yytext);  return TOK_LANDING;  }
"LAUNCH"                { output_token(TOK_LAUNCH,  yytext);  return TOK_LAUNCH;   }
"ABORT"                 { output_token(TOK_ABORT,   yytext);  return TOK_ABORT;    }

    /* Data Types */
"CARGO_INT"             { output_token(TOK_CARGO_INT,    yytext); return TOK_CARGO_INT;    }
"CARGO_FLOAT"           { output_token(TOK_CARGO_FLOAT,  yytext); return TOK_CARGO_FLOAT;  }
"CARGO_CHAR"            { output_token(TOK_CARGO_CHAR,   yytext); return TOK_CARGO_CHAR;   }
"CARGO_DOUBLE"          { output_token(TOK_CARGO_DOUBLE, yytext); return TOK_CARGO_DOUBLE; }
"VOID_SPACE"            { output_token(TOK_VOID_SPACE,   yytext); return TOK_VOID_SPACE;   }

    /* Declaration & Assignment */
"LOAD"                  { output_token(TOK_LOAD,        yytext); return TOK_LOAD;        }
"STORE"                 { output_token(TOK_STORE,       yytext); return TOK_STORE;       }
"CARGO_ARRAY"           { output_token(TOK_CARGO_ARRAY, yytext); return TOK_CARGO_ARRAY; }

    /* Arithmetic Operators */
"COMBINE"               { output_token(TOK_COMBINE,   yytext); return TOK_COMBINE;   }
"REDUCE"                { output_token(TOK_REDUCE,    yytext); return TOK_REDUCE;    }
"AMPLIFY"               { output_token(TOK_AMPLIFY,   yytext); return TOK_AMPLIFY;   }
"SPLIT"                 { output_token(TOK_SPLIT,     yytext); return TOK_SPLIT;     }
"REMAINDER"             { output_token(TOK_REMAINDER, yytext); return TOK_REMAINDER; }
"BOOST"                 { output_token(TOK_BOOST,     yytext); return TOK_BOOST;     }
"DEGRADE"               { output_token(TOK_DEGRADE,   yytext); return TOK_DEGRADE;   }

    /* Relational Operators — longer patterns first */
"EXCEEDS_OR_EQUAL"      { output_token(TOK_EXCEEDS_OR_EQUAL, yytext); return TOK_EXCEEDS_OR_EQUAL; }
"BELOW_OR_EQUAL"        { output_token(TOK_BELOW_OR_EQUAL,   yytext); return TOK_BELOW_OR_EQUAL;   }
"EXCEEDS"               { output_token(TOK_EXCEEDS, yytext); return TOK_EXCEEDS; }
"BELOW"                 { output_token(TOK_BELOW,   yytext); return TOK_BELOW;   }
"MATCHES"               { output_token(TOK_MATCHES, yytext); return TOK_MATCHES; }
"DIFFERS"               { output_token(TOK_DIFFERS, yytext); return TOK_DIFFERS; }

    /* Logical Operators */
"BOTH"                  { output_token(TOK_BOTH,   yytext); return TOK_BOTH;   }
"EITHER"                { output_token(TOK_EITHER, yytext); return TOK_EITHER; }
"NEGATE"                { output_token(TOK_NEGATE, yytext); return TOK_NEGATE; }

    /* Conditional Statements */
"CHECK_IF"              { output_token(TOK_CHECK_IF,   yytext); return TOK_CHECK_IF;   }
"ELSE_CHECK"            { output_token(TOK_ELSE_CHECK, yytext); return TOK_ELSE_CHECK; }
"OTHERWISE"             { output_token(TOK_OTHERWISE,  yytext); return TOK_OTHERWISE;  }

    /* Switch-Case — longer patterns first */
"DEFAULT_PROTOCOL"      { output_token(TOK_DEFAULT_PROTOCOL, yytext); return TOK_DEFAULT_PROTOCOL; }
"PROTOCOL"              { output_token(TOK_PROTOCOL, yytext); return TOK_PROTOCOL; }
"SCENARIO"              { output_token(TOK_SCENARIO, yytext); return TOK_SCENARIO; }

    /* Loop Constructs */
"ORBIT_WHILE"           { output_token(TOK_ORBIT_WHILE, yytext); return TOK_ORBIT_WHILE; }
"SEQUENCE"              { output_token(TOK_SEQUENCE,    yytext); return TOK_SEQUENCE;    }
"REPEAT"                { output_token(TOK_REPEAT,      yytext); return TOK_REPEAT;      }
"UNTIL"                 { output_token(TOK_UNTIL,       yytext); return TOK_UNTIL;       }

    /* Loop Control */
"TERMINATE"             { output_token(TOK_TERMINATE, yytext); return TOK_TERMINATE; }
"SKIP"                  { output_token(TOK_SKIP,      yytext); return TOK_SKIP;      }

    /* Functions */
"FUNCTION"              { output_token(TOK_FUNCTION,     yytext); return TOK_FUNCTION;     }
"RETURN_CARGO"          { output_token(TOK_RETURN_CARGO, yytext); return TOK_RETURN_CARGO; }

    /* I/O */
"RECEIVE"               { output_token(TOK_RECEIVE,  yytext); return TOK_RECEIVE;  }
"TRANSMIT"              { output_token(TOK_TRANSMIT, yytext); return TOK_TRANSMIT; }

    /* ==================== LITERALS ==================== */

    /* Float literal (must come before integer) */
{FLOAT}                 {
                            yylval.fval = atof(yytext);
                            output_token(TOK_FLOAT_NUM, yytext);
                            return TOK_FLOAT_NUM;
                        }

    /* Integer literal */
{INTEGER}               {
                            yylval.ival = atoi(yytext);
                            output_token(TOK_INTEGER, yytext);
                            return TOK_INTEGER;
                        }

    /* Character literal */
\'([^'\\]|\\.)?\'       {
                            yylval.sval = strdup(yytext);
                            output_token(TOK_CHAR_LITERAL, yytext);
                            return TOK_CHAR_LITERAL;
                        }

    /* String literal */
\"([^\"\\]|\\.)*\"      {
                            yylval.sval = strdup(yytext);
                            output_token(TOK_STRING_LITERAL, yytext);
                            return TOK_STRING_LITERAL;
                        }

    /* ==================== IDENTIFIERS ==================== */
{IDENTIFIER}            {
                            yylval.sval = strdup(yytext);
                            output_token(TOK_IDENTIFIER, yytext);
                            return TOK_IDENTIFIER;
                        }

    /* ==================== PUNCTUATION ==================== */
";"                     { output_token(TOK_SEMICOLON, ";"); return TOK_SEMICOLON; }
":"                     { output_token(TOK_COLON,     ":"); return TOK_COLON;     }
","                     { output_token(TOK_COMMA,     ","); return TOK_COMMA;     }
"("                     { output_token(TOK_LPAREN,    "("); return TOK_LPAREN;    }
")"                     { output_token(TOK_RPAREN,    ")"); return TOK_RPAREN;    }
"["                     { output_token(TOK_LBRACKET,  "["); return TOK_LBRACKET;  }
"]"                     { output_token(TOK_RBRACKET,  "]"); return TOK_RBRACKET;  }
"{"                     { output_token(TOK_LBRACE,    "{"); return TOK_LBRACE;    }
"}"                     { output_token(TOK_RBRACE,    "}"); return TOK_RBRACE;    }

    /* ==================== COMMENTS ==================== */
"//".*                  { /* Ignore single-line comments */ }

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
                            fprintf(stderr,
                                "Lexical Error at line %d: Unrecognized character '%s'\n",
                                yylineno, yytext);
                        }

%%

/*
 * NOTE: main() has been removed from this file.
 * The parser (smpl_parser.y) provides main() for the integrated compiler.
 *
 * token_output must be initialised by main() before yyparse() is called.
 * The parser's main() sets token_output = NULL (no token diagnostics) unless
 * you add a "--tokens" flag; feel free to extend it.
 */
