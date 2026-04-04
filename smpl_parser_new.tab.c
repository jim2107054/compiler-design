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
#line 1 "smpl_parser_new.y"

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


#line 125 "smpl_parser_new.tab.c"

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

#include "smpl_parser_new.tab.h"
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
  YYSYMBOL_block = 67,                     /* block  */
  YYSYMBOL_expr_stmt = 68,                 /* expr_stmt  */
  YYSYMBOL_declaration_stmt = 69,          /* declaration_stmt  */
  YYSYMBOL_data_type = 70,                 /* data_type  */
  YYSYMBOL_assignment_stmt = 71,           /* assignment_stmt  */
  YYSYMBOL_boost_stmt = 72,                /* boost_stmt  */
  YYSYMBOL_degrade_stmt = 73,              /* degrade_stmt  */
  YYSYMBOL_if_stmt = 74,                   /* if_stmt  */
  YYSYMBOL_switch_stmt = 75,               /* switch_stmt  */
  YYSYMBOL_case_list = 76,                 /* case_list  */
  YYSYMBOL_case_stmt = 77,                 /* case_stmt  */
  YYSYMBOL_while_loop = 78,                /* while_loop  */
  YYSYMBOL_for_loop = 79,                  /* for_loop  */
  YYSYMBOL_for_init = 80,                  /* for_init  */
  YYSYMBOL_for_update = 81,                /* for_update  */
  YYSYMBOL_do_while_loop = 82,             /* do_while_loop  */
  YYSYMBOL_loop_control_stmt = 83,         /* loop_control_stmt  */
  YYSYMBOL_io_stmt = 84,                   /* io_stmt  */
  YYSYMBOL_function_decl = 85,             /* function_decl  */
  YYSYMBOL_param_list = 86,                /* param_list  */
  YYSYMBOL_return_stmt = 87,               /* return_stmt  */
  YYSYMBOL_expr = 88,                      /* expr  */
  YYSYMBOL_primary_expr = 89,              /* primary_expr  */
  YYSYMBOL_unary_expr = 90,                /* unary_expr  */
  YYSYMBOL_binary_expr = 91,               /* binary_expr  */
  YYSYMBOL_function_call = 92,             /* function_call  */
  YYSYMBOL_arg_list = 93,                  /* arg_list  */
  YYSYMBOL_array_access = 94               /* array_access  */
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
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   558

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  227

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
       0,   154,   154,   161,   170,   177,   181,   191,   194,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   224,   233,   242,   247,   252,   257,   265,
     266,   267,   268,   269,   275,   280,   288,   300,   314,   318,
     322,   331,   338,   342,   349,   353,   359,   368,   377,   390,
     391,   392,   397,   398,   399,   400,   406,   415,   419,   428,
     433,   442,   447,   455,   460,   468,   472,   481,   482,   483,
     484,   485,   489,   493,   497,   502,   507,   512,   519,   526,
     530,   534,   538,   542,   546,   550,   554,   558,   562,   566,
     570,   574,   583,   588,   596,   600,   609
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
  "statement_list", "statement", "block", "expr_stmt", "declaration_stmt",
  "data_type", "assignment_stmt", "boost_stmt", "degrade_stmt", "if_stmt",
  "switch_stmt", "case_list", "case_stmt", "while_loop", "for_loop",
  "for_init", "for_update", "do_while_loop", "loop_control_stmt",
  "io_stmt", "function_decl", "param_list", "return_stmt", "expr",
  "primary_expr", "unary_expr", "binary_expr", "function_call", "arg_list",
  "array_access", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-99)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      14,   -99,   124,    22,   -99,    14,   -99,   -99,   -99,   -99,
     -99,   -99,   -13,   -99,   189,   -99,   -99,   -16,    -3,   -99,
     204,   362,   362,   362,   362,   362,    42,    52,   362,    -7,
      89,    93,   106,   284,    75,   110,   352,   108,   111,   -99,
     -99,   -99,   -99,    12,   362,   -99,   -99,   -99,   -99,   -99,
     -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,
     420,   -99,   -99,   -99,   -99,   -99,   237,     3,   -99,   134,
     124,    73,    47,   336,   336,   336,   336,   336,   115,   117,
     -99,   362,   362,   362,     7,   137,   -99,   -99,   -99,   451,
     123,   362,   362,   294,   362,     8,   362,   362,   362,   362,
     362,   362,   362,   362,   -99,   135,   181,   141,    64,   -99,
     144,    13,   362,   -99,   -99,   -99,   -99,   -99,   -99,   -99,
      18,    61,   396,   124,   175,   145,   161,   -99,   143,   404,
     459,   -99,   514,    69,    72,   -99,   -99,   -99,   -99,   -99,
      91,    91,   371,   238,   -99,   -99,   -99,   124,   181,   160,
     362,   -99,    83,   284,   195,   284,   170,   362,   362,   362,
     172,   173,   -99,   362,   -99,   209,   178,   -99,   185,   467,
     -99,    -5,   121,   -99,   230,   514,   498,   412,   -99,   -99,
     514,   362,   -99,   187,   -99,   284,   284,    98,   193,    86,
     -99,   362,     0,   196,   506,    16,   -99,   -99,   194,   197,
     -99,   -99,   -99,   514,   200,   208,   256,   214,   -99,   -99,
     213,   -99,   -99,   -99,   284,   -99,   -99,   362,   284,   362,
     284,   284,   514,   -99,   -36,   221,   -99
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,     0,     0,     7,     0,     5,    29,    30,    31,
      32,    33,     0,     1,     0,     7,     6,     0,     0,     7,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,     0,     8,    21,    22,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    20,    18,    19,
       0,    67,    68,    69,    70,    71,     0,     0,     3,     0,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,     0,    49,     0,    57,    58,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,     0,     0,     0,     0,    23,
       0,     0,     0,    79,    80,    81,    82,    83,    36,    37,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,    93,    94,     0,     0,    77,    84,    85,    86,    87,
      88,    89,    90,    91,     2,    62,    63,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,    92,    96,     0,    61,     0,     0,
      96,    38,     0,    47,     0,    50,     0,     0,    59,    60,
      95,     0,    64,     0,    26,     0,     0,     0,     0,     0,
      42,     0,    52,     0,     0,     0,    39,    40,     0,     0,
       7,    41,    43,    51,     0,     0,     0,     0,    56,    35,
       0,    27,     7,     7,    46,    54,    55,     0,     0,     0,
      44,    45,    53,    48,     0,     0,    28
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -99,   -99,   272,   -99,   -10,   -27,   -98,   -99,   -99,    -4,
     -99,   -99,   -99,   -99,   -99,   -99,   101,   -99,   -99,   -99,
     -99,   -99,   -99,   -99,   275,   -99,   -99,   -21,   -99,   -99,
     -99,   -99,    74,   -99
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,     4,     5,    14,    45,    46,    47,    48,    12,
      49,    50,    51,    52,    53,   189,   190,    54,    55,   125,
     207,    56,    57,    58,     6,   108,    59,    60,    61,    62,
      63,    64,   133,    65
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      73,    74,    75,    76,    77,    66,    85,    80,   145,    69,
       7,     8,     9,    10,    11,    89,    71,     1,   163,   123,
     204,   205,    13,    95,   225,    92,   150,   185,   186,   210,
      96,    97,    98,    99,   100,   101,   102,   103,    17,    67,
      96,    97,    98,    99,   100,   101,   102,   103,    81,    68,
     167,   206,   113,   114,   115,   116,   117,     2,   124,   106,
     120,   121,   122,   107,   135,   151,   110,    93,   211,    94,
     129,   130,   132,   134,   153,   136,   137,   138,   139,   140,
     141,   142,   143,    96,    97,    98,    99,   100,   101,   102,
     103,   152,   201,    78,    96,    97,    98,    99,   100,   101,
     102,   103,    93,    79,   112,    96,    97,    98,    99,   100,
     101,   102,   103,    96,    97,    98,    99,   154,   147,   156,
     148,   187,   188,   163,   111,   164,   171,    86,   173,   169,
     165,     7,     8,     9,    10,    11,   175,   176,   177,    19,
     109,   170,   180,   166,    82,   198,    20,   199,    83,    21,
      22,    23,    24,    25,    26,    27,   187,   188,   196,   197,
     194,    84,    87,    90,    28,    29,    91,   118,    30,   119,
     203,    31,    32,    33,   128,    34,    35,   126,    36,    37,
      38,    39,    40,    41,    42,    43,    19,   144,   157,    44,
     214,   223,   146,    18,    19,   149,   222,   158,   132,   160,
     172,    20,   220,   221,    21,    22,    23,    24,    25,    26,
      27,     7,     8,     9,    10,    11,   159,   168,    70,    28,
      29,   174,   181,    30,   178,   179,    31,    32,    33,   182,
      34,    35,   183,    36,    37,    38,    39,    40,    41,    42,
      43,   105,    19,   191,    44,   195,   200,   212,   208,    20,
     213,   215,    21,    22,    23,    24,    25,    26,    27,   216,
      96,    97,    98,    99,   100,   101,   102,    28,    29,   217,
     218,    30,   219,   226,    31,    32,    33,    15,    34,    35,
      16,    36,    37,    38,    39,    40,    41,    42,    43,    19,
     202,     0,    44,   224,     0,     0,    20,     0,     0,    21,
      22,    23,    24,    25,    26,    27,     0,     0,     0,    21,
      22,    23,    24,    25,    28,    29,     0,     0,    30,     0,
       0,    31,    32,    33,    28,    34,    35,     0,    36,    37,
      38,    39,    40,    41,    42,    43,     0,     0,     0,    44,
       0,    39,    40,    41,    42,    72,     0,     0,     0,    44,
     131,    21,    22,    23,    24,    25,     0,     0,    96,    97,
      98,    99,   100,   101,   102,   103,    28,    21,    22,    23,
      24,    25,     0,     0,     0,     0,     0,    21,    22,    23,
      24,    25,    28,    39,    40,    41,    42,    72,     0,     0,
       0,    44,    28,    96,    97,    98,    99,   100,   101,    39,
      40,    41,    42,    72,    88,     0,     0,    44,     0,    39,
      40,    41,    42,    72,     0,     0,     0,    44,    96,    97,
      98,    99,   100,   101,   102,   103,    96,    97,    98,    99,
     100,   101,   102,   103,    96,    97,    98,    99,   100,   101,
     102,   103,    96,    97,    98,    99,   100,   101,   102,   103,
       0,     0,   155,     0,     0,     0,     0,     0,     0,     0,
     161,     0,     0,     0,     0,     0,     0,     0,   193,     0,
       0,     0,   104,    96,    97,    98,    99,   100,   101,   102,
     103,    96,    97,    98,    99,   100,   101,   102,   103,    96,
      97,    98,    99,   100,   101,   102,   103,     0,     0,     0,
       0,     0,     0,   127,     0,     0,     0,     0,     0,     0,
       0,   162,     0,     0,     0,     0,     0,     0,     0,   184,
      96,    97,    98,    99,   100,   101,   102,   103,    96,    97,
      98,    99,   100,   101,   102,   103,    96,    97,    98,    99,
     100,   101,   102,   103,     0,     0,     0,     0,     0,     0,
     192,     0,     0,     0,     0,     0,     0,     0,   209
};

