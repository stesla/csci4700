/* ===========================================================================

header file for sample Flex/Bison parser.

=========================================================================== */

#define	TRUE		1
#define	FALSE		0

#define SYM_LEN         32

struct  yystype {
    int       int_val;
    char      *str_ptr;
    char      str_val[ SYM_LEN + 1 ];
};

#ifdef  YYSTYPE
#undef  YYSTYPE
#endif
#define YYSTYPE struct yystype
