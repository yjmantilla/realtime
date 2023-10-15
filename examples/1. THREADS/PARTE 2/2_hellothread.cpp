#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char ** argv) {

	thread hello([] { cout << "Hello,"; });
	thread world([] { cout << "world! \n"; });
	hello.join();
	world.join();

}
