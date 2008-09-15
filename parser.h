#ifndef PARSER_H
#define PARSER_H

#define TRUE        1
#define FALSE       0

#define SYM_LEN         32

struct  yystype {
  int       integer;
  char      *string;
  char      symbol[ SYM_LEN + 1 ];
};

#ifdef  YYSTYPE
#undef  YYSTYPE
#endif
#define YYSTYPE struct yystype

#endif PARSER_H
