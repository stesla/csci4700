/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ARRAY = 258,
     GLOBAL = 259,
     READ = 260,
     WRITE = 261,
     IDENTIFIER = 262,
     CONSTANT = 263,
     STRING_LITERAL = 264,
     INC_OP = 265,
     DEC_OP = 266,
     LE_OP = 267,
     GE_OP = 268,
     EQ_OP = 269,
     NE_OP = 270,
     AND_OP = 271,
     OR_OP = 272,
     IF = 273,
     ELSE = 274,
     WHILE = 275,
     FOR = 276,
     RETURN = 277
   };
#endif
#define ARRAY 258
#define GLOBAL 259
#define READ 260
#define WRITE 261
#define IDENTIFIER 262
#define CONSTANT 263
#define STRING_LITERAL 264
#define INC_OP 265
#define DEC_OP 266
#define LE_OP 267
#define GE_OP 268
#define EQ_OP 269
#define NE_OP 270
#define AND_OP 271
#define OR_OP 272
#define IF 273
#define ELSE 274
#define WHILE 275
#define FOR 276
#define RETURN 277




/* Copy the first part of user declarations.  */
#line 1 "parse.y"


/* ===========================================================================

Parser template, supplied as a starting point for compiler
construction. Use and abuse as you wish.

Author:  Bill Mahoney
For:     CSCI4700

=========================================================================== */

#include        <iostream>       // Just plain needed...
#include        <iomanip>        // Needed for setw, hex, ...
#include        <fstream>        // Needed for ofstream type
#include        <stdio.h>        // Possibly needed
#include        <unistd.h>       // Possibly needed
#include        <stdlib.h>       // Possibly needed
#include        <ctype.h>        // Possibly needed
#include        <string.h>       // Possibly needed
#include        "parse.h"

using namespace std;

extern int  yydebug; // Use if you want the Bison generated debugging

int usage( void );
int yyerror( const char *msg );
int my_input( char *buf, int max_size );
void detab( char *line );
int yylex( void );
extern "C" int yywrap( void );

