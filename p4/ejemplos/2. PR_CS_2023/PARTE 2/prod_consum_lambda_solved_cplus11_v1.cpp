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

	thread producer([&]() {
		for (int i = 0; i < 500; ++i) {
			lock_guard<std::mutex> lck1(queueMutex);
			cout << "Running thread producer: " << i << endl;
			goods.push(i);
			c++;
			cout << "Producing: Current goods quantity: " << c << endl;
			queueConditionVariable.notify_one();
		}
		done = true;
	});


	thread consumer([&]() {
		while (!done){
			unique_lock<std::mutex> lck2(queueMutex);
			cout << "Running thread consumer" << endl;
			if (!done) 
				queueConditionVariable.wait(lck2);
			while (!goods.empty()) {
				goods.pop();
				c--;
				cout << "Consuming: Current goods quantity: " << c << endl;
            		}
        	}
	});

    producer.join();
    consumer.join();

    cout << "Net: " << c << endl;

}
