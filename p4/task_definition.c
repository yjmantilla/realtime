/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2023 -
/*************************************************/


#include "task_definition.h"


static pthread_mutex_t buffer_mutex;
const int BUFFER_MAX_SIZE = 10; 
std::queue<int> buffer;

void thread_1(int* c) {
    int random_value = rand() % 100;  // Generate a random integer between 0 and 99

    pthread_mutex_lock(&buffer_mutex);
    
    if (buffer.size() < BUFFER_MAX_SIZE) {
        buffer.push(random_value);
        std::cout << "Produced: " << random_value << ". Buffer Size: " << buffer.size() << std::endl;
    } else {
        std::cout << "Buffer is full. Not producing." << std::endl;
    }

    pthread_mutex_unlock(&buffer_mutex);
}

// void thread_1(int* c)
// {
// 	pthread_mutex_lock(&lock);
// 	printf("Thread 1 counting: %d\n", (*c)++);
// 	pthread_mutex_unlock(&lock);
// }


void thread_2(int* c)
{
	pthread_mutex_lock(&buffer_mutex);
	printf("Thread 2 counting: %d\n", (*c)++);
	pthread_mutex_unlock(&buffer_mutex);
}