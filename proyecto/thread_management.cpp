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

std::atomic<bool> run_flag(true);

// Function to create formatted message
std::string format_message(float value,const std::string& message) {
    std::ostringstream stream;
    stream << message << value;
    return stream.str();
}

double log_message(const char* task_name, const std::string& message, int LOG, int offset_ms) {
    auto start = std::chrono::system_clock::now();
    if (LOG) {
        std::string filename = std::string(task_name) + "_log.csv";

        // Check if file exists to write headers
        std::ifstream file_test(filename);
        bool file_exists = file_test.good();
        file_test.close();

        // Getting the current time for timestamp
        auto now = std::chrono::high_resolution_clock::now();

        if (offset_ms>0){
            std::chrono::milliseconds duration(offset_ms);
            // Add milliseconds to the current time
            now = now + duration;
        }

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
    timespecAddUs(&t->nextActivation, (t->period_ms-t->overheadOffset)*1000); //convert to micro
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t->nextActivation, NULL);
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

double timespecDiffInMillis(const struct timespec *ts1, const struct timespec *ts2) {
    long sec_diff = ts2->tv_sec - ts1->tv_sec;
    long nsec_diff = ts2->tv_nsec - ts1->tv_nsec;

    // Handle nanosecond underflow
    if (nsec_diff < 0) {
        sec_diff--;
        nsec_diff += 1000000000L; // 1e9
    }

    // Convert to milliseconds
    double diff_in_millis = (double)sec_diff * 1000.0 + (double)nsec_diff / 1e6;
    return diff_in_millis;
}

void* generalized_thread(void *arg) {
    task_t *task = (task_t *) arg;
    //struct timespec next_execution_time;
    double debug_loss = 0;
    startPeriodicTimer(task);
    unsigned long long count=0;
    while (run_flag) {
        debug_loss=0;
        count++;

        // Log task start

        if (task->mutex) {
            // Log resource acquisition attempt
            //debug_loss+=log_message(task->name, "Attempting to acquire InfoBus resource",task->log,0);

            pthread_mutex_lock(task->mutex);

            // Log resource acquisition
            //debug_loss+= log_message(task->name, "InfoBus Resource acquired",task->log,0);
        }

        if (task->cpumutex){
            //debug_loss+=log_message(task->name, "Attempting to acquire CPUmutex",task->log,0);
            pthread_mutex_lock(task->cpumutex);
            //debug_loss+= log_message(task->name, "CPUmutex Resource acquired",task->log,0);
        }

        auto start = std::chrono::system_clock::now();
        debug_loss+=log_message(task->name, format_message(count,"Task started for the time: "),task->log,0);
        debug_loss+=log_message(task->name, "I expect to start again around this time",task->log,task->period_ms);
        timespec start_timespec = timePointToTimespec(start);
        // act_diff = timespecDiffToDouble(A,B)=B-A NOTE!!!!!!!!!!!
        double act_diff= timespecDiffToDouble(&task->nextActivation,&start_timespec);

        if (act_diff>=(double) task->deadlineTolerance/1000 && count > 1){
            debug_loss += log_message(task->name,"missed dealine",task->log,0);
        }
        if (count > 1){
            debug_loss+=log_message(task->name, format_message(act_diff*1000,"Activation difference [ms]:"),task->log,0);
        }

        task->task_function();

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        // Simulate task execution (WCET)
        double remainingTime_ms = task->wcet_ms - elapsed.count();
        debug_loss+=log_message(task->name, format_message(elapsed.count(),"Elapsed ms:"),task->log,0);
        if (remainingTime_ms > 0.0) { // dont account time for logging
            debug_loss+=log_message(task->name, format_message(remainingTime_ms,"Sleeping ms:"),task->log,0);
            usleep(static_cast<useconds_t>((remainingTime_ms) * 1000));
        }

        if (task->mutex) {
            pthread_mutex_unlock(task->mutex);

            // Log resource release
            debug_loss+=log_message(task->name, "InfoBus Resource released",task->log,0);
        }

        debug_loss+=log_message(task->name, format_message(debug_loss,"Approximate debug loss ms:"),task->log,0);
        // Log task end
        
        debug_loss+=log_message(task->name, "Task completed",task->log,0);

        //end = std::chrono::system_clock::now();
        //elapsed = end.count() - timespecToDuration(task->nextActivation);
        //remainingTime_ms = task->period_ms + debug_loss - elapsed.count();

        if (task->cpumutex){
        pthread_mutex_unlock(task->cpumutex);
        //debug_loss+=log_message(task->name, "CPUmutex Resource released",task->log,0);
        }
        waitNextActivation(task);
    }
    return NULL;
}


