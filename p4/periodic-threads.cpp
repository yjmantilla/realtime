#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <vector>
#include <optional>

#include "periodic_settings.h"
#include "task_definition.h"

// Constants to specify the runtime and the periodic timings for the threads.
constexpr int SECONDS_TO_RUN = 10;
constexpr int T1 = 100000; // Timing values in microseconds.
constexpr int T2 = 200000;
constexpr int T3 = 250000;
constexpr int T4 = 150000;

constexpr int OFF1 = 100000; // Offset values in microseconds.
constexpr int OFF2 = 150000;
constexpr int OFF3 = 160000;
constexpr int OFF4 = 170000;

static std::mutex mtx;  // Mutex used to synchronize threads during print operations.

// Generic function to represent the behavior of the threads.
void* genericThread(void* p_d) {
    // Cast the passed pointer to its original type.
    PeriodicThread* threadData = static_cast<PeriodicThread*>(p_d);

    mtx.lock();  // Lock to ensure serial access to std::cout.
    std::cout << "\nThread with period : " << threadData->period / 1000 << " ms.\n";
    startPeriodicTimer(threadData);  // Start the timer for this thread.
    mtx.unlock();
    
    // Infinite loop simulating the periodic tasks.
    while (true) {
        waitNextActivation(threadData);  // Wait for the next scheduled execution.
        std::optional<int> num = threadData->taskFunction(&(threadData->count), threadData->id);  
        if (num) {
            threadData->history.push(*num);  // If there's a value, push it to the history.
        }
    }
    return nullptr;
}

int main() {
    // Lists to store the configuration of each thread and their IDs.
    std::vector<PeriodicThread*> threadConfigs;
    std::vector<pthread_t> threadIDs;

    // Configurations for the producer and consumer threads.
    threadConfigs.push_back(new PeriodicThread{T1, OFF1, 0, {}, produce,1});
    threadConfigs.push_back(new PeriodicThread{T2, OFF2, 0, {}, consume,2});
    threadConfigs.push_back(new PeriodicThread{T3, OFF3, 0, {}, consume,3});
    threadConfigs.push_back(new PeriodicThread{T4, OFF4, 0, {}, consume,4});

    threadIDs.resize(threadConfigs.size());

    // Create pthreads for each configuration.
    for (size_t i = 0; i < threadConfigs.size(); ++i) {
        if (pthread_create(&threadIDs[i], nullptr, genericThread, static_cast<void*>(threadConfigs[i])) != 0) {
            std::cerr << "Cannot create pthread " << (i+1) << "\n";
        }
    }

    sleep(SECONDS_TO_RUN);  // Allow the threads to run for the specified duration.

    mtx.lock();  // Lock to ensure serial access to std::cout.
    std::cout << "\nThis code just ran for " << SECONDS_TO_RUN << " seconds. This is a constexpr parameter.\n\n";

    // Display the performance and history of each thread.
    for (size_t i = 0; i < threadConfigs.size(); ++i) {
        std::cout << "Considering the offset of " << threadConfigs[i]->offset << " us, and the period of " << threadConfigs[i]->period << " us, there were " << threadConfigs[i]->count << " iterations of Thread " << (i+1) << ".\n";
        std::cout << "\n";        
        std::cout << "History of Thread " << (i+1) << ": ";
        
        while (!threadConfigs[i]->history.empty()) {
            auto val = threadConfigs[i]->history.front();
            if (val.has_value()) {
                std::cout << val.value() << ", ";
            } else {
                std::cout << "NULL, ";
            }
            threadConfigs[i]->history.pop();
        }
        std::cout << "\n";
        std::cout << "\n";
    }
    mtx.unlock();

    // Clean up the dynamically allocated memory.
    for (auto config : threadConfigs) {
        delete config;
    }

    return 0;
}
