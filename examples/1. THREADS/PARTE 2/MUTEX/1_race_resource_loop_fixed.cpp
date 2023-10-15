#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mymutex;

void share_cout(string msg, int id)
{
	std::lock_guard <std::mutex > lock(mymutex);
        cout << msg << ":" << id << "\n";
}


void thread_function()
{
    for (int i = -100; i < 0; i++)
	share_cout("thread function: ", i);
//        cout << "thread function: " << i << "\n";
}

int main()
{
	thread t(&thread_function);
	for (int i = 0; i < 100; i++)
		share_cout("main thread: ", i);
//	    cout << "main thread: " << i << "\n";
	t.join();
	return 0;
}
