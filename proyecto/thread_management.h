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
} task_t;

double log_message(const char* task_name, const std::string& message, int LOG);

void* generalized_thread(void *arg);

#endif // THREAD_MANAGEMENT_H
