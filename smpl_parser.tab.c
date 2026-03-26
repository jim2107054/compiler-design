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


#line 170 "smpl_parser.tab.c"

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
       0,   191,   191,   193,   199,   208,   209,   217,   218,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   245,   250,   259,   260,   261,   262,
     263,   276,   282,   288,   295,   306,   308,   320,   326,   335,
     340,   356,   362,   368,   374,   380,   388,   389,   390,   391,
     392,   393,   396,   397,   400,   403,   405,   407,   409,   411,
     413,   422,   424,   426,   431,   433,   447,   452,   457,   462,
     463,   467,   468,   469,   480,   490,   495,   496,   501,   500,
     505,   504,   516,   521,   526,   535,   539,   541,   546,   548,
     550,   556,   561,   570,   572,   584,   589,   594,   595,   599,
     601,   606,   615,   617,   633,   657,   670
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
#line 192 "smpl_parser.y"
        { fprintf(output, "    return 0;\n}\n"); }
#line 1775 "smpl_parser.tab.c"
    break;

  case 3: /* program: mission_header statement_list TOK_LANDING TOK_SEMICOLON  */
#line 194 "smpl_parser.y"
        { fprintf(output, "    return 0;\n}\n"); }
#line 1781 "smpl_parser.tab.c"
    break;

  case 4: /* mission_header: TOK_MISSION  */
#line 200 "smpl_parser.y"
        {
            fprintf(output, "int main() {\n");
            indent_level = 1;
        }
#line 1790 "smpl_parser.tab.c"
    break;

  case 24: /* block_open: TOK_LAUNCH  */
#line 246 "smpl_parser.y"
        { fprintf(output, " {\n"); indent_level++; }
#line 1796 "smpl_parser.tab.c"
    break;

  case 25: /* block: block_open statement_list TOK_ABORT  */
#line 251 "smpl_parser.y"
        { indent_level--; print_indent(); fprintf(output, "}\n"); }
#line 1802 "smpl_parser.tab.c"
    break;

  case 26: /* data_type: TOK_CARGO_INT  */
#line 259 "smpl_parser.y"
                       { (yyval.sval) = safe_strdup("int");    }
#line 1808 "smpl_parser.tab.c"
    break;

  case 27: /* data_type: TOK_CARGO_FLOAT  */
#line 260 "smpl_parser.y"
                       { (yyval.sval) = safe_strdup("float");  }
#line 1814 "smpl_parser.tab.c"
    break;

  case 28: /* data_type: TOK_CARGO_CHAR  */
#line 261 "smpl_parser.y"
                       { (yyval.sval) = safe_strdup("char");   }
#line 1820 "smpl_parser.tab.c"
    break;

  case 29: /* data_type: TOK_CARGO_DOUBLE  */
#line 262 "smpl_parser.y"
                       { (yyval.sval) = safe_strdup("double"); }
#line 1826 "smpl_parser.tab.c"
    break;

  case 30: /* data_type: TOK_VOID_SPACE  */
#line 263 "smpl_parser.y"
                       { (yyval.sval) = safe_strdup("void");   }
#line 1832 "smpl_parser.tab.c"
    break;

  case 31: /* declaration_stmt: TOK_LOAD data_type TOK_IDENTIFIER TOK_SEMICOLON  */
#line 277 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "%s %s;\n", (yyvsp[-2].sval), (yyvsp[-1].sval));
            free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
        }
#line 1842 "smpl_parser.tab.c"
    break;

  case 32: /* declaration_stmt: TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON  */
#line 283 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "%s %s = %s;\n", (yyvsp[-4].sval), (yyvsp[-3].sval), (yyvsp[-1].sval));
            free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-1].sval));
        }
#line 1852 "smpl_parser.tab.c"
    break;

  case 33: /* declaration_stmt: TOK_LOAD data_type TOK_CARGO_ARRAY TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_SEMICOLON  */
#line 290 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "%s %s[%d];\n", (yyvsp[-6].sval), (yyvsp[-4].sval), (yyvsp[-2].ival));
            free((yyvsp[-6].sval)); free((yyvsp[-4].sval));
        }