static const yytype_int16 yycheck[] =
{
      21,    22,    23,    24,    25,    15,    33,    28,   106,    19,
       7,     8,     9,    10,    11,    36,    20,     3,    54,    12,
      20,    21,     0,    44,    60,    13,    13,    32,    33,    13,
      22,    23,    24,    25,    26,    27,    28,    29,    51,    55,
      22,    23,    24,    25,    26,    27,    28,    29,    55,    52,
     148,    51,    73,    74,    75,    76,    77,    43,    51,    56,
      81,    82,    83,    67,    56,    52,    70,    55,    52,    57,
      91,    92,    93,    94,    56,    96,    97,    98,    99,   100,
     101,   102,   103,    22,    23,    24,    25,    26,    27,    28,
      29,   112,     6,    51,    22,    23,    24,    25,    26,    27,
      28,    29,    55,    51,    57,    22,    23,    24,    25,    26,
      27,    28,    29,    22,    23,    24,    25,    56,    54,   123,
      56,    35,    36,    54,    51,    56,   153,    52,   155,   150,
      58,     7,     8,     9,    10,    11,   157,   158,   159,     5,
       6,    58,   163,   147,    55,    47,    12,    49,    55,    15,
      16,    17,    18,    19,    20,    21,    35,    36,   185,   186,
     181,    55,    52,    55,    30,    31,    55,    52,    34,    52,
     191,    37,    38,    39,    51,    41,    42,    40,    44,    45,
      46,    47,    48,    49,    50,    51,     5,    52,    13,    55,
     200,   218,    51,     4,     5,    51,   217,    52,   219,    56,
       5,    12,   212,   213,    15,    16,    17,    18,    19,    20,
      21,     7,     8,     9,    10,    11,    55,    57,    14,    30,
      31,    51,    13,    34,    52,    52,    37,    38,    39,    51,
      41,    42,    47,    44,    45,    46,    47,    48,    49,    50,
      51,     4,     5,    13,    55,    58,    53,    53,    52,    12,
      53,    51,    15,    16,    17,    18,    19,    20,    21,    51,
      22,    23,    24,    25,    26,    27,    28,    30,    31,    13,
      56,    34,    59,    52,    37,    38,    39,     5,    41,    42,
       5,    44,    45,    46,    47,    48,    49,    50,    51,     5,
     189,    -1,    55,   219,    -1,    -1,    12,    -1,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    15,
      16,    17,    18,    19,    30,    31,    -1,    -1,    34,    -1,
      -1,    37,    38,    39,    30,    41,    42,    -1,    44,    45,
      46,    47,    48,    49,    50,    51,    -1,    -1,    -1,    55,
      -1,    47,    48,    49,    50,    51,    -1,    -1,    -1,    55,
      56,    15,    16,    17,    18,    19,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    30,    47,    48,    49,    50,    51,    -1,    -1,
      -1,    55,    30,    22,    23,    24,    25,    26,    27,    47,
      48,    49,    50,    51,    52,    -1,    -1,    55,    -1,    47,
      48,    49,    50,    51,    -1,    -1,    -1,    55,    22,    23,
      24,    25,    26,    27,    28,    29,    22,    23,    24,    25,
      26,    27,    28,    29,    22,    23,    24,    25,    26,    27,
      28,    29,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
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
       0,     3,    43,    62,    63,    64,    85,     7,     8,     9,
      10,    11,    70,     0,    65,    63,    85,    51,     4,     5,
      12,    15,    16,    17,    18,    19,    20,    21,    30,    31,
      34,    37,    38,    39,    41,    42,    44,    45,    46,    47,
      48,    49,    50,    51,    55,    66,    67,    68,    69,    71,
      72,    73,    74,    75,    78,    79,    82,    83,    84,    87,
      88,    89,    90,    91,    92,    94,    65,    55,    52,    65,
      14,    70,    51,    88,    88,    88,    88,    88,    51,    51,
      88,    55,    55,    55,    55,    66,    52,    52,    52,    88,
      55,    55,    13,    55,    57,    88,    22,    23,    24,    25,
      26,    27,    28,    29,    52,     4,    56,    70,    86,     6,
      70,    51,    57,    88,    88,    88,    88,    88,    52,    52,
      88,    88,    88,    12,    51,    80,    40,    52,    51,    88,
      88,    56,    88,    93,    88,    56,    88,    88,    88,    88,
      88,    88,    88,    88,    52,    67,    51,    54,    56,    51,
      13,    52,    88,    56,    56,    56,    70,    13,    52,    55,
      56,    56,    52,    54,    56,    58,    70,    67,    57,    88,
      58,    66,     5,    66,    51,    88,    88,    88,    52,    52,
      88,    13,    51,    47,    52,    32,    33,    35,    36,    76,
      77,    13,    52,    56,    88,    58,    66,    66,    47,    49,
      53,     6,    77,    88,    20,    21,    51,    81,    52,    52,
      13,    52,    53,    53,    65,    51,    51,    13,    56,    59,
      65,    65,    88,    66,    93,    60,    52
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    61,    62,    62,    63,    64,    64,    65,    65,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    67,    68,    69,    69,    69,    69,    70,
      70,    70,    70,    70,    71,    71,    72,    73,    74,    74,
      74,    75,    76,    76,    77,    77,    77,    78,    79,    80,
      80,    80,    81,    81,    81,    81,    82,    83,    83,    84,
      84,    85,    85,    86,    86,    87,    87,    88,    88,    88,
      88,    88,    89,    89,    89,    89,    89,    89,    90,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    92,    92,    93,    93,    94
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     4,     1,     1,     2,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     4,     6,     8,    12,     1,
       1,     1,     1,     1,     4,     7,     3,     3,     5,     7,
       7,     7,     1,     2,     4,     4,     3,     5,     9,     0,
       3,     5,     0,     3,     2,     2,     7,     2,     2,     5,
       5,     7,     6,     2,     4,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     3,     1,     3,     4
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
#line 155 "smpl_parser_new.y"
        {
            ASTNode *func_list_node = (yyvsp[-4].ast);
            ASTNode *main_body = ast_stmt_list((yyvsp[-2].ast), NULL);
            (yyval.ast) = ast_program(func_list_node, main_body);
            g_program_ast = (yyval.ast);
        }
#line 1703 "smpl_parser_new.tab.c"
    break;

  case 3: /* program: mission_header statement_list TOK_LANDING TOK_SEMICOLON  */
#line 162 "smpl_parser_new.y"
        {
            ASTNode *main_body = ast_stmt_list((yyvsp[-2].ast), NULL);
            (yyval.ast) = ast_program(NULL, main_body);
            g_program_ast = (yyval.ast);
        }
#line 1713 "smpl_parser_new.tab.c"
    break;

  case 4: /* mission_header: TOK_MISSION  */
#line 171 "smpl_parser_new.y"
        {
            (yyval.ast) = NULL; /* Mission header processed, no AST node needed */
        }
#line 1721 "smpl_parser_new.tab.c"
    break;

  case 5: /* function_list: function_decl  */
#line 178 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_stmt_list((yyvsp[0].ast), NULL);
        }
