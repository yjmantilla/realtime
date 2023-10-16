#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <valgrind/callgrind.h>


#define SIZE 10485760
#define NBR 10000

int indexedAccessSum(int* a, int* index) {
    int sum = 0;
    for(int n = 0; n < NBR; ++n)
        sum += a[index[n]];
    return sum;
}

void fillArray(int* a, int size) {
    for(int i = 0; i < size; i++)
        a[i] = i;
}

int iterateSums(int* a, int* index) {
    int d = 0;
    for(int k = 0; k < 1000; ++k) {
        d += indexedAccessSum(a, index);
    }
    return d;
}

void makeIndexes(int* index, int numreads) {
    for(int k = 0; k < numreads; ++k)
        index[k] = rand() % SIZE; //k;
}

int main() {
    CALLGRIND_START_INSTRUMENTATION;    
    int* a = new int[SIZE];
    fillArray(a, SIZE);

    // array of random index
    int* index = new int[NBR];
    makeIndexes(index, NBR);

    int d = iterateSums(a, index);
    printf("%d\n", d);
    CALLGRIND_STOP_INSTRUMENTATION;
    return 0;
}
