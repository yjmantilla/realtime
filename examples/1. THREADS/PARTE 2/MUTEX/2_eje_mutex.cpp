#include <iostream>
#include <thread>
#include <mutex>


int theI = 0;
std::mutex theMutex;

void safe_inc(int n)
{
	std::lock_guard <std::mutex > lock(theMutex);
	theI += n;
	std::cout << "now: " << theI << std::endl;
}

int main()
{
	std::thread t1(safe_inc , 42);
	std::thread t2(safe_inc , 684);

	t1.join();
	t2.join();

	return 0;
}
