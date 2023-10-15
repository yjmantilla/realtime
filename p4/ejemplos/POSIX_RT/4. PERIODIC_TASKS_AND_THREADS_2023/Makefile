# Makefile to compile the examples about Timers and Periodic Threads with POSIX Real-Time Extension

# Written by Gustavo Patino
# UdeA - May/2022

CC = gcc
CFLAGS = -Wall -g
LIBRT = -lrt
LIBPTHREAD = -lpthread

EXAMPLE7 = example7_timer_create

EXAMPLE8 = example8_periodic-process

EXAMPLE9 = example9_periodic-task

EXAMPLE10 = example10_periodic-threads

EXAMPLE11 = example11_periodic-task


PROGS = $(EXAMPLE7) $(EXAMPLE8) $(EXAMPLE9) $(EXAMPLE10) $(EXAMPLE11) 


all: $(PROGS)

$(EXAMPLE7): $(EXAMPLE7).c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRT)

$(EXAMPLE8): $(EXAMPLE8).c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRT)

$(EXAMPLE9): $(EXAMPLE9).c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRT)

$(EXAMPLE10): $(EXAMPLE10).c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRT) $(LIBPTHREAD)

$(EXAMPLE11): $(EXAMPLE11).c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRT)
	

clean:
	rm -f *.o *.txt
	rm -f $(PROGS)