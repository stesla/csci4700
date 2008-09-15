%{

/* ===========================================================================

Scanner for test grammar for CSCI4700 Fall 2005

=========================================================================== */

#include <stdio.h>
#include "parser.h"
#include "y.tab.h"

extern YYSTYPE yylval;
extern short line;
extern char listing_line[];

int is_keyword( const char *string );
%}

D           [0-9]
L           [a-zA-Z_]
H           [a-fA-F0-9]
E           [Ee][+-]?{D}+
FS          (f|F|l|L)
IS          (u|U|l|L)*

%%

"\/\/"      {
                    while ( input() != '\n' )
            ;
            if ( yy_flex_debug )
              printf("Removed comment from input stream.\n");
                }

{L}({L}|{D})*       {  return( is_keyword( yytext ) ); }
0[xX]{H}+{IS}?      {  return( CONSTANT ); }
0{D}+{IS}?      {  return( CONSTANT ); }
{D}+{IS}?       {  return( CONSTANT ); }
{D}+{E}{FS}?        {  return( CONSTANT ); }
L?\"(\\.|[^\\"])*\" {  return( STRING_LITERAL ); }
"++"            {  return( INC_OP ); }
"--"            {  return( DEC_OP ); }
"&&"            {  return( AND_OP ); }
"||"            {  return( OR_OP ); }
"<="            {  return( LE_OP ); }
">="            {  return( GE_OP ); }
"=="            {  return( EQ_OP ); }
"!="            {  return( NE_OP ); }
";"         {  return( ';' ); }
("{"|"<%")      {  return( '{' ); }
("}"|"%>")      {  return( '}' ); }
","         {  return( ',' ); }
":"         {  return( ':' ); }
"="         {  return( '=' ); }
"("         {  return( '(' ); }
")"         {  return( ')' ); }
("["|"<:")      {  return( '[' ); }
("]"|":>")      {  return( ']' ); }
"."         {  return( '.' ); }
"&"         {  return( '&' ); }
"!"         {  return( '!' ); }
"~"         {  return( '~' ); }
"-"         {  return( '-' ); }
"+"         {  return( '+' ); }
"*"         {  return( '*' ); }
"/"         {  return( '/' ); }
"%"         {  return( '%' ); }
"<"         {  return( '<' ); }
">"         {  return( '>' ); }
"^"         {  return( '^' ); }
"|"         {  return( '|' ); }
"?"         {  return( '?' ); }

[ \t\v\n\f]     {  }
.           { /* ignore bad characters */ }

%%

struct  tab     {
    char     word[ 9 ];
    int      value;
};

int is_keyword( const char *string )
{

    int             comp( const void *, const void * );
    struct tab      *ptr;
    static struct   tab     table[] = {
      { "array", ARRAY },
      { "else", ELSE },
      { "for", FOR },
      { "global", GLOBAL },
      { "if", IF },
      { "read", READ },
      { "return", RETURN },
      { "while", WHILE },
      { "write", WRITE }
    };

    ptr = (struct tab *) bsearch( string, (char *) &table[ 0 ], sizeof( table ) /
                  sizeof( table[ 0 ] ), sizeof( table[ 0 ] ), comp );
    if ( ptr )
    return( ptr -> value );
    else
    return( IDENTIFIER );
}

int comp( const void *a, const void *b )
{
    return( strcmp( ( ( struct tab *) a ) -> word,
            ( ( struct tab *) b ) -> word ) );
}
