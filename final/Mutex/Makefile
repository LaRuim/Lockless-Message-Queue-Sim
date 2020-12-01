dummyName: driver.o queue.o
	g++ -g -pthread driver.o queue.o -o locked_mq
mainDummy: driver.cpp queue.h
	g++ -g -c driver.cpp
queueDummy: queue.cpp queue.h
	g++ -g queue.cpp