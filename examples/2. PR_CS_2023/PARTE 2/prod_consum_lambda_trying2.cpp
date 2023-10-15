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
        queueMutex.lock();
		cout << "Hola 2 " << endl;
		cout << "EMPTY: " << goods.empty() << " FinCons" << endl;
		while (!(goods.empty())){
            cout << "Hola!" << endl;
/* 			unique_lock<std::mutex> lck2(queueMutex);
			cout << "Running thread consumer" << endl;
			queueConditionVariable.wait(lck2, [&]{ return !goods.empty(); });*/
			goods.pop();
			c--;
			cout << "Consuming: Current goods quantity: " << c << endl;
         	}
        queueMutex.unlock();
	});	


	thread producer([&]() {
        queueMutex.lock();
		goods.push(45); c++;
        cout << "Producer: " << goods.empty() << " FinProd" << endl;
        queueMutex.unlock();
/* 		for (int i = 0; i < 500; ++i) {
			lock_guard<std::mutex> lck1(queueMutex);
			cout << "Running thread producer: " << i << endl;
			goods.push(i);
			c++;
			cout << "Producing: Current goods quantity: " << c << endl;
			queueConditionVariable.notify_one();
		}
		done = true; */
	});
	

	


	consumer.join();
    producer.join();
    

    cout << "Net: " << c << endl;

}
