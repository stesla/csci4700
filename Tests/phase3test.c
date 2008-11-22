/*****************************************************************************/
/* Here's the scoop on this test file.                                       */
/* 1. We never discussed comments. If you'd like to handle them that's great */
/*    but if not just remove them from the file.                             */
/* 2. Remove the top part of this file (see below) to make it a valid c--    */
/*    input file. You may want to keep the original for future use.          */
/*****************************************************************************/

/* Remove all of this to make it a valid c-- file */
#include <stdio.h>
#define write(X)  printf("%d\n",X)
#define writes(S) puts(S)
#define read(N)   scanf("%d",&N);
int input, nv, v, c, diff, i, isprime, fib1, fib2, fibsum;
/* OK, if you commented all of that out, the rest should be OK */

/* Calculate the integer square root for an integer number.    */
/* Stop when the user enters N < 1 as the input. Imagine that! */
/* It's not quite perfect, by the way, as sqrt(8) is 3 for     */
/* instance. But for our purposes it's good enough. Once that  */
/* is done, let's see if the number you gave us is prime.      */
main()
{

    read( input );
    while ( input > 0 )
    {
        if ( input < 3 )
            write(1);
        else
        {
            v = input / 2;
            for( c = 0; c < 25; c++ )
            {
                nv = ( v + input/v ) / 2;
                if ( v > nv )
                    diff = v - nv;
                else
                    diff = nv - v;
                if ( diff < 1 )
                    c = 25; /* "break" statement */
                else
                    v = nv;
            }
            writes( "The square root of that is about" );
            write( nv );

            /* Now let's see if the number you gave us is prime. */
            /* We only do this for values >= 3, see above...     */
            isprime = 1;
            for( i = 2; i < input && isprime; ++i )
            {
                if ( ( input % i ) == 0 )
                    isprime = 0;
            }
            if ( isprime )
                writes( "The input was also prime" );
            else
                writes( "The input was not prime." );


            /* I don't suppose it's a Fibonacci number is it? */
            fib1 = 1; fib2 = 1; fibsum = 2;
            while ( fibsum < input )
            {
                fibsum = fib1 + fib2;
                if ( fibsum == input )
                    fibsum = input; /* break */
                else
                {
                    /* Watch this... */
                    fib1 = fib1 ^ fib2;
                    fib2 = fib1 ^ fib2;
                    fib1 = fib1 ^ fib2;
                    fib2 = fib2 ^ fibsum;
                    fibsum = fib2 ^ fibsum;
                    fib2 = fib2 ^ fibsum;
                }
            }
            if ( fibsum == input )
                writes( "That's also a Fibonacci number!" );
            else
                writes( "That's not a Fibonacci number." );
        }
        read( input );
    }
}

