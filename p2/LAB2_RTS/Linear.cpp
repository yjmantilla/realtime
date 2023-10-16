#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <valgrind/callgrind.h>


#define SIZE 10485760
#define NBR 10000

int randomAccessSum(int* a, int* index) {
    int sum = 0;
    for(int n = 0; n < NBR; ++n)
        sum += a[index[n]];
    return sum;
}

void fillArray(int* a, int size) {
    for(int i = 0; i < size; i++)
        a[i] = i;
}

void linearIndexes(int* index, int size) {
    for(int k = 0; k < size; ++k)
        index[k] = k;
}

int main() {
    CALLGRIND_START_INSTRUMENTATION;    
// array of data
    int* a = new int[SIZE];
    fillArray(a, SIZE);

    // array of random index
    int* index = new int[NBR];
    linearIndexes(index, NBR);

    int d = 0;
    for(int k = 0; k < 1000; ++k) {
        d+=randomAccessSum(a, index);
    }

    printf("%d\n", d);
    CALLGRIND_STOP_INSTRUMENTATION;
    return 0;
}
