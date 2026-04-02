/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SMPL_PARSER_TAB_H_INCLUDED
# define YY_YY_SMPL_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOK_MISSION = 258,             /* TOK_MISSION  */
    TOK_LANDING = 259,             /* TOK_LANDING  */
    TOK_LAUNCH = 260,              /* TOK_LAUNCH  */
    TOK_ABORT = 261,               /* TOK_ABORT  */
    TOK_CARGO_INT = 262,           /* TOK_CARGO_INT  */
    TOK_CARGO_FLOAT = 263,         /* TOK_CARGO_FLOAT  */
    TOK_CARGO_CHAR = 264,          /* TOK_CARGO_CHAR  */
    TOK_CARGO_DOUBLE = 265,        /* TOK_CARGO_DOUBLE  */
    TOK_VOID_SPACE = 266,          /* TOK_VOID_SPACE  */
    TOK_LOAD = 267,                /* TOK_LOAD  */
    TOK_STORE = 268,               /* TOK_STORE  */
    TOK_CARGO_ARRAY = 269,         /* TOK_CARGO_ARRAY  */
    TOK_COMBINE = 270,             /* TOK_COMBINE  */
    TOK_REDUCE = 271,              /* TOK_REDUCE  */
    TOK_AMPLIFY = 272,             /* TOK_AMPLIFY  */
    TOK_SPLIT = 273,               /* TOK_SPLIT  */
    TOK_REMAINDER = 274,           /* TOK_REMAINDER  */
    TOK_BOOST = 275,               /* TOK_BOOST  */
    TOK_DEGRADE = 276,             /* TOK_DEGRADE  */
    TOK_EXCEEDS = 277,             /* TOK_EXCEEDS  */
    TOK_BELOW = 278,               /* TOK_BELOW  */
    TOK_EXCEEDS_OR_EQUAL = 279,    /* TOK_EXCEEDS_OR_EQUAL  */
    TOK_BELOW_OR_EQUAL = 280,      /* TOK_BELOW_OR_EQUAL  */
    TOK_MATCHES = 281,             /* TOK_MATCHES  */
    TOK_DIFFERS = 282,             /* TOK_DIFFERS  */
    TOK_BOTH = 283,                /* TOK_BOTH  */
    TOK_EITHER = 284,              /* TOK_EITHER  */
    TOK_NEGATE = 285,              /* TOK_NEGATE  */
    TOK_CHECK_IF = 286,            /* TOK_CHECK_IF  */
    TOK_ELSE_CHECK = 287,          /* TOK_ELSE_CHECK  */
    TOK_OTHERWISE = 288,           /* TOK_OTHERWISE  */
    TOK_PROTOCOL = 289,            /* TOK_PROTOCOL  */
    TOK_SCENARIO = 290,            /* TOK_SCENARIO  */
    TOK_DEFAULT_PROTOCOL = 291,    /* TOK_DEFAULT_PROTOCOL  */
    TOK_ORBIT_WHILE = 292,         /* TOK_ORBIT_WHILE  */
    TOK_SEQUENCE = 293,            /* TOK_SEQUENCE  */
    TOK_REPEAT = 294,              /* TOK_REPEAT  */
    TOK_UNTIL = 295,               /* TOK_UNTIL  */
    TOK_TERMINATE = 296,           /* TOK_TERMINATE  */
    TOK_SKIP = 297,                /* TOK_SKIP  */
    TOK_FUNCTION = 298,            /* TOK_FUNCTION  */
    TOK_RETURN_CARGO = 299,        /* TOK_RETURN_CARGO  */
    TOK_RECEIVE = 300,             /* TOK_RECEIVE  */
    TOK_TRANSMIT = 301,            /* TOK_TRANSMIT  */
    TOK_INTEGER = 302,             /* TOK_INTEGER  */
    TOK_FLOAT_NUM = 303,           /* TOK_FLOAT_NUM  */
    TOK_CHAR_LITERAL = 304,        /* TOK_CHAR_LITERAL  */
    TOK_STRING_LITERAL = 305,      /* TOK_STRING_LITERAL  */
    TOK_IDENTIFIER = 306,          /* TOK_IDENTIFIER  */
    TOK_SEMICOLON = 307,           /* TOK_SEMICOLON  */
    TOK_COLON = 308,               /* TOK_COLON  */
    TOK_COMMA = 309,               /* TOK_COMMA  */
    TOK_LPAREN = 310,              /* TOK_LPAREN  */
    TOK_RPAREN = 311,              /* TOK_RPAREN  */
    TOK_LBRACKET = 312,            /* TOK_LBRACKET  */
    TOK_RBRACKET = 313,            /* TOK_RBRACKET  */
    TOK_LBRACE = 314,              /* TOK_LBRACE  */
    TOK_RBRACE = 315               /* TOK_RBRACE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 1238 "smpl_parser.y"

    int   ival;
    float fval;
    char *sval;
    struct { char *code; char *type; char *tac; } typed;
    struct ASTNode *ast;  /* AST node pointer */

#line 132 "smpl_parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SMPL_PARSER_TAB_H_INCLUDED  */
