#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <valgrind/callgrind.h>


#define SIZE 10485760
#define NBR 10000



 int main() {
    CALLGRIND_START_INSTRUMENTATION;    
// array of data
    int* a = new int[SIZE];
    for(int i = 0; i < SIZE; i++)
        a[i] = i;

    // array of random index
    int* index = new int[NBR];
    for(int k = 0; k < NBR; ++k){
        index[k] = k;
        printf("index %d\n", k);
    }
    int d = 0;
    for(int k = 0; k < 1000; ++k) {
        int c = 0;

        // random fashion access
        for(int n = 0; n < NBR; ++n)
            c += a[index[n]];
            printf("c %d\n", c);

        d += c;
    }

    printf("%d\n", d);
    CALLGRIND_STOP_INSTRUMENTATION;
    return 0;
}
