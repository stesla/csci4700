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
%type <node> decl_list decl a_list identifier_list
%type <node> formal_list formal
%type <node> primary_expression postfix_expression unary_expression
%type <op> unary_operator

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
    : formal ',' formal_list { $$ = ast_create(AST_LIST, $1, $3); }
    | formal { $$ = ast_create(AST_LIST, $1, NULL); }
    | { $$ = ast_create(AST_LIST, NULL, NULL); }
    ;

formal
    : identifier '[' ']' { $$ = ast_create(AST_FORMAL, $1, TRUE); }
    | identifier { $$ = ast_create(AST_FORMAL, $1, FALSE); }
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
    : decl { $$ = ast_create(AST_LIST, $1, NULL); }
    | decl decl_list { $$ = ast_create(AST_LIST, $1, $2); }
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
    | '(' expression ')' { $$ = NULL; /* TODO: fix this */ }
    ;

postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']' { $$ = NULL; /* TODO: fix this */ }
    | postfix_expression INC_OP { $$ = ast_create(AST_POSTFIX, $1, AST_OP_PLUS_PLUS); }
    | postfix_expression DEC_OP { $$ = ast_create(AST_POSTFIX, $1, AST_OP_MINUS_MINUS); }
    ;

unary_expression
    : postfix_expression
    | INC_OP unary_expression { $$ = ast_create(AST_PREFIX, $2, AST_OP_PLUS_PLUS); }
    | DEC_OP unary_expression { $$ = ast_create(AST_PREFIX, $2, AST_OP_MINUS_MINUS); }
    | unary_operator unary_expression { $$ = ast_create(AST_PREFIX, $2, $1); }
    ;

unary_operator
    : '&' { $$ = AST_OP_AMP; }
    | '*' { $$ = AST_OP_STAR; }
    | '+' { $$ = AST_OP_PLUS; }
    | '-' { $$ = AST_OP_MINUS; }
    | '!' { $$ = AST_OP_BANG; }
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
