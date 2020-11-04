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
        ./test4 
*/

std::vector <std::deque <std::string>> all_messages = {{"Zero" ,"One",
                                "Two", "Three"}, {"Four", "Five", 
                                "Six", "Seven"}, {"Eight", "Nine", "Ten", "Eleven"},
                                {"Twelve", "Thirteen", "Fourteen", "Fifteen"}};
std::vector <std::deque <int>> all_priorities = {{0,1,2,3},{4,5,6,7},{8,9,10,11},
                                                {12,13,14,15}};

std::mutex thread_creation; 
std::condition_variable creating_thread;
int job_threads_created = 0;

void Queue::enqueue_job(int thread_number) {
    std::deque <std::string> messages = all_messages[thread_number];
    std::deque <int> priorities = all_priorities[thread_number];
    
    bool has_created = false;
    while (!messages.empty()) {
        if (!has_created) {
            std::unique_lock<std::mutex> Create(thread_creation);
            job_threads_created++;
            has_created = true;
        }
        else if (job_threads_created == TOTAL_THREADS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(rand()%250));
            std::unique_lock<std::mutex> Lock(Mutex);
            Condition.wait(Lock, [this]{return !this->in_use;});
            priority_enqueue(messages.front(), priorities.front(), thread_number+1);
            messages.pop_front();
            priorities.pop_front();
            Condition.notify_one();
        }
        else {
            continue;
        }
    }
}

int main(int argc, char* argv[]) {
    Queue* queue = new Queue();
    std::vector<std::thread> thread_pool;

    for (int thread_number = 0; thread_number < TOTAL_THREADS; thread_number++) {
        thread_pool.push_back(std::thread([&](int thread_number, Queue* queue){
            queue->enqueue_job(thread_number);
        }, thread_number, queue));
    }
        
    /* This is the dequeueing thread; It will dequeue 6 messages and then stop */
    thread_pool.push_back(std::thread([&](Queue* queue){
        int messages_retrieved = 0;
        while (messages_retrieved < 6) {
            std::unique_lock<std::mutex> Lock(queue->Mutex);
            queue->Condition.wait(Lock, [queue]{return !queue->in_use;});
            if (!queue->get_size()) {
                queue->Condition.notify_one();
                continue;
            }
            std::string message = queue->dequeue(TOTAL_THREADS);
            if (message != "-1") {
                messages_retrieved++;
            }
            else {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::this_thread::yield; // This does not seem to be fulfilling the purpose of sending the thread r
            }
            queue->Condition.notify_one();
        }
    }, queue));

    /* This line ensures that the main thread doesn't end up proceeding before the execution
       of all threads in the threads vector. */ 
    for (auto &thread: thread_pool) {
        thread.join();
    }
    
    std::cout << "\nStill in Queue after messages are removed:\n";
    queue->print_all();
    return 1;
}