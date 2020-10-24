#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include <mutex>
#include <condition_variable>
#include "queue.h"

/*  Run the program as:
        ./test3
    for single-threaded queueing.

    Run the program as:
        ./test3 --parallel-threads 
    for multi-threaded queueing.  */

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
                    
                    for (int i = thread_number; i < item_per_thread*TOTAL_THREADS; i+=item_per_thread) {
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
        
        threads.push_back(std::thread([&](Queue* queue){
            int data, priority;
            
            for (int i = 0; i < item_per_thread*TOTAL_THREADS; i++) {
                data = messages[i];
                priority = priorities[i];
                queue->priority_enqueue(data, priority, 1);
            }
        }, queue));
    }
    
    /* This is the dequeueing thread; It will dequeue 6 messages and then stop */
    threads.push_back(std::thread([&](Queue* queue){
        int messages_retrieved = 0;
        while (messages_retrieved < 6) {
            int message = queue->dequeue(TOTAL_THREADS);
            if (message != -1) {
                std::cout << "Dequeued Message: " << std::to_string(message)+"\n";
                messages_retrieved++;
            }
        }
    }, queue));

    /* This line ensures that the main thread doesn't end up proceeding before the execution
       of all threads in the threads vector. */ 
    for (auto &thread: threads) {
        thread.join();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "\nStill in Queue after messages are removed:\n";
    queue->print_all();
    return 1;
}