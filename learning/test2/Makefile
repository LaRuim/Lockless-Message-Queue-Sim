dummyName: test2.o queue.o
	g++ -g -pthread test2.o queue.o -o test2
mainDummy: test2.cpp queue.h
	g++ -g -c test2.cpp
queue_listDummy: queue.cpp queue.h
	g++ -g queue.cpp