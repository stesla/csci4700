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
/* OK, if you commented all of that out, the rest should be OK */

int leftStack[ 10 ]; // log2(100) is < 7 so we're good.
int lStackTop = 0;
int rightStack[ 10 ];
int rStackTop = 0;

void quickSort(int items[])
{
    int i, j, x, y, left, right;

    left = leftStack[ lStackTop ];
    right = rightStack[ rStackTop ];

    i = left;
    j = right;
    x = items[(left+right)/2];

    int flag = 1;
    while ( flag )
    {
        while((items[i] < x) && (i < right))
            i++;
        while((x < items[j]) && (j > left))
            j--;

        if(i <= j) {
            y = items[i];
            items[i] = items[j];
            items[j] = y;
            i++; j--;
        }
        if ( i > j )
            flag = 0;
    }

    // Some fooling here since in C-- there's no real concept of a
    // local parameter.

    if(i < right)
    {
        lStackTop++;
        leftStack[ lStackTop ] = i;
        rStackTop++;
        rightStack[ rStackTop ] = right;
        quickSort(items);
        lStackTop--;
        rStackTop--;
    }

    if(left < j)
    {
        lStackTop++;
        leftStack[ lStackTop ] = left;
        rStackTop++;
        rightStack[ rStackTop ] = j;
        quickSort(items);
        lStackTop--;
        rStackTop--;
    }
}


int main()
{
    int i, vec[ 100 ];
    for( i = 0; i < 100; i++ )
    {
        int val;
        read( val );
        vec[ i ] = val;
    }

    leftStack[ 0 ] = 0;
    rightStack[ 0 ] = 99;
    quickSort( vec );
    
    for( i = 0; i < 100; i++ )
    {
        int val;
        val = vec[ i ];
        write( val );
    }
    
}