#line 1862 "smpl_parser.tab.c"
    break;

  case 34: /* declaration_stmt: TOK_LOAD data_type TOK_CARGO_ARRAY TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_STORE TOK_LBRACE init_list TOK_RBRACE TOK_SEMICOLON  */
#line 298 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "%s %s[%d] = {%s};\n", (yyvsp[-10].sval), (yyvsp[-8].sval), (yyvsp[-6].ival), (yyvsp[-2].sval));
            free((yyvsp[-10].sval)); free((yyvsp[-8].sval)); free((yyvsp[-2].sval));
        }
#line 1872 "smpl_parser.tab.c"
    break;

  case 35: /* init_list: expr  */
#line 307 "smpl_parser.y"
        { (yyval.sval) = (yyvsp[0].sval); }
#line 1878 "smpl_parser.tab.c"
    break;

  case 36: /* init_list: init_list TOK_COMMA expr  */
#line 309 "smpl_parser.y"
        { char *t = mkstr("%s, %s", (yyvsp[-2].sval), (yyvsp[0].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].sval)); (yyval.sval) = t; }
#line 1884 "smpl_parser.tab.c"
    break;

  case 37: /* assignment_stmt: TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON  */
#line 321 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "%s = %s;\n", (yyvsp[-3].sval), (yyvsp[-1].sval));
            free((yyvsp[-3].sval)); free((yyvsp[-1].sval));
        }
#line 1894 "smpl_parser.tab.c"
    break;

  case 38: /* assignment_stmt: TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET TOK_STORE expr TOK_SEMICOLON  */
#line 327 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "%s[%s] = %s;\n", (yyvsp[-6].sval), (yyvsp[-4].sval), (yyvsp[-1].sval));
            free((yyvsp[-6].sval)); free((yyvsp[-4].sval)); free((yyvsp[-1].sval));
        }
#line 1904 "smpl_parser.tab.c"
    break;

  case 39: /* boost_stmt: TOK_BOOST TOK_IDENTIFIER TOK_SEMICOLON  */
#line 336 "smpl_parser.y"
        { print_indent(); fprintf(output, "%s++;\n", (yyvsp[-1].sval)); free((yyvsp[-1].sval)); }
#line 1910 "smpl_parser.tab.c"
    break;

  case 40: /* degrade_stmt: TOK_DEGRADE TOK_IDENTIFIER TOK_SEMICOLON  */
#line 341 "smpl_parser.y"
        { print_indent(); fprintf(output, "%s--;\n", (yyvsp[-1].sval)); free((yyvsp[-1].sval)); }
#line 1916 "smpl_parser.tab.c"
    break;

  case 41: /* expr: TOK_COMBINE expr expr  */
#line 357 "smpl_parser.y"
        {
            char *r;
            if (try_fold_int((yyvsp[-1].sval), (yyvsp[0].sval), '+', &r)) { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.sval) = r; }
            else { (yyval.sval) = mkstr("(%s + %s)", (yyvsp[-1].sval), (yyvsp[0].sval)); free((yyvsp[-1].sval)); free((yyvsp[0].sval)); }
        }
#line 1926 "smpl_parser.tab.c"
    break;

  case 42: /* expr: TOK_REDUCE expr expr  */
#line 363 "smpl_parser.y"
        {
            char *r;
            if (try_fold_int((yyvsp[-1].sval), (yyvsp[0].sval), '-', &r)) { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.sval) = r; }
            else { (yyval.sval) = mkstr("(%s - %s)", (yyvsp[-1].sval), (yyvsp[0].sval)); free((yyvsp[-1].sval)); free((yyvsp[0].sval)); }
        }
#line 1936 "smpl_parser.tab.c"
    break;

  case 43: /* expr: TOK_AMPLIFY expr expr  */
#line 369 "smpl_parser.y"
        {
            char *r;
            if (try_fold_int((yyvsp[-1].sval), (yyvsp[0].sval), '*', &r)) { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.sval) = r; }
            else { (yyval.sval) = mkstr("(%s * %s)", (yyvsp[-1].sval), (yyvsp[0].sval)); free((yyvsp[-1].sval)); free((yyvsp[0].sval)); }
        }
#line 1946 "smpl_parser.tab.c"
    break;

  case 44: /* expr: TOK_SPLIT expr expr  */
