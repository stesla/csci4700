%{

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "input.h"
#include "parser.h"

int yywrap( void )
{
  return( 1 );
}

void yyerror(const char *s)
{
  fprintf(stderr, "%s:%i: error: %s\n", input_file(), input_lineno(), s);
}

%}

%token ARRAY GLOBAL READ WRITE

%token <text> IDENTIFIER CONSTANT STRING_LITERAL
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP
%token IF ELSE WHILE FOR RETURN

%type <node> identifier constant string_literal
%type <node> decl a_list identifier_list

%expect 1

%%

translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;

external_declaration
    : identifier '(' formal_list ')' block
    | decl
    ;

formal_list
        : formal_list ',' formal
        | formal
        |
        ;

formal
        : identifier '[' ']'
        | identifier
        ;

block
    : '{' '}'
    | '{' statement_list '}'
    | '{' decl_list '}'
    | '{' decl_list statement_list '}'
    ;

statement_list
    : statement
    | statement_list statement
    ;

decl_list
        : decl
        | decl_list decl
        ;

decl
    : ARRAY a_list ';' { $$ = ast_create(AST_DECLARE, $2); }
    | GLOBAL identifier_list ';' { $$ = ast_create(AST_DECLARE, $2); }
    ;

a_list
    : identifier '[' constant ']' {
      $$ = ast_create(AST_LIST, ast_create(AST_ARRAY, $1, $3), NULL); }
    | identifier '[' constant ']' ',' a_list {
      $$ = ast_create(AST_LIST, ast_create(AST_ARRAY, $1, $3), $6); }
    ;

identifier_list
    : identifier { $$ = ast_create(AST_LIST, $1, NULL); }
    | identifier ',' identifier_list { $$ = ast_create(AST_LIST, $1, $3); }
    ;

statement
    : block
    | expression_stmt
    | selection_stmt
    | iteration_stmt
    | return_stmt
    | READ '(' identifier ')' ';'
    | WRITE '(' primary_expression ')' ';'
    ;

expression_stmt
    : ';'
    | expression ';'
    ;

selection_stmt
    : IF '(' expression ')' statement
    | IF '(' expression ')' statement ELSE statement
    ;

iteration_stmt
    : WHILE '(' expression ')' statement
    | FOR '(' expression_stmt expression_stmt ')' statement
    | FOR '(' expression_stmt expression_stmt expression ')' statement
    ;

return_stmt
    : RETURN ';'
    | RETURN expression ';'
    ;

expression
    : assignment_expression
    | expression ',' assignment_expression
    ;

primary_expression
    : identifier
    | constant
    | string_literal
    | '(' expression ')' { $$ = $2; }
    ;

postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']'
    | postfix_expression INC_OP
    | postfix_expression DEC_OP
    ;

unary_expression
    : postfix_expression
    | INC_OP unary_expression
    | DEC_OP unary_expression
    | unary_operator unary_expression
    ;

unary_operator
    : '&'
    | '*'
    | '+'
    | '-'
    | '!'
    ;

multiplicative_expression
    : unary_expression
    | multiplicative_expression '*' unary_expression
    | multiplicative_expression '/' unary_expression
    | multiplicative_expression '%' unary_expression
    ;

additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;

relational_expression
    : additive_expression
    | relational_expression '<' additive_expression
    | relational_expression '>' additive_expression
    | relational_expression LE_OP additive_expression
    | relational_expression GE_OP additive_expression
    ;

equality_expression
    : relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;

and_expression
    : equality_expression
    | and_expression '&' equality_expression
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression AND_OP inclusive_or_expression
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression OR_OP logical_and_expression
    ;

// Modified this and moved the function calls here from postfix_expression so that one can NOT
// call a function in the middle of an expression; this is harder for students to implement
// because of the live registers in the expression.
assignment_expression
    : logical_or_expression
    | postfix_expression '(' ')'
    | postfix_expression '(' identifier_list ')'
    | unary_expression '=' assignment_expression
    ;

constant : CONSTANT { $$ = ast_create(AST_CONSTANT, $1); }
identifier : IDENTIFIER { $$ = ast_create(AST_IDENTIFIER, $1); }
string_literal : STRING_LITERAL { $$ = ast_create(AST_STRING_LITERAL, $1); }

%%
