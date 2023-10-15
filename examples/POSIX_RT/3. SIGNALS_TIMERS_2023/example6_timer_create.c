/*************************************************/
//  - Code modified by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2023 -
/*************************************************/

#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define DEBUG_MODE
#define TIME2SLEEP 1000000	// Time to sleep in us.

void current_time()
{
	time_t sec;
	suseconds_t	msec;
	
	struct timeval tv;
	char zeros[3];

	gettimeofday(&tv, 0);

	sec = tv.tv_sec;
	msec = tv.tv_usec/1000;
	
	if (msec < 10)
		strcpy(zeros, "00");
	else if (msec < 100)
		strcpy(zeros, "0");
	else 		
		strcpy(zeros, "");
	
	printf("%ld.%s%ld\n", sec, zeros, msec);
}

// callback function when the timer expires.. this will be called
void timer_handle(int sig, siginfo_t *si, void *uc)
{
	// Aqui va el codigo asociado al llamado de la señal SIGRTMIN.

	struct timeval tv;	
	// https://man7.org/linux/man-pages/man2/gettimeofday.2.html
	
	struct tm *info; 
	// https://www.cplusplus.com/reference/ctime/tm/
	
	time_t t;	
	// https://cplusplus.com/reference/ctime/time_t/

	gettimeofday(&tv, NULL);	
	// https://man7.org/linux/man-pages/man2/gettimeofday.2.html
	// Se recupera el tiempo actual definido segun la Epoch de Unix.

	printf("\n********** A Real-Time Signal have just happened due to a timer! **********\n");
	
#ifdef DEBUG_MODE
	printf("********** The signal came at: ");	
	current_time();
	printf("\n");
#endif

	printf("\ntimer interrupt %ld.%ld\n", tv.tv_sec, tv.tv_usec / 1000);

	t = tv.tv_sec;

	info = localtime(&t); 
	// https://www.cplusplus.com/reference/ctime/localtime/?kw=localtime 
	
	printf("In standard format, the interruption happened at: %s\n",asctime (info));
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "Write: %s 'timeout value in microsec < 1000000' '1/0' to repeat or not.\n", argv[0]);
		return -1;
	}

	int ret;
	int timerval;
	int repeat = 0;

	struct sigevent sev;
	struct itimerspec its;
	struct sigaction sa;
	timer_t timerid;
	
	memset(&sev, 0, sizeof(sev));
	memset(&its, 0, sizeof(its));
	memset(&sa, 0, sizeof(sa));
	//	https://man7.org/linux/man-pages/man3/memset.3.html		

	timerval = atoi(argv[1]);
	repeat = atoi(argv[2]);	

	if (timerval > 999999) 
	{
		perror("Valor no permitido. Debe ser menor a 1000000 usec");
		return -1;
	}

	// set signal mask for the timer
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = timer_handle;	
	
	ret = sigaction(SIGRTMIN + 1, &sa, NULL);
	if (ret != 0) {
		perror("sigaction");
		fprintf(stderr, "failed to setup SIGACTION for SIGRTMIN + 1.\n");
		return -1;
	}

	// set signal mask for sigevent
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN + 1;
	sev.sigev_value.sival_ptr = &timerid;

	/**** Timer creation with timer_create: ***/
	ret = timer_create(CLOCK_REALTIME, &sev, &timerid);
	if (ret < 0) {
		printf("There were an error with timer_create.");
		exit(-1);
	}

	//***	Definiciòn e inicializaciòn del temporizador.

	if (timerval >= 0) {
		its.it_value.tv_sec = 0; // Offset
		its.it_value.tv_nsec = timerval * 1000; // Offset
		if (repeat) {
			its.it_interval.tv_sec = 0;
			its.it_interval.tv_nsec = timerval * 1000;
		}
    }
	
	/**** Arming the timer with timer_settime: ***/
	ret = timer_settime(timerid, 0, &its, NULL);
	if (ret < 0) {
		printf("There were an error with timer_settime.");
		exit(-1);
	}
	while (1);
}
