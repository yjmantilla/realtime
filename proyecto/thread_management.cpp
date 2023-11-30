#include "task_definition.h"
#include "thread_management.h"
#include <pthread.h>
#include <time.h>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <unistd.h>
#include <sys/time.h>
#include <sstream>
#define NSEC_PER_SEC 1000000000ULL
#define TOL 0

std::atomic<bool> run_flag(true);

// Function to create formatted message
std::string format_message(float value,const std::string& message) {
    std::ostringstream stream;
    stream << message << value << " ms";
    return stream.str();
}

double log_message(const char* task_name, const std::string& message, int LOG) {
    auto start = std::chrono::system_clock::now();
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
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

static inline void timespecAddUs(timespec *t, uint64_t d)
{
    d *= 1000;
    d += t->tv_nsec;
    while (d >= NSEC_PER_SEC) {
        d -= NSEC_PER_SEC;
        t->tv_sec += 1;
    }
    t->tv_nsec = static_cast<long>(d);
}

void waitNextActivation(task_t *t)
{
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t->nextActivation, NULL);
    timespecAddUs(&t->nextActivation, (t->period_ms-TOL)*1000); //convert to micro
}

void startPeriodicTimer(task_t* perThread)
{
    clock_gettime(CLOCK_REALTIME, &perThread->nextActivation);
    timespecAddUs(&perThread->nextActivation, perThread->offset*1000);
}


std::chrono::duration<double, std::milli> timespecToDuration(const timespec& ts) {
    auto secs = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(std::chrono::seconds(ts.tv_sec));
    auto nanosecs = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(std::chrono::nanoseconds(ts.tv_nsec));
    return secs + nanosecs;
}

timespec timePointToTimespec(const std::chrono::system_clock::time_point& tp) {
    auto duration_since_epoch = tp.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration_since_epoch);
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration_since_epoch - seconds);

    timespec ts;
    ts.tv_sec = seconds.count();
    ts.tv_nsec = nanoseconds.count();
    return ts;
}

double timespecDiffToDouble(const struct timespec *ts1, const struct timespec *ts2) {
    long sec_diff = ts2->tv_sec - ts1->tv_sec;
    long nsec_diff = ts2->tv_nsec - ts1->tv_nsec;

    // Handle nanosecond overflow
    if (nsec_diff < 0) {
        sec_diff--;
        nsec_diff += 1000000000L; // 1e9
    }

    // Convert to double
    return (double)sec_diff + (double)nsec_diff / 1e9;
}

int compareTimespec(const struct timespec *ts1, const struct timespec *ts2) {
    if (ts1->tv_sec < ts2->tv_sec) {
        return -1;
    } else if (ts1->tv_sec > ts2->tv_sec) {
        return 1;
    } else if (ts1->tv_nsec < ts2->tv_nsec) {
        return -1;
    } else if (ts1->tv_nsec > ts2->tv_nsec) {
        return 1;
    }
    return 0;
}

void* generalized_thread(void *arg) {
    task_t *task = (task_t *) arg;
    //struct timespec next_execution_time;
    double debug_loss = 0;
    startPeriodicTimer(task);
    timespecAddUs(&task->nextActivation, (task->period_ms-TOL)*1000); //convert to micro
    while (run_flag) {
        debug_loss=0;

        // Log task start

        if (task->mutex) {
            // Log resource acquisition attempt
            //debug_loss+=log_message(task->name, "Attempting to acquire resource",task->log);

            pthread_mutex_lock(task->mutex);

            // Log resource acquisition
            //debug_loss+= log_message(task->name, "Resource acquired",task->log);
        }

        if (task->cpumutex){
            pthread_mutex_lock(task->cpumutex);
        }

        auto start = std::chrono::system_clock::now();
        debug_loss+=log_message(task->name, "Task started - CPU and Resources Acquired",task->log);

        timespec start_timespec = timePointToTimespec(start);
        if (compareTimespec(&start_timespec,&task->nextActivation)>0){
            debug_loss += log_message(task->name,"missed dealine",task->log);
        }
        double act_diff= timespecDiffToDouble(&start_timespec,&task->nextActivation);
        debug_loss+=log_message(task->name, format_message(act_diff,"Activation difference:"),task->log);

        task->task_function();

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        // Simulate task execution (WCET)
        double remainingTime_ms = task->wcet_ms - elapsed.count();
        debug_loss+=log_message(task->name, format_message(elapsed.count(),"Elapsed ms:"),task->log);
        if (remainingTime_ms > 0.0) { // dont account time for logging
            debug_loss+=log_message(task->name, format_message(remainingTime_ms,"Sleeping ms:"),task->log);
            usleep(static_cast<useconds_t>((remainingTime_ms) * 1000));
        }

        if (task->mutex) {
            pthread_mutex_unlock(task->mutex);

            // Log resource release
            debug_loss+=log_message(task->name, "Resource released",task->log);
        }


        // Log task end
        debug_loss+=log_message(task->name, "Task completed - CPU (and resources) Released",task->log);


        end = std::chrono::system_clock::now();
        //elapsed = end.count() - timespecToDuration(task->nextActivation);
        //remainingTime_ms = task->period_ms + debug_loss - elapsed.count();

        if (task->cpumutex){
        pthread_mutex_unlock(task->cpumutex);
        }
        waitNextActivation(task);
    }
    return NULL;
}


