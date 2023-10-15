/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2023 -
/*************************************************/

#include "task_definition.h"

std::mutex lock; // Mutex for synchronization

void thread_1(int* c)
{
    lock.lock();
    (*c)++;
    std::cout << "Thread 1 counting: " << *c << std::endl;
    lock.unlock();
}

void thread_2(int* c)
{
    lock.lock();
    (*c)++;
    std::cout << "Thread 2 counting: " << *c << std::endl;
    lock.unlock();
}
