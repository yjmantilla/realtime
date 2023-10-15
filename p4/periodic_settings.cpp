#include <sys/time.h>
#include <time.h>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <string>
#include <cstring>

//#define DEBUG_MODE
#include "periodic_settings.h"

#define NSEC_PER_SEC 1000000000ULL

void currentTime()
{
    time_t sec;
    suseconds_t msec;
    std::string zeros;
    
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    sec = tv.tv_sec;
    msec = tv.tv_usec / 1000;
    
    if (msec < 10)
        zeros = "00";
    else if (msec < 100)
        zeros = "0";
    else 		
        zeros = "";

    std::cout << "\nEl tiempo actual en segundos es: " << sec << "." << zeros << msec << "\n";	
}

static inline void timespecAddUs(timespec *t, uint64_t d)
{
    #ifdef DEBUG_MODE
        std::cout << "\ntimespec t, component sec = " << t->tv_sec << "\n";
        std::cout << "timespec t, component nsec = " << t->tv_nsec << "\n";
    #endif

    d *= 1000;
    d += t->tv_nsec;
    while (d >= NSEC_PER_SEC) {
        d -= NSEC_PER_SEC;
        t->tv_sec += 1;
    }
    t->tv_nsec = static_cast<long>(d);
}

void waitNextActivation(PeriodicThread *t)  // <-- Updated here
{
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t->next_activation, nullptr);
    timespecAddUs(&t->next_activation, t->period);
}

void startPeriodicTimer(PeriodicThread* perThread)  // <-- Updated here
{
    #ifdef DEBUG_MODE
        std::cout << "\nBefore arming the timer: \n";
        currentTime();
    #endif

    std::cout << "Este hilo tiene un periodo esperado de : " << perThread->period / 1000 << " ms. \n";
    std::cout << "El offset de este hilo es de " << perThread->offset << " us.\n";

    clock_gettime(CLOCK_REALTIME, &perThread->next_activation);
    timespecAddUs(&perThread->next_activation, perThread->offset);
}
