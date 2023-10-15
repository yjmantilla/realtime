#include <iostream>
#include <thread>

using namespace std;

void thread_function()
{
    cout<<"Inside Thread :: ID  = "<<this_thread::get_id()<<endl;    
}
int main()  
{
    thread threadObj1(thread_function);
    thread threadObj2(thread_function);
 
    if(threadObj1.get_id() != threadObj2.get_id())
        cout<<"\nBoth Threads have different IDs\n"<<endl;
 
    cout<<"From Main Thread :: ID of Thread 1 = "<<threadObj1.get_id()<<endl;    
    cout<<"From Main Thread :: ID of Thread 2 = "<<threadObj2.get_id()<<endl;    
 
    threadObj1.join();    
    threadObj2.join();    

    return 0;
}

/*	Referencias:
    https://www.bogotobogo.com/cplusplus/C11/1_C11_creating_thread.php
    https://www.tutorialcup.com/cplusplus/multithreading.htm
*/