char		listing_line[ 132 ];	// for listings.	
int		err_count;		// # of errors.		
short		listing;		// true gives listing	
short           echo;                   // true echos input     
short           lex_debug;              // true debugs scanner  
short           line = 0;       	// For listings		
ifstream        infile;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 174 "parse.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   327

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  31
/* YYNRULES -- Number of rules. */
#define YYNRULES  88
/* YYNRULES -- Number of states. */
#define YYNSTATES  167

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   277

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,     2,     2,    37,    31,     2,
      23,    24,    32,    33,    25,    34,     2,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    30,
      38,    42,    39,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    26,     2,    27,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    28,    41,    29,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     8,    14,    16,    20,    22,    23,
      27,    29,    32,    36,    40,    45,    47,    50,    52,    55,
      59,    63,    68,    75,    77,    79,    81,    83,    85,    91,
      97,    99,   102,   108,   116,   122,   129,   137,   140,   144,
     146,   150,   152,   154,   156,   160,   162,   167,   170,   173,
     175,   178,   181,   184,   186,   188,   190,   192,   194,   196,
     200,   204,   208,   210,   214,   218,   220,   224,   228,   232,
     236,   238,   242,   246,   248,   252,   254,   258,   260,   264,
     266,   270,   272,   276,   278,   282,   287,   291,   293
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      44,     0,    -1,    45,    -1,    44,    45,    -1,     7,    23,
      46,    24,    48,    -1,    51,    -1,    46,    25,    47,    -1,
      47,    -1,    -1,     7,    26,    27,    -1,     7,    -1,    28,
      29,    -1,    28,    49,    29,    -1,    28,    50,    29,    -1,
      28,    50,    49,    29,    -1,    53,    -1,    49,    53,    -1,
      51,    -1,    50,    51,    -1,     3,    52,    30,    -1,     4,
      73,    30,    -1,     7,    26,     8,    27,    -1,    52,    25,
       7,    26,     8,    27,    -1,    48,    -1,    54,    -1,    55,
      -1,    56,    -1,    57,    -1,     5,    23,     7,    24,    30,
      -1,     6,    23,    59,    24,    30,    -1,    30,    -1,    58,
      30,    -1,    18,    23,    58,    24,    53,    -1,    18,    23,
      58,    24,    53,    19,    53,    -1,    20,    23,    58,    24,
      53,    -1,    21,    23,    54,    54,    24,    53,    -1,    21,
      23,    54,    54,    58,    24,    53,    -1,    22,    30,    -1,
      22,    58,    30,    -1,    72,    -1,    58,    25,    72,    -1,
       7,    -1,     8,    -1,     9,    -1,    23,    58,    24,    -1,
      59,    -1,    60,    26,    58,    27,    -1,    60,    10,    -1,
      60,    11,    -1,    60,    -1,    10,    61,    -1,    11,    61,
      -1,    62,    61,    -1,    31,    -1,    32,    -1,    33,    -1,
      34,    -1,    35,    -1,    61,    -1,    63,    32,    61,    -1,
      63,    36,    61,    -1,    63,    37,    61,    -1,    63,    -1,
      64,    33,    63,    -1,    64,    34,    63,    -1,    64,    -1,
      65,    38,    64,    -1,    65,    39,    64,    -1,    65,    12,
      64,    -1,    65,    13,    64,    -1,    65,    -1,    66,    14,
      65,    -1,    66,    15,    65,    -1,    66,    -1,    67,    31,
      66,    -1,    67,    -1,    68,    40,    67,    -1,    68,    -1,
      69,    41,    68,    -1,    69,    -1,    70,    16,    69,    -1,
      70,    -1,    71,    17,    70,    -1,    71,    -1,    60,    23,
      24,    -1,    60,    23,    73,    24,    -1,    61,    42,    72,
      -1,     7,    -1,    73,    25,     7,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,    64,    64,    65,    69,    70,    74,    75,    76,    80,
      81,    85,    86,    87,    88,    92,    93,    97,    98,   102,
     103,   107,   108,   112,   113,   114,   115,   116,   117,   118,
     122,   123,   127,   128,   132,   133,   134,   138,   139,   143,
     144,   148,   149,   150,   151,   155,   156,   157,   158,   162,
     163,   164,   165,   169,   170,   171,   172,   173,   177,   178,
     179,   180,   184,   185,   186,   190,   191,   192,   193,   194,
     198,   199,   200,   204,   205,   209,   210,   214,   215,   219,
     220,   224,   225,   232,   233,   234,   235,   239,   240
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ARRAY", "GLOBAL", "READ", "WRITE",
  "IDENTIFIER", "CONSTANT", "STRING_LITERAL", "INC_OP", "DEC_OP", "LE_OP",
  "GE_OP", "EQ_OP", "NE_OP", "AND_OP", "OR_OP", "IF", "ELSE", "WHILE",
  "FOR", "RETURN", "'('", "')'", "','", "'['", "']'", "'{'", "'}'", "';'",
  "'&'", "'*'", "'+'", "'-'", "'!'", "'/'", "'%'", "'<'", "'>'", "'^'",
  "'|'", "'='", "$accept", "translation_unit", "external_declaration",
  "formal_list", "formal", "block", "statement_list", "decl_list", "decl",
  "a_list", "statement", "expression_stmt", "selection_stmt",
  "iteration_stmt", "return_stmt", "expression", "primary_expression",
  "postfix_expression", "unary_expression", "unary_operator",
  "multiplicative_expression", "additive_expression",
  "relational_expression", "equality_expression", "and_expression",
  "exclusive_or_expression", "inclusive_or_expression",
  "logical_and_expression", "logical_or_expression",
  "assignment_expression", "identifier_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    40,    41,    44,    91,    93,   123,   125,
      59,    38,    42,    43,    45,    33,    47,    37,    60,    62,
      94,   124,    61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    43,    44,    44,    45,    45,    46,    46,    46,    47,
      47,    48,    48,    48,    48,    49,    49,    50,    50,    51,
      51,    52,    52,    53,    53,    53,    53,    53,    53,    53,
      54,    54,    55,    55,    56,    56,    56,    57,    57,    58,
      58,    59,    59,    59,    59,    60,    60,    60,    60,    61,
      61,    61,    61,    62,    62,    62,    62,    62,    63,    63,
      63,    63,    64,    64,    64,    65,    65,    65,    65,    65,
      66,    66,    66,    67,    67,    68,    68,    69,    69,    70,
      70,    71,    71,    72,    72,    72,    72,    73,    73
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     5,     1,     3,     1,     0,     3,
       1,     2,     3,     3,     4,     1,     2,     1,     2,     3,
       3,     4,     6,     1,     1,     1,     1,     1,     5,     5,
       1,     2,     5,     7,     5,     6,     7,     2,     3,     1,
       3,     1,     1,     1,     3,     1,     4,     2,     2,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     4,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     0,     0,     2,     5,     0,     0,    87,
       0,     8,     1,     3,     0,     0,    19,     0,    20,    10,
       0,     7,     0,     0,    88,     0,     0,     0,    21,     0,
       9,     0,     4,     6,     0,     0,     0,    41,    42,    43,
       0,     0,     0,     0,     0,     0,     0,    11,    30,    53,
      54,    55,    56,    57,    23,     0,     0,    17,    15,    24,
      25,    26,    27,     0,    45,    49,    58,     0,    62,    65,
      70,    73,    75,    77,    79,    81,    83,    39,    22,     0,
       0,    49,    50,    51,     0,     0,     0,    37,     0,     0,
      12,    16,    13,     0,    18,     0,    31,    47,    48,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    38,    44,    14,    40,    84,     0,
       0,    86,    59,    60,    61,    58,    63,    64,    68,    69,
      66,    67,    71,    72,    74,    76,    78,    80,    82,     0,
       0,     0,     0,     0,    85,    46,    28,    29,    32,    34,
       0,     0,     0,    35,     0,    33,    36
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     4,     5,    20,    21,    54,    55,    56,     6,     8,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      10
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -95
static const short yypact[] =
{
      47,    21,    46,    71,   146,   -95,   -95,   125,    68,   -95,
      70,   151,   -95,   -95,    94,   154,   -95,   179,   -95,   168,
      41,   -95,   166,   184,   -95,   185,   183,   151,   -95,   205,
     -95,   134,   -95,   -95,   187,   192,   194,   -95,   -95,   -95,
     292,   292,   199,   200,   201,    81,   292,   -95,   -95,   -95,
     -95,   -95,   -95,   -95,   -95,   198,   167,   -95,   -95,   -95,
     -95,   -95,   -95,    76,   -95,    29,   202,   292,    24,    53,
       6,   103,   210,   203,   204,   209,   225,   -95,   -95,   239,
      40,    73,   -95,   -95,   292,   292,   258,   -95,    78,    95,
     -95,   -95,   -95,   229,   -95,   292,   -95,   -95,   -95,    22,
     292,   292,   -95,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   224,
     230,   123,   135,   258,   -95,   -95,   -95,   -95,   -95,   155,
      37,   -95,   -95,   -95,   -95,   -95,    24,    24,    53,    53,
      53,    53,     6,     6,   103,   210,   203,   204,   209,   223,
     226,     2,     2,   263,   -95,   -95,   -95,   -95,   236,   -95,
       2,   157,     2,   -95,     2,   -95,   -95
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
     -95,   -95,   271,   -95,   249,   251,   222,   -95,   -30,   -95,
     -55,   -80,   -95,   -95,   -95,   -43,   227,   -36,    18,   -95,
      77,   -94,    79,   165,   169,   164,   188,   186,   -95,   -74,
     207
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      91,    57,    88,    89,    81,    81,   123,    35,    36,    37,
      38,    39,    40,    41,   138,   139,   140,   141,   108,   109,
      42,   127,    43,    44,    45,    46,    94,   131,     7,     9,
      31,    81,    48,    49,    50,    51,    52,    53,    91,    97,
      98,   121,   122,   153,   110,   111,   128,    37,    38,    39,
       1,     2,    99,     9,     3,   100,   103,   130,    82,    83,
     104,   105,    95,    46,   155,    26,    27,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    97,    98,   102,   106,   107,    37,    38,
      39,    40,    41,    15,    11,    17,   158,   159,    16,   100,
      18,    95,    22,    95,    46,   163,    96,   165,   124,   166,
     161,    87,    49,    50,    51,    52,    53,   112,   113,   125,
      95,   132,   133,   134,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,     1,     2,    35,
      36,    37,    38,    39,    40,    41,    12,   151,    95,     1,
       2,    14,    42,     3,    43,    44,    45,    46,    19,   152,
      95,    23,    31,    47,    48,    49,    50,    51,    52,    53,
       1,     2,    35,    36,    37,    38,    39,    40,    41,   154,
      17,   164,    95,   136,   137,    42,    24,    43,    44,    45,
      46,   142,   143,    28,    25,    31,    92,    48,    49,    50,
      51,    52,    53,    35,    36,    37,    38,    39,    40,    41,
      29,    31,    30,    34,    78,    79,    42,    80,    43,    44,
      45,    46,    84,    85,    86,   117,    31,    90,    48,    49,
      50,    51,    52,    53,    35,    36,    37,    38,    39,    40,
      41,   114,   118,   115,   101,   116,   119,    42,   149,    43,
      44,    45,    46,   156,   150,   162,   157,    31,   126,    48,
      49,    50,    51,    52,    53,    37,    38,    39,    40,    41,
      37,    38,    39,    40,    41,    13,    33,    32,    93,   144,
     146,    46,     0,     0,   145,     0,    46,   160,    48,    49,
      50,    51,    52,    53,    49,    50,    51,    52,    53,    37,
      38,    39,    40,    41,   148,   147,   129,   120,     0,     0,
       0,     0,     0,     0,     0,    46,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,    52,    53
};

