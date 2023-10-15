
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <queue>

using namespace std;


int main() {

	int c = 0;
	bool done = false;
	queue<int> goods;
    
	mutex queueMutex;
	condition_variable queueConditionVariable;

	thread consumer([&]() {
        while (!(done && goods.empty())){
        	unique_lock<std::mutex> lck2(queueMutex);
			cout << "Running thread consumer" << endl;
			queueConditionVariable.wait(lck2, [&]{ return !goods.empty(); });
			goods.pop();
			c--;
			cout << "Consuming: Current goods quantity: " << c << endl;
			lck2.unlock();
			this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });	

	thread producer([&]() {
     	for (int i = 0; i < 500; ++i) {
			unique_lock<std::mutex> lck1(queueMutex);
			cout << "Running thread producer: " << i << endl;
			goods.push(i);
			c++;
			cout << "Producing: Current goods quantity: " << c << endl;
			queueConditionVariable.notify_one();
			lck1.unlock();
			this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		done = true;
	});
	
	consumer.join();
	producer.join();
    
	cout << "\nThe market has closed!" << endl;
    cout << "Final number of goods: " << c << endl;
}
