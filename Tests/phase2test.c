/*****************************************************************************/
/* Here's the scoop on this test file.                                       */
/* 1. We never discussed comments. If you'd like to handle them that's great */
/*    but if not just remove them from the file.                             */
/* 2. Remove the top part of this file (see below) to make it a valid c--    */
/*    input file. You may want to keep the original for future use.          */
/* 3. If the pre nad post operators (++) don't work QUITE right I can live   */
/*    with this. In other words, we'll use these sparingly.                  */
/*****************************************************************************/

/* Remove all of this to make it a valid c-- file */
#include <stdio.h>
#define write(X) printf("%d\n",X)
int a, b, c, flag;
/* OK, if you commented all of that out, the rest should be OK */

main()
{
    /* puts( "Next are plain operators..." ); */

    a = 12; a = a - 12; write( a );            /* 0 - decimal */
    a = 012; a = a - 9; write( a );            /* 1 - octal */
    a = 0x12; a = a - 2 * 9; write( a );       /* 0 - hex */
    a = -1; a = a + 2; write( a );             /* 1 - Either the scanner does -1 or the parser should do a unary - */
    a = - 1; a = 1 + a; write( a );            /* 0 - definitely unary */
    a = 0; a = ! a; write( a );                /* 1 - Cast to boolean (false), not it, get 1 */
    a = 34; a = ! a; write( a );               /* 0 - Cast to boolean (true) and not it to get zero */
    a = 2 * 3 + 40 / 5 - 13; write( a );       /* 1 - Multiplication and division before addition and subtraction */
    a = 10 % 3 + 1 - 3 + 1; write( a );        /* 0 - Does your "mod" work? Associativity of +/- left to right? */
    a = 10 / 3 + 19 / 5 - 5; write( a );       /* 1 - Misc. */
    a = (( a - 1 ) * 100 + 5) - 5; write( a ); /* 0 - Misc. */

    /* puts( "Next are relations..." ); */

    a = 10; b = 10; c = 20;

    flag = a <= b; write( flag );              /* 1 */
    flag = a < b; write( flag );               /* 0 */
    flag = a >= b; write( flag );              /* 1 */
    flag = a > b; write( flag );               /* 0 */
    flag = a == b; write( flag );              /* 1 */
    flag = a != b; write( flag );              /* 0 */
    flag = a == c / 2; write( flag );          /* 1 */
    flag = a == b + 1; write( flag );          /* 0 */

    /* puts( "Next are bit-wise..." ); */

    c = ( a | ! b) / 10; write( c );           /* 1 */
    c = ! a & ! b; write( c );                 /* 0 */
    c = ( ! b | a ) / 10; write( c );          /* 1 */
    c = ( a & b ) - 10; write( c );            /* 0 */
    c = ( a ^ b ) + 1; write( c );             /* 1 */
    c = ( a ^ 1 ) - 11; write( c );            /* 0 */

    /* puts( "Next are short-circuit logical && and ||..." ); */

    flag = a && b; write( flag );              /* 1 */
    flag = (a && b) - 1; write( flag );        /* 0 */
    flag = a && b - 1; write( flag );          /* 1 */
    flag = b - 10 && a; write( flag );         /* 0 */
    flag = 0 || a; write( flag );              /* 1 */
    flag = (a - 10) || (b - 10); write(flag);  /* 0 */
    flag = a || ( b - 10 ); write( flag );     /* 1 */
    flag = ! flag || ! flag; write( flag );    /* 0 */

    /* puts( "Next are the pre/post inc..." ); */

    a = 0; a++; write( a );                    /* 1 */
    a--; write( a );                           /* 0 */
    b = ++a; write( b );                       /* 1 */
    b = --a; write( b );                       /* 0 */
    b = a++ + 1; write( b );                   /* 1 - did you do the increment later? */
    b = a++ - 1; write( b );                   /* 0 - did you do the increment later? */
    b = --a; write( b );                       /* 1 - did you do the increment earlier? */
    b = --a; write( b );                       /* 1 - did you do the increment earlier? */

}

