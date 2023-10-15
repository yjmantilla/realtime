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

constexpr int SECONDS_TO_RUN = 10;

constexpr int T1 = 100000;
constexpr int T2 = 200000;
constexpr int T3 = 250000;
constexpr int T4 = 150000;

constexpr int OFF1 = 100000;
constexpr int OFF2 = 150000;
constexpr int OFF3 = 160000;
constexpr int OFF4 = 170000;

static std::mutex mtx;


void* genericThread(void* p_d) {
    PeriodicThread* threadData = static_cast<PeriodicThread*>(p_d);

    mtx.lock();
    
    std::cout << "\nThread with period : " << threadData->period / 1000 << " ms.\n";
    startPeriodicTimer(threadData);
    
    mtx.unlock();
    
    while (true) {
        waitNextActivation(threadData);
        std::optional<int> num = threadData->taskFunction(&(threadData->count), threadData->id);  
        if (num) {
            threadData->history.push(*num);
        }
    }
    return nullptr;
}

int main() {
    std::vector<PeriodicThread*> threadConfigs;
    std::vector<pthread_t> threadIDs;

    // Producer Thread
    threadConfigs.push_back(new PeriodicThread{T1, OFF1, 0, {}, produce,1});

    // Consumer Threads
    threadConfigs.push_back(new PeriodicThread{T2, OFF2, 0, {}, consume,2});
    threadConfigs.push_back(new PeriodicThread{T3, OFF3, 0, {}, consume,3});
    threadConfigs.push_back(new PeriodicThread{T4, OFF4, 0, {}, consume,4});

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
		std::cout << "\n";		
		std::cout << "History of Thread " << (i+1) << ": ";
		while (!threadConfigs[i]->history.empty()) {
			auto val = threadConfigs[i]->history.front();
			if (val.has_value()) {
				std::cout << val.value() << ", ";
			} else {
				std::cout << "NULL, "; // or "n/a" or any placeholder for a missing value
			}
			threadConfigs[i]->history.pop();  // remove the value from the queue once printed
		}
		std::cout << "\n";
		std::cout << "\n";
	}
    mtx.unlock();

    for (auto config : threadConfigs) {
        delete config;
    }

    return 0;
}