static const short yycheck[] =
{
      55,    31,    45,    46,    40,    41,    86,     5,     6,     7,
       8,     9,    10,    11,   108,   109,   110,   111,    12,    13,
      18,    95,    20,    21,    22,    23,    56,   101,     7,     7,
      28,    67,    30,    31,    32,    33,    34,    35,    93,    10,
      11,    84,    85,   123,    38,    39,    24,     7,     8,     9,
       3,     4,    23,     7,     7,    26,    32,   100,    40,    41,
      36,    37,    25,    23,    27,    24,    25,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    10,    11,    67,    33,    34,     7,     8,
       9,    10,    11,    25,    23,    25,   151,   152,    30,    26,
      30,    25,     8,    25,    23,   160,    30,   162,    30,   164,
     153,    30,    31,    32,    33,    34,    35,    14,    15,    24,
      25,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,     3,     4,     5,
       6,     7,     8,     9,    10,    11,     0,    24,    25,     3,
       4,    26,    18,     7,    20,    21,    22,    23,     7,    24,
      25,     7,    28,    29,    30,    31,    32,    33,    34,    35,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    24,
      25,    24,    25,   106,   107,    18,     7,    20,    21,    22,
      23,   112,   113,    27,    26,    28,    29,    30,    31,    32,
      33,    34,    35,     5,     6,     7,     8,     9,    10,    11,
      26,    28,    27,     8,    27,    23,    18,    23,    20,    21,
      22,    23,    23,    23,    23,    16,    28,    29,    30,    31,
      32,    33,    34,    35,     5,     6,     7,     8,     9,    10,
      11,    31,    17,    40,    42,    41,     7,    18,    24,    20,
      21,    22,    23,    30,    24,    19,    30,    28,    29,    30,
      31,    32,    33,    34,    35,     7,     8,     9,    10,    11,
       7,     8,     9,    10,    11,     4,    27,    26,    56,   114,
     116,    23,    -1,    -1,   115,    -1,    23,    24,    30,    31,
      32,    33,    34,    35,    31,    32,    33,    34,    35,     7,
       8,     9,    10,    11,   118,   117,    99,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     7,    44,    45,    51,     7,    52,     7,
      73,    23,     0,    45,    26,    25,    30,    25,    30,     7,
      46,    47,     8,     7,     7,    26,    24,    25,    27,    26,
      27,    28,    48,    47,     8,     5,     6,     7,     8,     9,
      10,    11,    18,    20,    21,    22,    23,    29,    30,    31,
      32,    33,    34,    35,    48,    49,    50,    51,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    27,    23,
      23,    60,    61,    61,    23,    23,    23,    30,    58,    58,
      29,    53,    29,    49,    51,    25,    30,    10,    11,    23,
      26,    42,    61,    32,    36,    37,    33,    34,    12,    13,
      38,    39,    14,    15,    31,    40,    41,    16,    17,     7,
      59,    58,    58,    54,    30,    24,    29,    72,    24,    73,
      58,    72,    61,    61,    61,    61,    63,    63,    64,    64,
      64,    64,    65,    65,    66,    67,    68,    69,    70,    24,
      24,    24,    24,    54,    24,    27,    30,    30,    53,    53,
      24,    58,    19,    53,    24,    53,    53
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stdout, "%s ", Title);				\
      yysymprint (stdout, 					\
                  Token, Value);	\
      YYFPRINTF (stdout, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stdout, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stdout, " %d", *bottom);
  YYFPRINTF (stdout, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stdout, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stdout, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stdout, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stdout, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stdout, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stdout, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stdout, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stdout, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stdout, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
      
    }

