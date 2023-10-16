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
#include <queue>
#include <optional>
#include <mutex>
struct PeriodicThread {
    int64_t offset;
    int64_t period;
    int count;
    timespec next_activation;
    std::optional<int> (*taskFunction)(int*, int, std::mutex&);
    int id;
    std::queue<std::optional<int>> history;  // Queue to store the history of produced/consumed values
};

void currentTime();

void startPeriodicTimer(PeriodicThread* perThread);

void waitNextActivation(PeriodicThread* t);
