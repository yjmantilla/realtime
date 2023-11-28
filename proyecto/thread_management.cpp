#include "task_definition.h"
#include "thread_management.h"
#include <pthread.h>
#include <time.h>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <unistd.h>

std::atomic<bool> run_flag(true);

// Function to create formatted message
std::string format_message(float value) {
    std::ostringstream stream;
    stream << "missed deadline by " << value << " ms";
    return stream.str();
}

void log_message(const char* task_name, const std::string& message, int LOG) {
    if (LOG) {
        std::string filename = std::string(task_name) + "_log.csv";

        // Check if file exists to write headers
        std::ifstream file_test(filename);
        bool file_exists = file_test.good();
        file_test.close();

        // Getting the current time for timestamp
        auto now = std::chrono::high_resolution_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        auto now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;

        std::tm *now_tm = std::localtime(&now_time_t);

        std::ofstream log_file(filename, std::ios::app);

        // Write headers if file does not exist
        if (!file_exists) {
            log_file << "Task,Timestamp,Event" << std::endl;
        }

        // Write the timestamp and message to the file
        log_file << task_name << ",";
        log_file << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
        log_file << '.' << std::setfill('0') << std::setw(9) << now_ns.count();
        log_file << "," << message << std::endl;
    }
}


void* generalized_thread(void *arg) {
    task_t *task = (task_t *) arg;
    struct timespec next_execution_time;

    while (run_flag) {
        auto start = std::chrono::system_clock::now();
        // Log task start
        log_message(task->name, "Task started",task->log);

        if (task->mutex) {
            // Log resource acquisition attempt
            log_message(task->name, "Attempting to acquire resource",task->log);

            pthread_mutex_lock(task->mutex);

            // Log resource acquisition
            log_message(task->name, "Resource acquired",task->log);
        }

        task->task_function();

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        // Simulate task execution (WCET)
        double remainingTime_ms = task->wcet_ms - elapsed.count();

        if (remainingTime_ms > 1.0) { // give time to logging
            usleep(static_cast<useconds_t>((remainingTime_ms-1) * 1000));
        }

        if (task->mutex) {
            pthread_mutex_unlock(task->mutex);

            // Log resource release
            log_message(task->name, "Resource released",task->log);
        }


        // Log task end
        log_message(task->name, "Task completed",task->log);

        end = std::chrono::system_clock::now();
        elapsed = end - start;
        remainingTime_ms = task->period_ms - elapsed.count();
        if (remainingTime_ms < 0.0){
            log_message(task->name,format_message(-1*remainingTime_ms),task->log);
        }

        // Calculate next execution time
        next_execution_time.tv_nsec += task->period_ms * 1000000;
        if (next_execution_time.tv_nsec >= 1000000000) {
            next_execution_time.tv_sec += 1;
            next_execution_time.tv_nsec -= 1000000000;
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_execution_time, NULL);
    }
    return NULL;
}