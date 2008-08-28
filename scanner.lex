%{

/* ===========================================================================

Scanner for test grammar for CSCI4700 Fall 2005

=========================================================================== */

#include        <iostream>
#include        <iomanip>
#include        <stdlib.h>
#include        <string.h>
#include        "parse.h"
#include        "y.tab.h"

using namespace std;

extern YYSTYPE  yylval;
extern short	lex_debug, line;
extern char	listing_line[];

int is_keyword( const char *string );

// Define this so that it will call our input routine (my_input) instead
// of reading from stdin (cin) as a normal flex-generated scanner would.
int my_input( unsigned char *buf, int max_size );
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) result = my_input( (unsigned char *) buf, max_size );

// ----------------------------------------------------------------
// Here are the things I have removed from the original lex file...
// ----------------------------------------------------------------

%}

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*


%%

"\/\/"		{
		    // These are comments.
                    while ( yyinput() != '\n' )
			;
		    if ( lex_debug )
		        cout << "Removed comment from input stream.\n";
                }

{L}({L}|{D})*		{  return( is_keyword( yytext ) ); }
0[xX]{H}+{IS}?		{  return( CONSTANT ); }
0{D}+{IS}?		{  return( CONSTANT ); }
{D}+{IS}?		{  return( CONSTANT ); }
{D}+{E}{FS}?		{  return( CONSTANT ); }
L?\"(\\.|[^\\"])*\"	{  return( STRING_LITERAL ); }
"++"			{  return( INC_OP ); }
"--"			{  return( DEC_OP ); }
"&&"			{  return( AND_OP ); }
"||"			{  return( OR_OP ); }
"<="			{  return( LE_OP ); }
">="			{  return( GE_OP ); }
"=="			{  return( EQ_OP ); }
"!="			{  return( NE_OP ); }
";"			{  return( ';' ); }
("{"|"<%")		{  return( '{' ); }
("}"|"%>")		{  return( '}' ); }
","			{  return( ',' ); }
":"			{  return( ':' ); }
"="			{  return( '=' ); }
"("			{  return( '(' ); }
")"			{  return( ')' ); }
("["|"<:")		{  return( '[' ); }
("]"|":>")		{  return( ']' ); }
"."			{  return( '.' ); }
"&"			{  return( '&' ); }
"!"			{  return( '!' ); }
"~"			{  return( '~' ); }
"-"			{  return( '-' ); }
"+"			{  return( '+' ); }
"*"			{  return( '*' ); }
"/"			{  return( '/' ); }
"%"			{  return( '%' ); }
"<"			{  return( '<' ); }
">"			{  return( '>' ); }
"^"			{  return( '^' ); }
"|"			{  return( '|' ); }
"?"			{  return( '?' ); }

[ \t\v\n\f]		{  }
.			{ /* ignore bad characters */ }

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
{ "array",	ARRAY },
{ "else",	ELSE },
{ "for",	FOR },
{ "global",     GLOBAL },
{ "if",		IF },
{ "read",       READ },
{ "return",	RETURN },
{ "while",	WHILE },
{ "write",      WRITE }
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