#line 1729 "smpl_parser_new.tab.c"
    break;

  case 6: /* function_list: function_list function_decl  */
#line 182 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_stmt_list((yyvsp[0].ast), (yyvsp[-1].ast));
        }
#line 1737 "smpl_parser_new.tab.c"
    break;

  case 7: /* statement_list: %empty  */
#line 191 "smpl_parser_new.y"
        {
            (yyval.ast) = NULL;
        }
#line 1745 "smpl_parser_new.tab.c"
    break;

  case 8: /* statement_list: statement_list statement  */
#line 195 "smpl_parser_new.y"
        {
            if ((yyvsp[-1].ast) == NULL) {
                (yyval.ast) = ast_stmt_list((yyvsp[0].ast), NULL);
            } else {
                (yyval.ast) = ast_stmt_list((yyvsp[0].ast), (yyvsp[-1].ast));
            }
        }
#line 1757 "smpl_parser_new.tab.c"
    break;

  case 9: /* statement: declaration_stmt  */
#line 205 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1763 "smpl_parser_new.tab.c"
    break;

  case 10: /* statement: assignment_stmt  */
#line 206 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1769 "smpl_parser_new.tab.c"
    break;

  case 11: /* statement: boost_stmt  */
#line 207 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1775 "smpl_parser_new.tab.c"
    break;

  case 12: /* statement: degrade_stmt  */
