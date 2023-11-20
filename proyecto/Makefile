CC = g++
CFLAGS = -Wall -g
LIBRT = -lrt
LIBPTHREAD = -lpthread

SETTINGS = periodic_settings
TASKS = task_definition
MAIN = periodic-threads


all: $(MAIN)

$(SETTINGS).o: $(SETTINGS).cpp $(SETTINGS).h
	$(CC) $(CFLAGS) -c $(SETTINGS).cpp $(LIBRT) $(LIBPTHREAD)

$(TASKS).o: $(TASKS).cpp $(TASKS).h
	$(CC) $(CFLAGS) -c $(TASKS).cpp $(LIBRT) $(LIBPTHREAD)

$(MAIN): $(MAIN).cpp $(SETTINGS).o $(TASKS).o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRT) $(LIBPTHREAD)



clean:
	rm -f *.o
	rm -f $(MAIN)


