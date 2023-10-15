CPP = g++ 

CPPFLAGS = -std=c++11 -pthread

SOURCE1 = 1_example_lambda.cpp 
SOURCE2 = 2_hellothread.cpp
SOURCE3 = 2_Hellothreadfunction.cpp
SOURCE4 = 3_num_threads.cpp
SOURCE5 = 4_getting_id.cpp


all: ejem1_lambdas ejem2_hello ejem2_helloFunc ejem3_numThreads ejem4_threadId


ejem1_lambdas: $(SOURCE1)
	$(CPP) $(CPPFLAGS) $(SOURCE1) -o $@

ejem2_hello: $(SOURCE2)
	$(CPP) $(CPPFLAGS) $(SOURCE2) -o $@

ejem2_helloFunc: $(SOURCE3)
	$(CPP) $(CPPFLAGS) $(SOURCE3) -o $@

ejem3_numThreads: $(SOURCE4)
	$(CPP) $(CPPFLAGS) $(SOURCE4) -o $@

ejem4_threadId: $(SOURCE5)
	$(CPP) $(CPPFLAGS) $(SOURCE5) -o $@	


clean:
	$(RM) all: ejem1_lambdas ejem2_hello ejem2_helloFunc ejem3_numThreads ejem4_threadId *.txt
