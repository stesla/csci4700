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
%type <node> multiplicative_expression additive_expression
%type <node> relational_expression equality_expression and_expression
%type <node> exclusive_or_expression inclusive_or_expression
%type <node> logical_and_expression logical_or_expression
%type <node> assignment_expression expression return_stmt
%type <node> block expression_stmt selection_stmt iteration_stmt statement
%type <node> statement_list external_declaration translation_unit
%type <op> unary_operator

%expect 1

%%

translation_unit
    : external_declaration { $$ = ast_create(AST_LIST, $1, NULL); }
    | external_declaration translation_unit { $$ = ast_create(AST_LIST, $1, $2); }
    ;

external_declaration
    : identifier '(' formal_list ')' block { $$ = ast_create(AST_FUNCTION, $1, $3, $5); }
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
    : '{' '}' { $$ = ast_create(AST_BLOCK, NULL, NULL); }
    | '{' statement_list '}' { $$ = ast_create(AST_BLOCK, NULL, $2); }
    | '{' decl_list '}' { $$ = ast_create(AST_BLOCK, $2, NULL); }
    | '{' decl_list statement_list '}' { $$ = ast_create(AST_BLOCK, $2, $3); }
    ;

statement_list
    : statement { $$ = ast_create(AST_LIST, $1, NULL); }
    | statement statement_list { $$ = ast_create(AST_LIST, $1, $2); }
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
    : identifier '[' constant ']' { $$ = ast_create(AST_LIST, ast_create(AST_ARRAY, $1, $3), NULL); }
    | identifier '[' constant ']' ',' a_list { $$ = ast_create(AST_LIST, ast_create(AST_ARRAY, $1, $3), $6); }
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
    | READ '(' identifier ')' ';' { $$ = ast_create(AST_READ, $3); }
    | WRITE '(' primary_expression ')' ';' { $$ = ast_create(AST_WRITE, $3); }
    ;

expression_stmt
    : ';' { $$ = NULL; }
    | expression ';' { $$ = $1; }
    ;

selection_stmt
    : IF '(' expression ')' statement { $$ = ast_create(AST_CONDITIONAL, $3, $5, NULL); }
    | IF '(' expression ')' statement ELSE statement { $$ = ast_create(AST_CONDITIONAL, $3, $5, $7); }
    ;

iteration_stmt
    : WHILE '(' expression ')' statement { $$ = ast_create(AST_WHILE, $3, $5); }
    | FOR '(' expression_stmt expression_stmt ')' statement { $$ = ast_create(AST_FOR, $3, $4, NULL, $6); }
    | FOR '(' expression_stmt expression_stmt expression ')' statement { $$ = ast_create(AST_FOR, $3, $4, $5, $7); }
    ;

return_stmt
    : RETURN ';' { $$ = ast_create(AST_RETURN, NULL); }
    | RETURN expression ';' { $$ = ast_create(AST_RETURN, $2); }
    ;

expression
    : assignment_expression { $$ = ast_create(AST_LIST, $1, NULL); }
    | assignment_expression ',' expression { $$ = ast_create(AST_LIST, $1, $3); }
    ;

primary_expression
    : identifier
    | constant
    | string_literal
    | '(' expression ')' { $$ = ast_create(AST_GROUP, $2); }
    ;

postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']' { $$ = ast_create(AST_ARRAY, $1, $3); }
    | postfix_expression INC_OP { $$ = ast_create(AST_POSTFIX, $1, AST_OP_INC); }
    | postfix_expression DEC_OP { $$ = ast_create(AST_POSTFIX, $1, AST_OP_DEC); }
    ;

unary_expression
    : postfix_expression
    | INC_OP unary_expression { $$ = ast_create(AST_PREFIX, $2, AST_OP_INC); }
    | DEC_OP unary_expression { $$ = ast_create(AST_PREFIX, $2, AST_OP_DEC); }
    | unary_operator unary_expression { $$ = ast_create(AST_PREFIX, $2, $1); }
    ;

unary_operator
    : '&' { $$ = AST_OP_REF; }
    | '*' { $$ = AST_OP_DEREF; }
    | '+' { $$ = AST_OP_PLUS; }
    | '-' { $$ = AST_OP_MINUS; }
    | '!' { $$ = AST_OP_NOT; }
    ;

multiplicative_expression
    : unary_expression
    | multiplicative_expression '*' unary_expression { $$ = ast_create(AST_BINARY, AST_OP_MULT, $1, $3); }
    | multiplicative_expression '/' unary_expression { $$ = ast_create(AST_BINARY, AST_OP_DIV, $1, $3); }
    | multiplicative_expression '%' unary_expression { $$ = ast_create(AST_BINARY, AST_OP_MOD, $1, $3); }
    ;

additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression { $$ = ast_create(AST_BINARY, AST_OP_PLUS, $1, $3); }
    | additive_expression '-' multiplicative_expression { $$ = ast_create(AST_BINARY, AST_OP_MINUS, $1, $3); }
    ;

relational_expression
    : additive_expression
    | relational_expression '<' additive_expression { $$ = ast_create(AST_BINARY, AST_OP_LT, $1, $3); }
    | relational_expression '>' additive_expression { $$ = ast_create(AST_BINARY, AST_OP_GT, $1, $3); }
    | relational_expression LE_OP additive_expression { $$ = ast_create(AST_BINARY, AST_OP_LE, $1, $3); }
    | relational_expression GE_OP additive_expression { $$ = ast_create(AST_BINARY, AST_OP_GE, $1, $3); }
    ;

equality_expression
    : relational_expression
    | equality_expression EQ_OP relational_expression { $$ = ast_create(AST_BINARY, AST_OP_EQ, $1, $3); }
    | equality_expression NE_OP relational_expression { $$ = ast_create(AST_BINARY, AST_OP_NE, $1, $3); }
    ;

and_expression
    : equality_expression
    | and_expression '&' equality_expression { $$ = ast_create(AST_BINARY, AST_OP_BAND, $1, $3); }
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression { $$ = ast_create(AST_BINARY, AST_OP_BXOR, $1, $3); }
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression { $$ = ast_create(AST_BINARY, AST_OP_BOR, $1, $3); }
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression AND_OP inclusive_or_expression { $$ = ast_create(AST_BINARY, AST_OP_LAND, $1, $3); }
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression OR_OP logical_and_expression { $$ = ast_create(AST_BINARY, AST_OP_LOR, $1, $3); }
    ;

// Modified this and moved the function calls here from postfix_expression so that one can NOT
// call a function in the middle of an expression; this is harder for students to implement
// because of the live registers in the expression.
assignment_expression
    : logical_or_expression
    | postfix_expression '(' ')' { $$ = ast_create(AST_CALL, $1, ast_create(AST_LIST, NULL, NULL)); }
    | postfix_expression '(' identifier_list ')' { $$ = ast_create(AST_CALL, $1, $3); }
    | unary_expression '=' assignment_expression { $$ = ast_create(AST_BINARY, AST_OP_ASSIGN, $1, $3); }
    ;

constant : CONSTANT { $$ = ast_create(AST_CONSTANT, $1); }
identifier : IDENTIFIER { $$ = ast_create(AST_IDENTIFIER, $1); }
string_literal : STRING_LITERAL { $$ = ast_create(AST_STRING_LITERAL, $1); }

%%
