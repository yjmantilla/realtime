/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2022 -
/*************************************************/

#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t lock;

void thread_1(int* c);
void thread_2(int* c);
void thread_3(int* c);

