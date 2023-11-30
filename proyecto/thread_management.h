#ifndef THREAD_MANAGEMENT_H
#define THREAD_MANAGEMENT_H

#include <pthread.h>
#include <string>
#include <atomic> // For std::atomic

extern std::atomic<bool> run_flag;

typedef void (*task_code)(void);

typedef struct {
    int priority;              // Priority
    const char* name;          // Task name
    int period_ms;             // Task period in milliseconds
    int wcet_ms;               // Worst-case execution time in milliseconds
    pthread_mutex_t* mutex;    // Optional mutex for shared resource
    task_code task_function;   // Function pointer to the task-specific code
    int log;
    pthread_mutex_t* cpumutex;    // Optional mutex for shared resource
    int offset; // from the start
    struct timespec nextActivation;
    float overheadOffset; // amount of ms to correct (substract) the next activation to account for logging overhead
    float deadlineTolerance; //amount of milliseconds to consider a deadline miss
} task_t;

double log_message(const char* task_name, const std::string& message, int LOG, int offset_ms);

void* generalized_thread(void *arg);

void startPeriodicTimer(task_t* perThread);

void waitNextActivation(task_t* t);


#endif // THREAD_MANAGEMENT_H
