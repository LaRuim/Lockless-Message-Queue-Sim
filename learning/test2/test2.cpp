#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include "queue.h"

/*  Run the program as:
        ./test2
    for single-threaded queueing.

    Run the program as:
        ./test2 --parallel-threads 
    for multi-threaded queueing. 
    
    Dequeuing is only implemented with multiple threads.  */

int main(int argc, char* argv[]) {
    Queue* queue = new Queue();
    int messages[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int priorities[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

    std::vector<std::thread> threads;
    int item_per_thread = (sizeof(messages)/sizeof(int))/TOTAL_THREADS;
    if (argc > 1) {
        if (std::string(argv[1]) == "--parallel-threads" && argc == 2) {
            for (int thread_number = 0; thread_number < TOTAL_THREADS; thread_number++) {
                threads.push_back(std::thread([&](int thread_number, Queue* queue){
                    int data, priority;
                    
                    for (int i = thread_number*item_per_thread; i < item_per_thread*(thread_number+1); i++) {
                        data = messages[i];
                        priority = priorities[i];
                        queue->priority_enqueue(data, priority, thread_number);
                    }
                }, thread_number, queue));
            }
        }
        else {
            std::cerr << "Invalid argument. Only one optional argument, --parallel-threads is allowed, to run with multiple threads.\n";
            return 0;
        }
    }
    else {
        int data, priority;
        
        for (int i = 0; i < (TOTAL_THREADS)*item_per_thread; i++) {
            data = messages[i];
            priority = priorities[i];
            queue->priority_enqueue(data, priority, 1);
        }
    }

    /*  This delay is to make sure deletion does not commence before full population,
        purely for simplicity  */
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    std::cout << "\nInitial populated queue:\n";
    queue->print_all();

    /*  The dequeueing is taken care by 4 threads; Ideally, it should dequeue 4 unique elements sequentially
        and display them. However, that doesn't happen, due to a race condition being created.  */
    std::cout << "\nMessages:\n";
    for (int thread_number = 0; thread_number < TOTAL_THREADS; thread_number++) {
    threads.push_back(std::thread([&](int thread_number, Queue* queue){ 
            std::cout << std::to_string(queue->dequeue(thread_number))+"\n";
        }, thread_number, queue));
    }

    /* This line ensures that the main thread doesn't end up proceeding before the execution
       of all threads in the threads vector. */ 
    for (auto &thread: threads) {
        thread.join();
    }

    std::cout << "\nStill in Queue after messages are removed:\n";
    queue->print_all();
    return 1;
}