#line 375 "smpl_parser.y"
        {
            char *r;
            if (try_fold_int((yyvsp[-1].sval), (yyvsp[0].sval), '/', &r)) { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.sval) = r; }
            else { (yyval.sval) = mkstr("(%s / %s)", (yyvsp[-1].sval), (yyvsp[0].sval)); free((yyvsp[-1].sval)); free((yyvsp[0].sval)); }
        }
#line 1956 "smpl_parser.tab.c"
    break;

  case 45: /* expr: TOK_REMAINDER expr expr  */
#line 381 "smpl_parser.y"
        {
            char *r;
            if (try_fold_int((yyvsp[-1].sval), (yyvsp[0].sval), '%', &r)) { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.sval) = r; }
            else { (yyval.sval) = mkstr("(%s %% %s)", (yyvsp[-1].sval), (yyvsp[0].sval)); free((yyvsp[-1].sval)); free((yyvsp[0].sval)); }
        }
#line 1966 "smpl_parser.tab.c"
    break;

  case 46: /* expr: expr TOK_EXCEEDS expr  */
#line 388 "smpl_parser.y"
                                     { (yyval.sval) = mkstr("(%s > %s)",  (yyvsp[-2].sval),(yyvsp[0].sval)); free((yyvsp[-2].sval));free((yyvsp[0].sval)); }
#line 1972 "smpl_parser.tab.c"
    break;

  case 47: /* expr: expr TOK_BELOW expr  */
#line 389 "smpl_parser.y"
                                     { (yyval.sval) = mkstr("(%s < %s)",  (yyvsp[-2].sval),(yyvsp[0].sval)); free((yyvsp[-2].sval));free((yyvsp[0].sval)); }
#line 1978 "smpl_parser.tab.c"
    break;

  case 48: /* expr: expr TOK_EXCEEDS_OR_EQUAL expr  */
#line 390 "smpl_parser.y"
                                     { (yyval.sval) = mkstr("(%s >= %s)", (yyvsp[-2].sval),(yyvsp[0].sval)); free((yyvsp[-2].sval));free((yyvsp[0].sval)); }
#line 1984 "smpl_parser.tab.c"
    break;

  case 49: /* expr: expr TOK_BELOW_OR_EQUAL expr  */
#line 391 "smpl_parser.y"
                                     { (yyval.sval) = mkstr("(%s <= %s)", (yyvsp[-2].sval),(yyvsp[0].sval)); free((yyvsp[-2].sval));free((yyvsp[0].sval)); }
#line 1990 "smpl_parser.tab.c"
    break;

  case 50: /* expr: expr TOK_MATCHES expr  */
#line 392 "smpl_parser.y"
                                     { (yyval.sval) = mkstr("(%s == %s)", (yyvsp[-2].sval),(yyvsp[0].sval)); free((yyvsp[-2].sval));free((yyvsp[0].sval)); }
#line 1996 "smpl_parser.tab.c"
    break;

  case 51: /* expr: expr TOK_DIFFERS expr  */
#line 393 "smpl_parser.y"
                                     { (yyval.sval) = mkstr("(%s != %s)", (yyvsp[-2].sval),(yyvsp[0].sval)); free((yyvsp[-2].sval));free((yyvsp[0].sval)); }
#line 2002 "smpl_parser.tab.c"
    break;

  case 52: /* expr: expr TOK_BOTH expr  */
#line 396 "smpl_parser.y"
                          { (yyval.sval) = mkstr("(%s && %s)", (yyvsp[-2].sval),(yyvsp[0].sval)); free((yyvsp[-2].sval));free((yyvsp[0].sval)); }
#line 2008 "smpl_parser.tab.c"
    break;

  case 53: /* expr: expr TOK_EITHER expr  */
#line 397 "smpl_parser.y"
                           { (yyval.sval) = mkstr("(%s || %s)", (yyvsp[-2].sval),(yyvsp[0].sval)); free((yyvsp[-2].sval));free((yyvsp[0].sval)); }
#line 2014 "smpl_parser.tab.c"
    break;

  case 54: /* expr: TOK_NEGATE expr  */
#line 400 "smpl_parser.y"
                      { (yyval.sval) = mkstr("(!%s)", (yyvsp[0].sval)); free((yyvsp[0].sval)); }
