/*************************************************/

// Prof. Luca Abeni. The Real-Time Operating Systems and Middleware course
// http://www.dit.unitn.it/~abeni/RTOS/periodic_tasks.h

/*************************************************/


/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2023 -
/*************************************************/


#include <ctime>

struct PeriodicThread {
    int64_t offset;
    int64_t period;
    int count;
    timespec next_activation;
    void (*taskFunction)(int*, int);  // 1: counter, 2: id
    int id;  // Added thread ID
};

void currentTime();

void startPeriodicTimer(PeriodicThread* perThread);

void waitNextActivation(PeriodicThread* t);