#line 208 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1781 "smpl_parser_new.tab.c"
    break;

  case 13: /* statement: if_stmt  */
#line 209 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1787 "smpl_parser_new.tab.c"
    break;

  case 14: /* statement: switch_stmt  */
#line 210 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1793 "smpl_parser_new.tab.c"
    break;

  case 15: /* statement: while_loop  */
#line 211 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1799 "smpl_parser_new.tab.c"
    break;

  case 16: /* statement: for_loop  */
#line 212 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1805 "smpl_parser_new.tab.c"
    break;

  case 17: /* statement: do_while_loop  */
#line 213 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1811 "smpl_parser_new.tab.c"
    break;

  case 18: /* statement: io_stmt  */
#line 214 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1817 "smpl_parser_new.tab.c"
    break;

  case 19: /* statement: return_stmt  */
#line 215 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1823 "smpl_parser_new.tab.c"
    break;

  case 20: /* statement: loop_control_stmt  */
#line 216 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1829 "smpl_parser_new.tab.c"
    break;

  case 21: /* statement: block  */
#line 217 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1835 "smpl_parser_new.tab.c"
    break;

  case 22: /* statement: expr_stmt  */
#line 218 "smpl_parser_new.y"
                          { (yyval.ast) = (yyvsp[0].ast); }
