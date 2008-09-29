%{
#include <stdio.h>
#include "lexer_util.h"
#include "parser.h"
#include "y.tab.h"

%}

D           [0-9]
L           [a-zA-Z_]
H           [a-fA-F0-9]
E           [Ee][+-]?{D}+
FS          (f|F|l|L)
IS          (u|U|l|L)*

%option reentrant bison-bridge bison-locations

%%

{L}({L}|{D})* { return(identifier(yylval, yytext)); }
0[xX]{H}+{IS}? { return(constant(yylval, yytext)); }
0{D}+{IS}? { return(constant(yylval, yytext)); }
{D}+{IS}? { return(constant(yylval, yytext)); }
{D}+{E}{FS}? { return(constant(yylval, yytext)); }
L?\"(\\.|[^\\"])*\" { return(string_literal(yylval, yytext)); }
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

\n          { new_line(yylloc); }

"\/\/".*    { /* ignore comments */ }
[ \t\v\f]   { /* ignore whitespace */ }
.           { /* ignore bad characters */ }

%%
