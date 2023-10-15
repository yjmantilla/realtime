#include <iostream>
#include <thread>

using namespace std;

void ThreadFunction() {
    cout << "Hello from a thread!" << endl;
}

int main() {
    thread th(ThreadFunction);
    cout << "Hello from Main! Today friday" << endl;
    th.join();

    return 0;
}

/*	Referencias:
    https://www.bogotobogo.com/cplusplus/C11/1_C11_creating_thread.php
    https://www.tutorialcup.com/cplusplus/multithreading.htm
*/
