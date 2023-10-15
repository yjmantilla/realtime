/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2023 -
/*************************************************/


#include "task_definition.h"

static pthread_mutex_t lock;

void thread_1(int* c)
{
	pthread_mutex_lock(&lock);
	printf("Thread 1 counting: %d\n", (*c)++);
	pthread_mutex_unlock(&lock);
}


void thread_2(int* c)
{
	pthread_mutex_lock(&lock);
	printf("Thread 2 counting: %d\n", (*c)++);
	pthread_mutex_unlock(&lock);
}