#line 2020 "smpl_parser.tab.c"
    break;

  case 55: /* expr: TOK_IDENTIFIER  */
#line 404 "smpl_parser.y"
        { (yyval.sval) = safe_strdup((yyvsp[0].sval)); free((yyvsp[0].sval)); }
#line 2026 "smpl_parser.tab.c"
    break;

  case 56: /* expr: TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET  */
#line 406 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s[%s]", (yyvsp[-3].sval), (yyvsp[-1].sval)); free((yyvsp[-3].sval)); free((yyvsp[-1].sval)); }
#line 2032 "smpl_parser.tab.c"
    break;

  case 57: /* expr: TOK_IDENTIFIER TOK_LPAREN arg_list TOK_RPAREN  */
#line 408 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s(%s)", (yyvsp[-3].sval), (yyvsp[-1].sval)); free((yyvsp[-3].sval)); free((yyvsp[-1].sval)); }
#line 2038 "smpl_parser.tab.c"
    break;

  case 58: /* expr: TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN  */
#line 410 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s()", (yyvsp[-2].sval)); free((yyvsp[-2].sval)); }
#line 2044 "smpl_parser.tab.c"
    break;

  case 59: /* expr: TOK_INTEGER  */
#line 412 "smpl_parser.y"
        { (yyval.sval) = mkstr("%d", (yyvsp[0].ival)); }
#line 2050 "smpl_parser.tab.c"
    break;

  case 60: /* expr: TOK_FLOAT_NUM  */
#line 414 "smpl_parser.y"
        {
            char buf[64];
            snprintf(buf, sizeof buf, "%g", (double)(yyvsp[0].fval));
            /* Ensure it contains a decimal point so it reads as a float */
            if (!strchr(buf, '.') && !strchr(buf, 'e'))
                strncat(buf, ".0", sizeof buf - strlen(buf) - 1);
            (yyval.sval) = strdup(buf);
        }
#line 2063 "smpl_parser.tab.c"
    break;

  case 61: /* expr: TOK_CHAR_LITERAL  */
#line 423 "smpl_parser.y"
        { (yyval.sval) = safe_strdup((yyvsp[0].sval)); free((yyvsp[0].sval)); }
#line 2069 "smpl_parser.tab.c"
    break;

  case 62: /* expr: TOK_STRING_LITERAL  */
#line 425 "smpl_parser.y"
        { (yyval.sval) = safe_strdup((yyvsp[0].sval)); free((yyvsp[0].sval)); }
#line 2075 "smpl_parser.tab.c"
    break;

  case 63: /* expr: TOK_LPAREN expr TOK_RPAREN  */
#line 427 "smpl_parser.y"
        { (yyval.sval) = mkstr("(%s)", (yyvsp[-1].sval)); free((yyvsp[-1].sval)); }
#line 2081 "smpl_parser.tab.c"
    break;

  case 64: /* arg_list: expr  */
#line 432 "smpl_parser.y"
        { (yyval.sval) = (yyvsp[0].sval); }
#line 2087 "smpl_parser.tab.c"
    break;

  case 65: /* arg_list: arg_list TOK_COMMA expr  */
#line 434 "smpl_parser.y"
        { char *t = mkstr("%s, %s", (yyvsp[-2].sval), (yyvsp[0].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].sval)); (yyval.sval) = t; }
#line 2093 "smpl_parser.tab.c"
    break;

  case 66: /* if_header: TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN  */
#line 448 "smpl_parser.y"
        { print_indent(); fprintf(output, "if (%s)", (yyvsp[-1].sval)); free((yyvsp[-1].sval)); }
#line 2099 "smpl_parser.tab.c"
    break;

  case 67: /* else_if_header: TOK_ELSE_CHECK TOK_LPAREN expr TOK_RPAREN  */
#line 453 "smpl_parser.y"
        { print_indent(); fprintf(output, "else if (%s)", (yyvsp[-1].sval)); free((yyvsp[-1].sval)); }
#line 2105 "smpl_parser.tab.c"
    break;

  case 68: /* otherwise_header: TOK_OTHERWISE  */
#line 458 "smpl_parser.y"
        { print_indent(); fprintf(output, "else"); }