/* Line 1000 of yacc.c.  */
#line 1240 "parse.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
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

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stdout, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 243 "parse.y"


/* ===========================================================================

MAIN

=========================================================================== */

int main( int ac, char *av[] )
{
    
    char *filename;
    int  i;

    if ( ac < 2 )
	exit( usage() );
    else
	for( i = 1; i < ac; i++ )
	    if ( av[ i ][ 0 ] == '-' )
		switch( av[ i ][ 1 ] )
		{
                    case 'e':       
                        echo = ! echo;
                        break;
                    case 'l':       
                        lex_debug = ! lex_debug;
                        break;
                    case 's':       
                        listing = ! listing;
                        break;
                    case 'y':  
                        yydebug = ! yydebug;
                        break;
                    default:
                        exit( usage() );
		}
	    else
		filename = av[ i ];
    
    infile.open( filename, ios::in );
    if ( ! infile )
    {
        cerr << "Can't open source file!\n";
	exit( 2 );
    }
    
    // We can test the return of yyparse, but I'll go ahead
    // and track an error count internally and use that.

    (void) yyparse();

    infile.close();

    if ( ! err_count )
    {
        cout << "Compiled OK\n";
        return( 0 );
    }
    else
    {
	cerr << "Completed with " << err_count << "errors.\n";
	return( 1 );
    }
    
}

