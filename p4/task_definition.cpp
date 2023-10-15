#include "task_definition.h"


std::mt19937 rng(std::random_device{}());
// Create a uniform distribution between 0 and 99
std::uniform_int_distribution<int> dist(0, 99);

std::queue<int> buffer;
const int BUFFER_MAX_SIZE = 10;  // Set a limit for the buffer

std::mutex lock; // Mutex for synchronization


std::optional<int> produce(int* c, int id)
{
    lock.lock();

    std::optional<int> random_value = dist(rng);

    if (buffer.size() < BUFFER_MAX_SIZE) {
        buffer.push(random_value.value());
    } else {
        std::cout << "Buffer is full, dropping value: " << random_value.value() << std::endl;
        random_value = std::nullopt;
    }

    if (random_value.has_value()) {
        std::cout << "Thread " << id << " random value: " << random_value.value() << std::endl;
    }
    lock.unlock();

    (*c)++;

    return random_value;
}
std::optional<int> consume(int* c, int id)
{
    lock.lock();
    std::optional<int> value;
    if (!buffer.empty()) {
        value = buffer.front();
        buffer.pop();
    } else {
        value = std::nullopt;
    }
    if (value.has_value()) {
        std::cout << "Thread " << id << " consumed value: " << value.value() << std::endl;
    } else {
        std::cout << "Thread " << id << " did not consume a value." << std::endl;
    }
    std::cout << "Thread " << id << " counting: " << *c << std::endl;
    lock.unlock();
    (*c)++;

    return value;
}