#line 2111 "smpl_parser.tab.c"
    break;

  case 74: /* switch_header: TOK_PROTOCOL TOK_LPAREN expr TOK_RPAREN TOK_LAUNCH  */
#line 481 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "switch (%s) {\n", (yyvsp[-2].sval));
            free((yyvsp[-2].sval));
            indent_level++;
        }
#line 2122 "smpl_parser.tab.c"
    break;

  case 75: /* switch_stmt: switch_header case_list TOK_ABORT  */
#line 491 "smpl_parser.y"
        { indent_level--; print_indent(); fprintf(output, "}\n"); }
#line 2128 "smpl_parser.tab.c"
    break;

  case 78: /* $@1: %empty  */
#line 501 "smpl_parser.y"
        { print_indent(); fprintf(output, "case %d:\n", (yyvsp[-1].ival)); indent_level++; }
#line 2134 "smpl_parser.tab.c"
    break;

  case 79: /* case_item: TOK_SCENARIO TOK_INTEGER TOK_COLON $@1 statement_list  */
#line 503 "smpl_parser.y"
        { indent_level--; }
#line 2140 "smpl_parser.tab.c"
    break;

  case 80: /* $@2: %empty  */
#line 505 "smpl_parser.y"
        { print_indent(); fprintf(output, "default:\n"); indent_level++; }
#line 2146 "smpl_parser.tab.c"
    break;

  case 81: /* case_item: TOK_DEFAULT_PROTOCOL TOK_COLON $@2 statement_list  */
#line 507 "smpl_parser.y"
        { indent_level--; }
#line 2152 "smpl_parser.tab.c"
    break;

  case 82: /* while_header: TOK_ORBIT_WHILE TOK_LPAREN expr TOK_RPAREN  */
#line 517 "smpl_parser.y"
        { print_indent(); fprintf(output, "while (%s)", (yyvsp[-1].sval)); free((yyvsp[-1].sval)); }
#line 2158 "smpl_parser.tab.c"
    break;

  case 84: /* for_header: TOK_SEQUENCE TOK_LPAREN for_init TOK_SEMICOLON expr TOK_SEMICOLON for_update TOK_RPAREN  */
#line 527 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "for (%s; %s; %s)", (yyvsp[-5].sval), (yyvsp[-3].sval), (yyvsp[-1].sval));
            free((yyvsp[-5].sval)); free((yyvsp[-3].sval)); free((yyvsp[-1].sval));
        }
#line 2168 "smpl_parser.tab.c"
    break;

  case 86: /* for_init: TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr  */
#line 540 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s %s = %s", (yyvsp[-3].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].sval)); }
#line 2174 "smpl_parser.tab.c"
    break;

  case 87: /* for_init: TOK_IDENTIFIER TOK_STORE expr  */
#line 542 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s = %s", (yyvsp[-2].sval), (yyvsp[0].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].sval)); }
#line 2180 "smpl_parser.tab.c"
    break;

  case 88: /* for_update: TOK_BOOST TOK_IDENTIFIER  */
#line 547 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s++", (yyvsp[0].sval)); free((yyvsp[0].sval)); }
#line 2186 "smpl_parser.tab.c"
    break;

  case 89: /* for_update: TOK_DEGRADE TOK_IDENTIFIER  */
#line 549 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s--", (yyvsp[0].sval)); free((yyvsp[0].sval)); }
#line 2192 "smpl_parser.tab.c"
    break;

  case 90: /* for_update: TOK_IDENTIFIER TOK_STORE expr  */
#line 551 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s = %s", (yyvsp[-2].sval), (yyvsp[0].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].sval)); }
#line 2198 "smpl_parser.tab.c"
    break;

  case 91: /* do_header: TOK_REPEAT  */
#line 557 "smpl_parser.y"
        { print_indent(); fprintf(output, "do"); }
#line 2204 "smpl_parser.tab.c"
    break;

  case 92: /* do_while_loop: do_header block TOK_UNTIL TOK_LPAREN expr TOK_RPAREN TOK_SEMICOLON  */
#line 562 "smpl_parser.y"
        { print_indent(); fprintf(output, "while (%s);\n", (yyvsp[-2].sval)); free((yyvsp[-2].sval)); }
