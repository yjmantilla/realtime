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
    timespec r;
    int period;
    int offset;
    int count;
};

void currentTime();

void startPeriodicTimer(PeriodicThread* perThread);

void waitNextActivation(PeriodicThread* t);
