/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2023 -
/*************************************************/

#include "task_definition.h"


std::mt19937 rng(std::random_device{}());
// Create a uniform distribution between 0 and 99
std::uniform_int_distribution<int> dist(0, 99);

std::queue<int> buffer;
const int BUFFER_MAX_SIZE = 10;  // Set a limit for the buffer

std::mutex lock; // Mutex for synchronization

void produce(int* c, int id)
{
    lock.lock();

    int random_value = dist(rng);
    if (buffer.size() < BUFFER_MAX_SIZE) {
        buffer.push(random_value);
        std::cout << "Thread " << id << " random value: " << random_value << std::endl;
    } else {
        std::cout << "Buffer is full, dropping value: " << random_value << std::endl;
    }
    (*c)++;
    lock.unlock();
}

void consume(int* c, int id)
{
    lock.lock();
    if (!buffer.empty()) {
        int value = buffer.front();
        buffer.pop();
        std::cout << "Thread " << id << " consumed value: " << value << std::endl;
    } else {
        std::cout << "Buffer is empty, nothing to consume." << std::endl;
    }
    (*c)++;
    std::cout << "Thread " << id << " counting: " << *c << std::endl;
    lock.unlock();
}
