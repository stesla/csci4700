%{

/* ===========================================================================

Scanner for test grammar for CSCI4700 Fall 2005

=========================================================================== */

#include <stdio.h>
#include "lexer_util.h"
#include "parser.h"
#include "y.tab.h"

extern YYSTYPE yylval;
%}

D           [0-9]
L           [a-zA-Z_]
H           [a-fA-F0-9]
E           [Ee][+-]?{D}+
FS          (f|F|l|L)
IS          (u|U|l|L)*

%%

"\/\/"      { consume_comment(); }

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

/* Flex defines input() as static, so wrap it for lexer_util */
int next_char(void) { return(input()); }
