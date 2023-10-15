#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char ** argv) {
	
	// Storing a int inside a auto variable
	auto var_1 = 5;
	 
	// Storing a character inside a auto variable
	auto var_2 = 'C';
	 
	cout << var_1 << endl;
	cout << var_2 << endl;

	auto fun_sum = [](int a , int b){
		return a+b;
	};
	cout << "Sum = " << fun_sum(4,5) << endl;
}
