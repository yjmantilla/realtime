#include "task_definition.h"
#include "thread_management.h"
#include <pthread.h>
#include <stdio.h>
#include <cstdlib>
#include <chrono>

// Define a function to calculate priority based on task properties
int calculate_priority(const task_t& task) {
    // You can use task properties to calculate the priority as needed
    int priority = 0;
    
    // Example: Higher priority for tasks with shorter periods
    priority = 100 / task.period_ms;

    return priority;
}

int main(int argc, char* argv[]) {
    int runtime_ms = 100; // Default runtime value
    int log=0;
    if (argc >= 3) {
        runtime_ms = std::atof(argv[1]);
        log = std::atoi(argv[2]);
    }else{
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    pthread_mutex_t infobus_mutex;
    pthread_mutex_init(&infobus_mutex, NULL);

    // Define task properties
    task_t tasks[] = {
        {7,"SCHED_BUS", 25, 5, NULL, sched_bus_code, log},        // SCHED_BUS
        {6,"DATA",25, 5, &infobus_mutex, data_code, log},   // DATA
        //{5,"CONTROL", 50, 5, &infobus_mutex, control_code, log},// CONTROL
        {4,"RADIO", 50, 5, NULL, radio_code, log},            // RADIO
        //{3,"VIDEO", 50, 5, NULL, video_code, log},            // VIDEO
        {2,"MEASURE", 100, 10, &infobus_mutex, measure_code, log}, // MEASURE
        {1,"FORECAST", 100, 15, &infobus_mutex, forecast_code, log} // FORECAST
    };

    const int num_tasks = sizeof(tasks) / sizeof(task_t);
    pthread_t threads[num_tasks];
    pthread_attr_t attr; // Attribute for thread properties
    
    // Create and start threads for each task
    for (int i = 0; i < num_tasks; i++) {

        pthread_attr_init(&attr);
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&attr, SCHED_FIFO); // SCHED_FIFO Or SCHED_RR

        struct sched_param param;
        param.sched_priority = tasks[i].priority; // calculate_priority(tasks[i]); // Calculate priority based on task properties
        pthread_attr_setschedparam(&attr, &param);

        pthread_create(&threads[i], &attr, generalized_thread, (void *)&tasks[i]);
    
    }


    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >= runtime_ms) {
            run_flag = false; // Signal threads to stop
            break;
        }
    }
    // Wait for all threads to complete (join)
    for (int i = 0; i < num_tasks; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&infobus_mutex);

    return 0;
}
