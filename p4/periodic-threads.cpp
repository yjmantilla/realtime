/*
 * Implement 2 periodic threads, with periods 100ms and 150ms.
 * The job bodies are functions named task1() and task2()
 * (defined in task_definition.c)
 *
 * WARNING!!! This program is incomplete: it does not set the threads
 * priorities! (threads are scheduled using the SCHED_OTHER policy)
 */

/*************************************************/
// Prof. Luca Abeni. The Real-Time Operating Systems and Middleware course
// http://www.dit.unitn.it/~abeni/RTOS/periodic_threads.c
/*************************************************/

/*************************************************/
//  - Code modified by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2023 -
/*************************************************/


#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "periodic_settings.h"
#include "task_definition.h"

#define SECONDS_TO_RUN 10

static pthread_mutex_t lock;

void *thread1(void* p_d)
{
	pthread_mutex_lock(&lock);
	struct periodic_thread* temp = (struct periodic_thread*) p_d;

	temp->offset = 5000000; // En unidades de us.
	temp->period = 100000; // En unidades de us.
	temp->count = 0;

	printf("\nThread 1 with period : %d ms. \n", temp->period/1000);
	start_periodic_timer(temp);

	pthread_mutex_unlock(&lock);

	while (1) {
		wait_next_activation(temp);
		thread_1(&(temp->count));
  	}
}

void *thread2(void* p_d)
{
	pthread_mutex_lock(&lock);
	struct periodic_thread* temp = (struct periodic_thread*) p_d;

	temp->offset = 200000;
	temp->period = 150000;
	temp->count = 0;

	printf("\nThread 2 with period : %d ms. \n", temp->period/1000);
	start_periodic_timer(temp);

	pthread_mutex_unlock(&lock);

	while (1) {
		wait_next_activation(temp);
		thread_2(&(temp->count));
  	}
}

int main(int argc, char *argv[])
{
	int err;
	pthread_t second_id, third_id;

	struct periodic_thread *pt1;
	struct periodic_thread *pt2;
	
	pt1 = new periodic_thread;
	pt2 = new periodic_thread;
	
	/* creation and activation of the new thread */

	err = pthread_create(&second_id, NULL, thread1, (void*) pt1);
	if (err != 0) {
	fprintf(stderr, "Cannot create pthread 1");
	}

	err = pthread_create(&third_id, NULL, thread2, (void*) pt2);
	if (err != 0) {
	fprintf(stderr, "Cannot create thread 2");
	}

	sleep(SECONDS_TO_RUN);

	pthread_mutex_lock(&lock);
	printf("\nThis code just ran for %d seconds. This is #define parameter.\n\n", SECONDS_TO_RUN);
	printf("Considering the offset of %d us, and the period of %d us, there were %d iterations of Thread 1.\n", pt1->offset, pt1->period, pt1->count);
	printf("Considering the offset of %d us, and the period of %d us, there were %d iterations of Thread 2.\n\n\n", pt2->offset, pt2->period, pt2->count);

	pthread_mutex_unlock(&lock);

	pthread_mutex_destroy(&lock);

  return 0;
}