#line 1841 "smpl_parser_new.tab.c"
    break;

  case 23: /* block: TOK_LAUNCH statement_list TOK_ABORT  */
#line 225 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_block((yyvsp[-1].ast));
        }
#line 1849 "smpl_parser_new.tab.c"
    break;

  case 24: /* expr_stmt: expr TOK_SEMICOLON  */
#line 234 "smpl_parser_new.y"
        {
            (yyval.ast) = (yyvsp[-1].ast);
        }
#line 1857 "smpl_parser_new.tab.c"
    break;

  case 25: /* declaration_stmt: TOK_LOAD data_type TOK_IDENTIFIER TOK_SEMICOLON  */
#line 243 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_declaration((yyvsp[-2].type_val), (yyvsp[-1].sval), 0, NULL, NULL);
            free((yyvsp[-1].sval));
        }
#line 1866 "smpl_parser_new.tab.c"
    break;

  case 26: /* declaration_stmt: TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON  */
#line 248 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_declaration((yyvsp[-4].type_val), (yyvsp[-3].sval), 0, NULL, (yyvsp[-1].ast));
            free((yyvsp[-3].sval));
        }
#line 1875 "smpl_parser_new.tab.c"
    break;

  case 27: /* declaration_stmt: TOK_LOAD TOK_CARGO_ARRAY data_type TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_SEMICOLON  */
#line 253 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_declaration((yyvsp[-5].type_val), (yyvsp[-4].sval), 1, ast_int_literal((yyvsp[-2].ival)), NULL);
            free((yyvsp[-4].sval));
        }
#line 1884 "smpl_parser_new.tab.c"
    break;

  case 28: /* declaration_stmt: TOK_LOAD TOK_CARGO_ARRAY data_type TOK_IDENTIFIER TOK_LBRACKET TOK_INTEGER TOK_RBRACKET TOK_STORE TOK_LBRACE arg_list TOK_RBRACE TOK_SEMICOLON  */
#line 258 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_declaration((yyvsp[-9].type_val), (yyvsp[-8].sval), 1, ast_int_literal((yyvsp[-6].ival)), (yyvsp[-2].ast));
            free((yyvsp[-8].sval));
        }
#line 1893 "smpl_parser_new.tab.c"
    break;

  case 29: /* data_type: TOK_CARGO_INT  */
#line 265 "smpl_parser_new.y"
                         { (yyval.type_val) = SMPL_INT;    }
#line 1899 "smpl_parser_new.tab.c"
    break;

  case 30: /* data_type: TOK_CARGO_FLOAT  */
#line 266 "smpl_parser_new.y"
                         { (yyval.type_val) = SMPL_FLOAT;  }
#line 1905 "smpl_parser_new.tab.c"
    break;

  case 31: /* data_type: TOK_CARGO_CHAR  */
#line 267 "smpl_parser_new.y"
                         { (yyval.type_val) = SMPL_CHAR;   }
#line 1911 "smpl_parser_new.tab.c"
    break;

  case 32: /* data_type: TOK_CARGO_DOUBLE  */
#line 268 "smpl_parser_new.y"
                         { (yyval.type_val) = SMPL_DOUBLE; }
#line 1917 "smpl_parser_new.tab.c"
    break;

  case 33: /* data_type: TOK_VOID_SPACE  */
#line 269 "smpl_parser_new.y"
                         { (yyval.type_val) = SMPL_VOID;   }
#line 1923 "smpl_parser_new.tab.c"
    break;

  case 34: /* assignment_stmt: TOK_IDENTIFIER TOK_STORE expr TOK_SEMICOLON  */
#line 276 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_assignment((yyvsp[-3].sval), (yyvsp[-1].ast));
            free((yyvsp[-3].sval));
        }
#line 1932 "smpl_parser_new.tab.c"
    break;

  case 35: /* assignment_stmt: TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET TOK_STORE expr TOK_SEMICOLON  */
#line 281 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_array_assign((yyvsp[-6].sval), (yyvsp[-4].ast), (yyvsp[-1].ast));
            free((yyvsp[-6].sval));
        }
#line 1941 "smpl_parser_new.tab.c"
    break;

  case 36: /* boost_stmt: TOK_BOOST TOK_IDENTIFIER TOK_SEMICOLON  */
#line 289 "smpl_parser_new.y"
        {
            /* x++ equivalent: x = x + 1 */
            ASTNode *var = ast_identifier((yyvsp[-1].sval));
            ASTNode *one = ast_int_literal(1);
            ASTNode *add = ast_binary_op("+", var, one);
            (yyval.ast) = ast_assignment((yyvsp[-1].sval), add);
            free((yyvsp[-1].sval));
        }
#line 1954 "smpl_parser_new.tab.c"
    break;

  case 37: /* degrade_stmt: TOK_DEGRADE TOK_IDENTIFIER TOK_SEMICOLON  */
#line 301 "smpl_parser_new.y"
        {
            /* x-- equivalent: x = x - 1 */
            ASTNode *var = ast_identifier((yyvsp[-1].sval));
            ASTNode *one = ast_int_literal(1);
            ASTNode *sub = ast_binary_op("-", var, one);
            (yyval.ast) = ast_assignment((yyvsp[-1].sval), sub);
            free((yyvsp[-1].sval));
        }
