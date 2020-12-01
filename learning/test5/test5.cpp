#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <chrono>
#include <string>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include "queue.h"

/* 
    Run the program as:
        ./test5 
*/

int Thread::enqueue_to_another_thread(Thread* receiver, int message) {
    std::unique_lock<std::mutex> Lock(receiver->message_queue->Mutex);
    receiver->message_queue->Condition.wait(Lock, [receiver]{return !(receiver->message_queue->in_use);});
    receiver->message_queue->enqueue_message(message, 0, 0);
    receiver->message_queue->Condition.notify_all();
    return 1;
}

int main(int argc, char* argv[]) {
    srand(200);
    std::vector<std::thread> thread_pool;
    std::vector<Thread*> thread_objects;
    for (int i = 0; i < TOTAL_THREADS; i++) {
        Thread* thread = new Thread(i);
        thread_objects.push_back(thread);
    }
    thread_pool.push_back(std::thread([&](Thread* thread_object){
        //queue->enqueue_job(0);
        
        int random_number = rand()%25;
        std::this_thread::sleep_for(std::chrono::milliseconds(random_number)); // Represents initial work by initial thread.
        thread_object->enqueue_to_another_thread(thread_objects[1], random_number%4);
        std::cout << "Thread " << 1 << " enqueued to Thread " << 2 << "\n";
        random_number = rand()%25;

    }, thread_objects[0]));
    for (int thread_number = 1; thread_number < TOTAL_THREADS; thread_number++) {
        thread_pool.push_back(std::thread([&](int thread_number, std::vector<Thread*> thread_objects){
            int work_done = 0;
            while (true) {
                int random_number = rand()%25;
                std::this_thread::sleep_for(std::chrono::milliseconds(random_number)); // Represents initial work by initial thread.
                std::cout << "Thread " << thread_number+1 << " did pre-message queue work.\n";
                std::unique_lock<std::mutex> Lock(thread_objects[thread_number]->message_queue->Mutex);
                thread_objects[thread_number]->message_queue->Condition.wait_for(Lock, std::chrono::milliseconds(5000), [thread_objects, thread_number]{return thread_objects[thread_number]->message_queue->get_size() > 0;});
                int message = thread_objects[thread_number]->message_queue->dequeue(thread_number);
                thread_objects[thread_number]->message_queue->Condition.notify_one(); 
                switch (message) {
                    case 0:
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        std::cout << "Thread " << thread_number+1 << " did Work Type 1\n"; // work type 1
                        break;
                    case 1:
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        std::cout << "Thread " << thread_number+1 << " did Work Type 2\n"; // work type 2
                        break;
                    case 2:
                        std::this_thread::sleep_for(std::chrono::milliseconds(25));
                        std::cout << "Thread " << thread_number+1 << " did Work Type 3\n"; // work type 3
                        break;
                    case 3:
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        std::cout << "Thread " << thread_number+1 << " did Work Type 4\n"; // work type 4
                        break;
                    default:
                        break;
                }
                int where_to_message = random_number*message%TOTAL_THREADS;
                if (!where_to_message) {
                    where_to_message++;
                }
                if (where_to_message == thread_number) {
                    where_to_message = (thread_number+1)%TOTAL_THREADS;
                    if (!where_to_message) {
                        where_to_message++;
                    }
                }
                if (!(message+1)) {
                    break;
                }
                thread_objects[thread_number]->enqueue_to_another_thread(thread_objects[where_to_message], random_number%4);
                std::cout << "Thread " << thread_number+1 << " enqueued to Thread " << where_to_message+1 << "\n";
                work_done++;
                std::cout << "Work count of Thread " << thread_number + 1 << " is " << work_done << "\n";
                if (work_done == 5) {
                    break;
                }
            }
        }, thread_number, thread_objects));
    }

    /* This line ensures that the main thread doesn't end up proceeding before the execution
       of all threads in the threads vector. */ 
    for (auto &thread: thread_pool) {
        thread.join();
    }
    
    return 1;
}