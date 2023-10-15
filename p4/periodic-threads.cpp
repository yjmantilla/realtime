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


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <thread>
#include <mutex>
#include <unistd.h>

#include "periodic_settings.h"
#include "task_definition.h"

constexpr int SECONDS_TO_RUN = 10;

static std::mutex mtx;

void* thread1(void* p_d)
{
    mtx.lock();
    PeriodicThread* temp = static_cast<PeriodicThread*>(p_d);

    temp->offset = 5000000; // En unidades de us.
    temp->period = 100000; // En unidades de us.
    temp->count = 0;

    std::cout << "\nThread 1 with period : " << temp->period / 1000 << " ms.\n";
    startPeriodicTimer(temp);

    mtx.unlock();

    while (true) {
        waitNextActivation(temp);
        thread_1(&(temp->count));
    }
}

void* thread2(void* p_d)
{
    mtx.lock();
    PeriodicThread* temp = static_cast<PeriodicThread*>(p_d);

    temp->offset = 200000;
    temp->period = 150000;
    temp->count = 0;

    std::cout << "\nThread 2 with period : " << temp->period / 1000 << " ms.\n";
    startPeriodicTimer(temp);

    mtx.unlock();

    while (true) {
        waitNextActivation(temp);
        thread_2(&(temp->count));
    }
}

int main()
{
    int err;
    pthread_t second_id, third_id;

    auto pt1 = new PeriodicThread;
    auto pt2 = new PeriodicThread;
    
    err = pthread_create(&second_id, nullptr, thread1, static_cast<void*>(pt1));
    if (err != 0) {
        std::cerr << "Cannot create pthread 1\n";
    }

    err = pthread_create(&third_id, nullptr, thread2, static_cast<void*>(pt2));
    if (err != 0) {
        std::cerr << "Cannot create thread 2\n";
    }

    sleep(SECONDS_TO_RUN);

    mtx.lock();
    std::cout << "\nThis code just ran for " << SECONDS_TO_RUN << " seconds. This is constexpr parameter.\n\n";
    std::cout << "Considering the offset of " << pt1->offset << " us, and the period of " << pt1->period << " us, there were " << pt1->count << " iterations of Thread 1.\n";
    std::cout << "Considering the offset of " << pt2->offset << " us, and the period of " << pt2->period << " us, there were " << pt2->count << " iterations of Thread 2.\n\n\n";

    mtx.unlock();

    //In C++ and with the Standard Library's abstractions, the resource management is typically more automatic than in C. The std::mutex object is automatically destroyed and its resources freed when it goes out of scope (i.e., when the program exits in this case, as it's a global variable). Therefore, you don't need an explicit call like pthread_mutex_destroy().

    delete pt1;
    delete pt2;

    return 0;
}
