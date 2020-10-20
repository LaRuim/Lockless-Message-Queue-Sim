#include <iostream>
#include <thread>
#include <vector>

const int TOTAL_THREADS = 5;
const int EACH_THREAD_RESONSIBILITY = 100000;

void race(int* generic_shared_variable, int thread_number)  {
    for (int j = 0; j < EACH_THREAD_RESONSIBILITY; j++){
        //std::cout << "Thread " << thread_number << " just changed " << *generic_shared_variable; 
        *generic_shared_variable = *generic_shared_variable + 1; 
        //std::cout << " to " << *generic_shared_variable << "\n"; 
    }
}
 
int main(void) {
    int* generic_shared_variable = new int(0);

    std::vector<std::thread> threads;
    
    for (int thread_number = 0; thread_number < TOTAL_THREADS; thread_number++) {
        threads.push_back(std::thread(race, generic_shared_variable, thread_number));
    }
    
    for (auto &thread: threads) {
        thread.join();
    }
    std::cout << "This is generic_shared_variable: " << *generic_shared_variable << '\n';
    int expected = EACH_THREAD_RESONSIBILITY * TOTAL_THREADS;
    std::cout << "Expected: " << expected << '\n'; 
    return 0;
}