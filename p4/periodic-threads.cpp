#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <vector>

#include "periodic_settings.h"
#include "task_definition.h"

constexpr int SECONDS_TO_RUN = 10;

static std::mutex mtx;


void* genericThread(void* p_d) {
    PeriodicThread* threadData = static_cast<PeriodicThread*>(p_d);

    mtx.lock();
    
    std::cout << "\nThread with period : " << threadData->period / 1000 << " ms.\n";
    startPeriodicTimer(threadData);
    
    mtx.unlock();
    
    while (true) {
        waitNextActivation(threadData);
        threadData->taskFunction(&(threadData->count), threadData->id);  // Pass the ID as well
    }
}

int main() {
    std::vector<PeriodicThread*> threadConfigs;
    std::vector<pthread_t> threadIDs;

    // Producer Thread
    threadConfigs.push_back(new PeriodicThread{5000000, 100000, 0, {}, produce,1});

    // Consumer Threads
    threadConfigs.push_back(new PeriodicThread{200000, 150000, 0, {}, consume,2});
    threadConfigs.push_back(new PeriodicThread{250000, 160000, 0, {}, consume,3});  // New consumer thread
    threadConfigs.push_back(new PeriodicThread{300000, 170000, 0, {}, consume,4});  // New consumer thread

    threadIDs.resize(threadConfigs.size());

    for (size_t i = 0; i < threadConfigs.size(); ++i) {
        if (pthread_create(&threadIDs[i], nullptr, genericThread, static_cast<void*>(threadConfigs[i])) != 0) {
            std::cerr << "Cannot create pthread " << (i+1) << "\n";
        }
    }

    sleep(SECONDS_TO_RUN);

    mtx.lock();
    std::cout << "\nThis code just ran for " << SECONDS_TO_RUN << " seconds. This is a constexpr parameter.\n\n";

    for (size_t i = 0; i < threadConfigs.size(); ++i) {
        std::cout << "Considering the offset of " << threadConfigs[i]->offset << " us, and the period of " << threadConfigs[i]->period << " us, there were " << threadConfigs[i]->count << " iterations of Thread " << (i+1) << ".\n";
    }

    mtx.unlock();

    for (auto config : threadConfigs) {
        delete config;
    }

    return 0;
}