/*===========================================================================

usage

Print usage message, curl up, and die. Basically we just return 1
always, as we are usually called as "exit( usage() )". So there.

Inputs:  None
Outputs: None
Returns: 1 always

=========================================================================== */

int usage( void )
{
    cout << "usage: pki_asm <file> [-e] [-l] [-m size ] [-p] [-s] [-y]\n"
         << "-e == turns on echo of input file\n"
         << "-l == turns on lex debug flag\n"
         << "-s == enable a listing of the program being compiled.\n"
         << "-y == turns on yydebug flag\n";
    return( 1 );
}

/* ===========================================================================

yyerror

This is called from within the parser when something is not matching a
grammar rule. It can also be called manually (see de_reference) to
generate an error for some other reason. 

Inputs:  None
Outputs: None
Returns: int?

=========================================================================== */

int yyerror( const char *msg )
{
    cerr << "Error: line " << line << ": " << listing_line << endl;
    cerr << msg << endl;
    err_count++;
    return( 0 );
}

/* ===========================================================================

yywrap

This function is called automatically when we tell the scanner that
the file is done. The purpose is to let the scanner know if there is
more input coming up (like from an additonal file) or not. In the case
of the assembler, we want to go through the file two times - once to
make the symbol table, once to do the dirty work. So the first time
we're called, rewind to the beginning of the file. Second time, tell
them that we're really done.

Inputs:  None
Outputs: None
Returns: 0 as an indication that there is more input (pass two for us)
         1 on a true end-of-file

=========================================================================== */

extern "C" {

int yywrap( void )
{
    return( 1 ); /* done! */
}

};

/* ===========================================================================

my_input

This function is dropped in in the place of the normal scanner input
function. The reason we do this is to allow us to count input lines,
generate listing output, and so on. To set this up, in the scanner
define YY_INPUT to call here instead if handling it internally. Then
whenever the scanner wants data we call here, read a line, return
it. At the end of file it is necessary to return a 0 to indicate "no
more".

Inputs:  buf - pointer to a place where the scanner wants the data
         max_size - the largest buffer that the scanner will accept
Outputs: buf - filled in with data from the input file (one byte at a
         time using this function, although the data is still buffered
	 internally to us, so it isn't too inefficient).
Returns: 0 on end-of-file
         N - number of bytes read into "buf" (always one for this
	 version)

=========================================================================== */

int my_input( unsigned char *buf, int max_size )
{

    if ( ! infile.eof() )
        infile.getline( listing_line, sizeof( listing_line ) );
    
    if ( infile.eof() )
    {
	listing_line[ 0 ] = '\0';
	*buf = '\0';
	return( 0 ); // A.k.a. YY_NULL 
    }
    else
    {
	char *s;
	// Getline tosses the newline, but we want it on there.
	// Various things depend on it (it is treated as a token).
	strcat( listing_line, "\n" );
	if ( listing )
        {
  	    detab( listing_line );
            cout << listing_line;
        }
	line++;
        // For some reason FLEX wants this as unsigned char, but
        // strcpy wants it as signed char...
        strcpy( (char *) buf, listing_line );
	return( strlen( listing_line ) );
    }
}

/* ===========================================================================

detab

Remove any tab characters from the input line and replace them with
spaces. Basically this is just here to make things line up regardless
of the tab settings on the terminal / printer / whatever. If it is not
needed, you can just nuke it.

Inputs:  line - the line to handle
Outputs: line - with tabs replaced by spaces
Returns: none

=========================================================================== */

void detab( char *line )
{
    
    static char	  temp[ BUFSIZ ];
    register char *s, *d;
    int		  col;

    col = 0; s = line; d = temp;
    while ( *s )
	if ( *s != '\t' )
	    *d++ = *s++, col++;
	else
	{
	    do	{
		*d++ = ' ';
		col++;
	    } while ( col % 8 );
	    s++;
	}
    *d = '\0';
    (void) strcpy( line, temp );
    
} 