#line 1967 "smpl_parser_new.tab.c"
    break;

  case 38: /* if_stmt: TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN statement  */
#line 315 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_if_stmt((yyvsp[-2].ast), (yyvsp[0].ast), NULL);
        }
#line 1975 "smpl_parser_new.tab.c"
    break;

  case 39: /* if_stmt: TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN statement TOK_ELSE_CHECK statement  */
#line 319 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_if_stmt((yyvsp[-4].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 1983 "smpl_parser_new.tab.c"
    break;

  case 40: /* if_stmt: TOK_CHECK_IF TOK_LPAREN expr TOK_RPAREN statement TOK_OTHERWISE statement  */
#line 323 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_if_stmt((yyvsp[-4].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 1991 "smpl_parser_new.tab.c"
    break;

  case 41: /* switch_stmt: TOK_PROTOCOL TOK_LPAREN expr TOK_RPAREN TOK_LAUNCH case_list TOK_ABORT  */
#line 332 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_switch_stmt((yyvsp[-4].ast), (yyvsp[-1].ast));
        }
#line 1999 "smpl_parser_new.tab.c"
    break;

  case 42: /* case_list: case_stmt  */
#line 339 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_stmt_list((yyvsp[0].ast), NULL);
        }
#line 2007 "smpl_parser_new.tab.c"
    break;

  case 43: /* case_list: case_list case_stmt  */
#line 343 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_stmt_list((yyvsp[0].ast), (yyvsp[-1].ast));
        }
#line 2015 "smpl_parser_new.tab.c"
    break;

  case 44: /* case_stmt: TOK_SCENARIO TOK_INTEGER TOK_COLON statement_list  */
#line 350 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_case_stmt(ast_int_literal((yyvsp[-2].ival)), (yyvsp[0].ast));
        }
#line 2023 "smpl_parser_new.tab.c"
    break;

  case 45: /* case_stmt: TOK_SCENARIO TOK_CHAR_LITERAL TOK_COLON statement_list  */
#line 354 "smpl_parser_new.y"
        {
            /* Convert char literal to int */
            (yyval.ast) = ast_case_stmt(ast_char_literal((yyvsp[-2].sval)[0]), (yyvsp[0].ast));
            free((yyvsp[-2].sval));
        }
#line 2033 "smpl_parser_new.tab.c"
    break;

  case 46: /* case_stmt: TOK_DEFAULT_PROTOCOL TOK_COLON statement_list  */
#line 360 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_default_case((yyvsp[0].ast));
        }
#line 2041 "smpl_parser_new.tab.c"
    break;

  case 47: /* while_loop: TOK_ORBIT_WHILE TOK_LPAREN expr TOK_RPAREN statement  */
#line 369 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_while_loop((yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 2049 "smpl_parser_new.tab.c"
    break;

  case 48: /* for_loop: TOK_SEQUENCE TOK_LPAREN for_init TOK_SEMICOLON expr TOK_SEMICOLON for_update TOK_RPAREN statement  */
#line 378 "smpl_parser_new.y"
        {
            /* For simplicity, parse init/update as strings and create simple nodes */
            /* In a full implementation, these should be proper AST sub-trees */
            ASTNode *init = ast_for_init((yyvsp[-6].sval));
            ASTNode *update = ast_for_update((yyvsp[-2].sval));
            (yyval.ast) = ast_for_loop(init, (yyvsp[-4].ast), update, (yyvsp[0].ast));
            if ((yyvsp[-6].sval)) free((yyvsp[-6].sval));
            if ((yyvsp[-2].sval)) free((yyvsp[-2].sval));
        }
#line 2063 "smpl_parser_new.tab.c"
    break;

  case 49: /* for_init: %empty  */
#line 390 "smpl_parser_new.y"
                                                 { (yyval.sval) = NULL; }
#line 2069 "smpl_parser_new.tab.c"
    break;

  case 50: /* for_init: TOK_IDENTIFIER TOK_STORE expr  */
#line 391 "smpl_parser_new.y"
                                                 { (yyval.sval) = strdup((yyvsp[-2].sval)); free((yyvsp[-2].sval)); }
#line 2075 "smpl_parser_new.tab.c"
    break;

  case 51: /* for_init: TOK_LOAD data_type TOK_IDENTIFIER TOK_STORE expr  */
#line 393 "smpl_parser_new.y"
        { (yyval.sval) = strdup((yyvsp[-2].sval)); free((yyvsp[-2].sval)); }
#line 2081 "smpl_parser_new.tab.c"
    break;

  case 52: /* for_update: %empty  */
#line 397 "smpl_parser_new.y"
                                                 { (yyval.sval) = NULL; }
#line 2087 "smpl_parser_new.tab.c"
    break;

  case 53: /* for_update: TOK_IDENTIFIER TOK_STORE expr  */
#line 398 "smpl_parser_new.y"
                                                 { (yyval.sval) = strdup((yyvsp[-2].sval)); free((yyvsp[-2].sval)); }
#line 2093 "smpl_parser_new.tab.c"
    break;

  case 54: /* for_update: TOK_BOOST TOK_IDENTIFIER  */
