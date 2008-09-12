%{

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
#include        "parser.h"

using namespace std;

extern int yydebug; // Use if you want the Bison generated debugging

int usage( void );
int yyerror( const char *msg );
int my_input( char *buf, int max_size );
void detab( char *line );
int yylex( void );
extern "C" int yywrap( void );

char        listing_line[ 132 ];    // for listings.
int     err_count;      // # of errors.
short       listing;        // true gives listing
short           echo;                   // true echos input
short           lex_debug;              // true debugs scanner
short           line = 0;           // For listings
ifstream        infile;

%}

/* ===========================================================================

Here's the grammar part. The %expect says to expect one conflict
(shift/reduce). In this case it is the dangling else problem and we
know about that one.

=========================================================================== */

%token ARRAY GLOBAL READ WRITE

%token IDENTIFIER CONSTANT STRING_LITERAL
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP
%token IF ELSE WHILE FOR RETURN

%expect 1

%%

translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;

external_declaration
    : IDENTIFIER '(' formal_list ')' block
        | decl
    ;

formal_list
        : formal_list ',' formal
        | formal
        |
        ;

formal
        : IDENTIFIER '[' ']'
        | IDENTIFIER
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
        : ARRAY a_list ';'
        | GLOBAL identifier_list ';'
        ;

a_list
        : IDENTIFIER '[' CONSTANT ']'
        | a_list ',' IDENTIFIER '[' CONSTANT ']'
        ;

statement
    : block
    | expression_stmt
    | selection_stmt
    | iteration_stmt
    | return_stmt
        | READ '(' IDENTIFIER ')' ';'
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
    : IDENTIFIER
    | CONSTANT
    | STRING_LITERAL
    | '(' expression ')'
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

identifier_list
    : IDENTIFIER
    | identifier_list ',' IDENTIFIER
    ;

%%

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
    cout << "usage: whatever <file> [-e] [-l] [-m size ] [-p] [-s] [-y]\n"
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

    static char   temp[ BUFSIZ ];
    register char *s, *d;
    int       col;

    col = 0; s = line; d = temp;
    while ( *s )
    if ( *s != '\t' )
        *d++ = *s++, col++;
    else
    {
        do  {
        *d++ = ' ';
        col++;
        } while ( col % 8 );
        s++;
    }
    *d = '\0';
    (void) strcpy( line, temp );

}
