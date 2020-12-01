#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <random>
#include "queue.h"

/* 
    Run the program as:
        ./locked_mq
*/

int Thread::enqueue_to_another_thread(Thread* receiver, int message) {
    ////std::cout << "Thread " << this->id+1 << " wants to acquire mutex to enqueue to Thread " << receiver->id+1 << ".\n";
    std::unique_lock<std::mutex> Lock(receiver->message_queue->Mutex);
    ////std::cout << "Thread " << this->id+1 << " acquired mutex to enqueue to Thread " << receiver->id+1 << " and will now wait on the condvar.\n";
    receiver->message_queue->Condition.wait(Lock, [receiver]{return !(receiver->message_queue->in_use);});
    ////std::cout << "Thread " << this->id+1 << " is done waiting on the cond_var and will now enqueue to Thread " << receiver->id+1 << ".\n";
    receiver->message_queue->enqueue_message(message, this->id);
    ////std::cout << "Thread " << this->id+1 << " finished enqueueing to Thread " << receiver->id+1 << ".\n";
    Lock.unlock();
    receiver->message_queue->Condition.notify_one();
    return 1;
}

int main(int argc, char* argv[]) {
    bool* lone_thread;
    *lone_thread = false;
    auto start = std::chrono::high_resolution_clock::now();
    srand(time(0));
    std::vector<std::thread> thread_pool;
    std::vector<Thread*> thread_objects;
    for (int i = 0; i < TOTAL_THREADS; i++) {
        Thread* thread = new Thread(i);
        thread_objects.push_back(thread);
    }
    thread_pool.push_back(std::thread([&](Thread* thread_object){
        //queue->enqueue_job(0);
        std::random_device rd;
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> distr(150, 250);
        std::uniform_int_distribution<> distr3(0, 3);
        int random_number = distr(gen);
        for (int i = 0; i < 4; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(random_number)); // Represents initial work by initial thread.
            thread_object->enqueue_to_another_thread(thread_objects[1], distr3(gen));
            std::cout << "Thread " << 1 << " enqueued to Thread " << 2 << "\n";
            random_number = distr(gen);
            thread_object->enqueue_to_another_thread(thread_objects[2], distr3(gen));
            std::cout << "Thread " << 1 << " enqueued to Thread " << 3 << "\n";
            random_number = distr(gen);
            thread_object->enqueue_to_another_thread(thread_objects[3], distr3(gen));
            std::cout << "Thread " << 1 << " enqueued to Thread " << 4 << "\n";
        }
        return;

    }, thread_objects[0]));
    for (int thread_number = 1; thread_number < TOTAL_THREADS; thread_number++) {
        thread_pool.push_back(std::thread([&](int thread_number, std::vector<Thread*> thread_objects, bool* lone_thread){
            int work_done = 0;
            while (true) {
                std::random_device rd;
                std::mt19937 gen(rd()); 
                std::uniform_int_distribution<> distr(150, 250);
                std::uniform_int_distribution<> distr2(1, 3);
                std::uniform_int_distribution<> distr3(0, 3);
                std::this_thread::sleep_for(std::chrono::milliseconds(distr(gen))); // Represents initial work by initial thread.
                std::cout << "Thread " << thread_number+1 << " did initial, independent work.\n";
                std::unique_lock<std::mutex> Lock(thread_objects[thread_number]->message_queue->Mutex);
                thread_objects[thread_number]->message_queue->Condition.wait_for(Lock, std::chrono::milliseconds(5000), [thread_objects, thread_number]{return thread_objects[thread_number]->message_queue->get_size() > 0;});
                int message = thread_objects[thread_number]->message_queue->dequeue(thread_number);

                switch (message) {
                    case 0:
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        std::cout << "Thread " << thread_number+1 << " did Work Type 1\n"; // work type 1
                        break;
                    case 1:
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        std::cout << "Thread " << thread_number+1 << " did Work Type 2\n"; // work type 2
                        break;
                    case 2:
                        std::this_thread::sleep_for(std::chrono::milliseconds(250));
                        std::cout << "Thread " << thread_number+1 << " did Work Type 3\n"; // work type 3
                        break;
                    case 3:
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        std::cout << "Thread " << thread_number+1 << " did Work Type 4\n"; // work type 4
                        break;
                    default:
                        break;
                }
                Lock.unlock();
                thread_objects[thread_number]->message_queue->Condition.notify_all(); 
                int where_to_message = distr2(gen);
                while (where_to_message == thread_number) {
                    where_to_message = distr2(gen);
                }
                if (!(message+1)) {
                    *lone_thread = true;
                    break;
                }

                thread_objects[thread_number]->enqueue_to_another_thread(thread_objects[where_to_message], distr3(gen));
                std::cout << "Thread " << thread_number+1 << " enqueued to Thread " << where_to_message+1 << "\n";
                work_done++;
                //std::cout << "Work count of Thread " << thread_number + 1 << " is " << work_done << "\n";
                if (work_done == TASKS) {
                    break;
                }
            }
        }, thread_number, thread_objects, lone_thread));
    }

    /* This line ensures that the main thread doesn't end up proceeding before the execution
       of all threads in the threads vector. */ 
    for (auto &thread: thread_pool) {
        thread.join();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    auto time = duration.count();
    if (*lone_thread) {
        time -= 5000000;
    }
    std::cout << "Execution time: " << (double)time/1000000 << " seconds\n";
    //std::cout << (double)time/1000000 << "\n";
    return 1;
}