#line 399 "smpl_parser_new.y"
                                                 { (yyval.sval) = strdup((yyvsp[0].sval)); free((yyvsp[0].sval)); }
#line 2099 "smpl_parser_new.tab.c"
    break;

  case 55: /* for_update: TOK_DEGRADE TOK_IDENTIFIER  */
#line 400 "smpl_parser_new.y"
                                                 { (yyval.sval) = strdup((yyvsp[0].sval)); free((yyvsp[0].sval)); }
#line 2105 "smpl_parser_new.tab.c"
    break;

  case 56: /* do_while_loop: TOK_REPEAT statement TOK_UNTIL TOK_LPAREN expr TOK_RPAREN TOK_SEMICOLON  */
#line 407 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_do_while_loop((yyvsp[-2].ast), (yyvsp[-5].ast));
        }
#line 2113 "smpl_parser_new.tab.c"
    break;

  case 57: /* loop_control_stmt: TOK_TERMINATE TOK_SEMICOLON  */
#line 416 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_break();
        }
#line 2121 "smpl_parser_new.tab.c"
    break;

  case 58: /* loop_control_stmt: TOK_SKIP TOK_SEMICOLON  */
#line 420 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_continue();
        }
#line 2129 "smpl_parser_new.tab.c"
    break;

  case 59: /* io_stmt: TOK_RECEIVE TOK_LPAREN TOK_IDENTIFIER TOK_RPAREN TOK_SEMICOLON  */
#line 429 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_input(ast_identifier((yyvsp[-2].sval)));
            free((yyvsp[-2].sval));
        }
#line 2138 "smpl_parser_new.tab.c"
    break;

  case 60: /* io_stmt: TOK_TRANSMIT TOK_LPAREN expr TOK_RPAREN TOK_SEMICOLON  */
#line 434 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_output((yyvsp[-2].ast));
        }
#line 2146 "smpl_parser_new.tab.c"
    break;

  case 61: /* function_decl: TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN param_list TOK_RPAREN block  */
#line 443 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_function((yyvsp[-5].type_val), (yyvsp[-4].sval), (yyvsp[-2].ast), (yyvsp[0].ast));
            free((yyvsp[-4].sval));
        }
#line 2155 "smpl_parser_new.tab.c"
    break;

  case 62: /* function_decl: TOK_FUNCTION data_type TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN block  */
#line 448 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_function((yyvsp[-4].type_val), (yyvsp[-3].sval), NULL, (yyvsp[0].ast));
            free((yyvsp[-3].sval));
        }
#line 2164 "smpl_parser_new.tab.c"
    break;

  case 63: /* param_list: data_type TOK_IDENTIFIER  */
#line 456 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_param((yyvsp[-1].type_val), (yyvsp[0].sval));
            free((yyvsp[0].sval));
        }
#line 2173 "smpl_parser_new.tab.c"
    break;

  case 64: /* param_list: param_list TOK_COMMA data_type TOK_IDENTIFIER  */
#line 461 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_stmt_list(ast_param((yyvsp[-1].type_val), (yyvsp[0].sval)), (yyvsp[-3].ast));
            free((yyvsp[0].sval));
        }
#line 2182 "smpl_parser_new.tab.c"
    break;

  case 65: /* return_stmt: TOK_RETURN_CARGO expr TOK_SEMICOLON  */
#line 469 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_return((yyvsp[-1].ast));
        }
#line 2190 "smpl_parser_new.tab.c"
    break;

  case 66: /* return_stmt: TOK_RETURN_CARGO TOK_SEMICOLON  */
#line 473 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_return(NULL);
        }
#line 2198 "smpl_parser_new.tab.c"
    break;

  case 67: /* expr: primary_expr  */
#line 481 "smpl_parser_new.y"
                         { (yyval.ast) = (yyvsp[0].ast); }
#line 2204 "smpl_parser_new.tab.c"
    break;

  case 68: /* expr: unary_expr  */
#line 482 "smpl_parser_new.y"
                         { (yyval.ast) = (yyvsp[0].ast); }
#line 2210 "smpl_parser_new.tab.c"
    break;

  case 69: /* expr: binary_expr  */
#line 483 "smpl_parser_new.y"
                         { (yyval.ast) = (yyvsp[0].ast); }
#line 2216 "smpl_parser_new.tab.c"
    break;

  case 70: /* expr: function_call  */
#line 484 "smpl_parser_new.y"
                         { (yyval.ast) = (yyvsp[0].ast); }
#line 2222 "smpl_parser_new.tab.c"
    break;

  case 71: /* expr: array_access  */
#line 485 "smpl_parser_new.y"
                         { (yyval.ast) = (yyvsp[0].ast); }
#line 2228 "smpl_parser_new.tab.c"
    break;

  case 72: /* primary_expr: TOK_INTEGER  */
#line 490 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_int_literal((yyvsp[0].ival));
        }
#line 2236 "smpl_parser_new.tab.c"
    break;

  case 73: /* primary_expr: TOK_FLOAT_NUM  */
#line 494 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_float_literal((yyvsp[0].fval));
        }
#line 2244 "smpl_parser_new.tab.c"
    break;

  case 74: /* primary_expr: TOK_CHAR_LITERAL  */
#line 498 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_char_literal((yyvsp[0].sval)[0]);
            free((yyvsp[0].sval));
        }