#line 2210 "smpl_parser.tab.c"
    break;

  case 93: /* loop_control_stmt: TOK_TERMINATE TOK_SEMICOLON  */
#line 571 "smpl_parser.y"
        { print_indent(); fprintf(output, "break;\n"); }
#line 2216 "smpl_parser.tab.c"
    break;

  case 94: /* loop_control_stmt: TOK_SKIP TOK_SEMICOLON  */
#line 573 "smpl_parser.y"
        { print_indent(); fprintf(output, "continue;\n"); }
#line 2222 "smpl_parser.tab.c"
    break;

  case 95: /* func_header_params: TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN param_list TOK_RPAREN  */
#line 585 "smpl_parser.y"
        { fprintf(output, "%s %s(%s)", (yyvsp[-4].sval), (yyvsp[-3].sval), (yyvsp[-1].sval)); free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-1].sval)); }
#line 2228 "smpl_parser.tab.c"
    break;

  case 96: /* func_header_void: TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN  */
#line 590 "smpl_parser.y"
        { fprintf(output, "%s %s(void)", (yyvsp[-3].sval), (yyvsp[-2].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); }
#line 2234 "smpl_parser.tab.c"
    break;

  case 97: /* function_decl: func_header_params block  */
#line 594 "smpl_parser.y"
                               { fprintf(output, "\n"); }
#line 2240 "smpl_parser.tab.c"
    break;

  case 98: /* function_decl: func_header_void block  */
#line 595 "smpl_parser.y"
                               { fprintf(output, "\n"); }
#line 2246 "smpl_parser.tab.c"
    break;

  case 99: /* param_list: param  */
#line 600 "smpl_parser.y"
        { (yyval.sval) = (yyvsp[0].sval); }
#line 2252 "smpl_parser.tab.c"
    break;

  case 100: /* param_list: param_list TOK_COMMA param  */
#line 602 "smpl_parser.y"
        { char *t = mkstr("%s, %s", (yyvsp[-2].sval), (yyvsp[0].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].sval)); (yyval.sval) = t; }
#line 2258 "smpl_parser.tab.c"
    break;

  case 101: /* param: data_type TOK_IDENTIFIER  */
#line 607 "smpl_parser.y"
        { (yyval.sval) = mkstr("%s %s", (yyvsp[-1].sval), (yyvsp[0].sval)); free((yyvsp[-1].sval)); free((yyvsp[0].sval)); }
#line 2264 "smpl_parser.tab.c"
    break;

  case 102: /* return_stmt: TOK_RETURN_CARGO expr TOK_SEMICOLON  */
#line 616 "smpl_parser.y"
        { print_indent(); fprintf(output, "return %s;\n", (yyvsp[-1].sval)); free((yyvsp[-1].sval)); }
#line 2270 "smpl_parser.tab.c"
    break;

  case 103: /* return_stmt: TOK_RETURN_CARGO TOK_SEMICOLON  */
#line 618 "smpl_parser.y"
        { print_indent(); fprintf(output, "return;\n"); }
#line 2276 "smpl_parser.tab.c"
    break;

  case 104: /* io_stmt: TOK_TRANSMIT expr TOK_SEMICOLON  */
#line 634 "smpl_parser.y"
        {
            print_indent();
            const char *e = (yyvsp[-1].sval);
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
            free((yyvsp[-1].sval));
        }
#line 2304 "smpl_parser.tab.c"
    break;

  case 105: /* io_stmt: TOK_RECEIVE TOK_IDENTIFIER TOK_SEMICOLON  */
#line 658 "smpl_parser.y"
        {
            print_indent();
            fprintf(output, "scanf(\"%%d\", &%s);\n", (yyvsp[-1].sval));
            free((yyvsp[-1].sval));
        }
#line 2314 "smpl_parser.tab.c"
    break;

  case 106: /* expr_stmt: expr TOK_SEMICOLON  */
#line 671 "smpl_parser.y"
        { print_indent(); fprintf(output, "%s;\n", (yyvsp[-1].sval)); free((yyvsp[-1].sval)); }
#line 2320 "smpl_parser.tab.c"
    break;


#line 2324 "smpl_parser.tab.c"

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

#line 674 "smpl_parser.y"


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