#line 2253 "smpl_parser_new.tab.c"
    break;

  case 75: /* primary_expr: TOK_STRING_LITERAL  */
#line 503 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_string_literal((yyvsp[0].sval));
            free((yyvsp[0].sval));
        }
#line 2262 "smpl_parser_new.tab.c"
    break;

  case 76: /* primary_expr: TOK_IDENTIFIER  */
#line 508 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_identifier((yyvsp[0].sval));
            free((yyvsp[0].sval));
        }
#line 2271 "smpl_parser_new.tab.c"
    break;

  case 77: /* primary_expr: TOK_LPAREN expr TOK_RPAREN  */
#line 513 "smpl_parser_new.y"
        {
            (yyval.ast) = (yyvsp[-1].ast);
        }
#line 2279 "smpl_parser_new.tab.c"
    break;

  case 78: /* unary_expr: TOK_NEGATE expr  */
#line 520 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_unary_op("!", (yyvsp[0].ast));
        }
#line 2287 "smpl_parser_new.tab.c"
    break;

  case 79: /* binary_expr: TOK_COMBINE expr expr  */
#line 527 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("+", (yyvsp[-1].ast), (yyvsp[0].ast));
        }
#line 2295 "smpl_parser_new.tab.c"
    break;

  case 80: /* binary_expr: TOK_REDUCE expr expr  */
#line 531 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("-", (yyvsp[-1].ast), (yyvsp[0].ast));
        }
#line 2303 "smpl_parser_new.tab.c"
    break;

  case 81: /* binary_expr: TOK_AMPLIFY expr expr  */
#line 535 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("*", (yyvsp[-1].ast), (yyvsp[0].ast));
        }
#line 2311 "smpl_parser_new.tab.c"
    break;

  case 82: /* binary_expr: TOK_SPLIT expr expr  */
#line 539 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("/", (yyvsp[-1].ast), (yyvsp[0].ast));
        }
#line 2319 "smpl_parser_new.tab.c"
    break;

  case 83: /* binary_expr: TOK_REMAINDER expr expr  */
#line 543 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("%", (yyvsp[-1].ast), (yyvsp[0].ast));
        }
#line 2327 "smpl_parser_new.tab.c"
    break;

  case 84: /* binary_expr: expr TOK_EXCEEDS expr  */
#line 547 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op(">", (yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 2335 "smpl_parser_new.tab.c"
    break;

  case 85: /* binary_expr: expr TOK_BELOW expr  */
#line 551 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("<", (yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 2343 "smpl_parser_new.tab.c"
    break;

  case 86: /* binary_expr: expr TOK_EXCEEDS_OR_EQUAL expr  */
#line 555 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op(">=", (yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 2351 "smpl_parser_new.tab.c"
    break;

  case 87: /* binary_expr: expr TOK_BELOW_OR_EQUAL expr  */
#line 559 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("<=", (yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 2359 "smpl_parser_new.tab.c"
    break;

  case 88: /* binary_expr: expr TOK_MATCHES expr  */
#line 563 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("==", (yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 2367 "smpl_parser_new.tab.c"
    break;

  case 89: /* binary_expr: expr TOK_DIFFERS expr  */
#line 567 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("!=", (yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 2375 "smpl_parser_new.tab.c"
    break;

  case 90: /* binary_expr: expr TOK_BOTH expr  */
#line 571 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("&&", (yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 2383 "smpl_parser_new.tab.c"
    break;

  case 91: /* binary_expr: expr TOK_EITHER expr  */
#line 575 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_binary_op("||", (yyvsp[-2].ast), (yyvsp[0].ast));
        }
#line 2391 "smpl_parser_new.tab.c"
    break;

  case 92: /* function_call: TOK_IDENTIFIER TOK_LPAREN arg_list TOK_RPAREN  */
#line 584 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_func_call((yyvsp[-3].sval), (yyvsp[-1].ast));
            free((yyvsp[-3].sval));
        }
#line 2400 "smpl_parser_new.tab.c"
    break;

  case 93: /* function_call: TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN  */
#line 589 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_func_call((yyvsp[-2].sval), NULL);
            free((yyvsp[-2].sval));
        }
#line 2409 "smpl_parser_new.tab.c"
    break;

  case 94: /* arg_list: expr  */
#line 597 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_stmt_list((yyvsp[0].ast), NULL);
        }
#line 2417 "smpl_parser_new.tab.c"
    break;

  case 95: /* arg_list: arg_list TOK_COMMA expr  */
#line 601 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_stmt_list((yyvsp[0].ast), (yyvsp[-2].ast));
        }
#line 2425 "smpl_parser_new.tab.c"
    break;

  case 96: /* array_access: TOK_IDENTIFIER TOK_LBRACKET expr TOK_RBRACKET  */
#line 610 "smpl_parser_new.y"
        {
            (yyval.ast) = ast_array_access((yyvsp[-3].sval), (yyvsp[-1].ast));
            free((yyvsp[-3].sval));
        }
#line 2434 "smpl_parser_new.tab.c"
    break;


#line 2438 "smpl_parser_new.tab.c"

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

#line 616 "smpl_parser_new.y"


/* ═══════════════════════════════════════════════════════════════════════════
 * END OF GRAMMAR
 * ═══════════════════════════════════════════════════════════════════════════ */
