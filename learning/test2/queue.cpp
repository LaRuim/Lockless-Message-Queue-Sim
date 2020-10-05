#include <cstdio>
#include <ctime>
#include <chrono>
#include <iostream>
#include <thread>
#include "queue.h"

/* CONSTRUCTORS */

Node::Node(int data, int priority, Node* previous_node, Node* next_node){
    this->priority = priority;
    this->data = data;
    this->previous_node = previous_node;
    this->next_node = next_node;
}

Queue::Queue() {
    head = NULL;
    tail = NULL;
    size = 0;
}


/* QUEUE FUNCTIONS */

// The visible enqueue function;
int Queue::priority_enqueue(int data, int priority, int thread_number) {

    if (!get_size()) {
        std::this_thread::sleep_for(std::chrono::milliseconds((TOTAL_THREADS-thread_number-1)*50));
        std::cout << "Appending " + std::to_string(priority) + ". " + std::to_string(data) + " by thread " + std::to_string(thread_number) + "\n";
        append(data, priority);
        return 1;
    }

    else {
        Node* iterator = get_head();
        int current_priority;
        std::this_thread::sleep_for(std::chrono::milliseconds((TOTAL_THREADS-thread_number-1)*50));   
        while (iterator != nullptr) {
            current_priority = iterator->get_priority();
            if (priority < current_priority) {
                std::cout << "Enqueueing " + std::to_string(priority) + ". " + std::to_string(data) + " by thread " + std::to_string(thread_number) + "\n";
                insert_before(iterator, data, priority);
                return 1;
            }
            iterator = iterator->get_next_node();
        }
        append(data, priority);
    }
    
    return 0;
}

// The visible dequeue function
int Queue::dequeue(int thread_number) {
    int peek_value = peek_priority();
    std::this_thread::sleep_for(std::chrono::milliseconds((TOTAL_THREADS-thread_number-1)*50));
    if (peek_value != -1) {
        Node* hold = get_head();

        // std::this_thread::sleep_for(std::chrono::milliseconds((TOTAL_THREADS-thread_number-1)*50));
        
        /*  The above causes a double-free error; One thread tries to free an already freed location.
            This is probably the best demonstration of there being a race condition, but 
            it's commented out as it throws an error. To try it out, uncomment the above
            line and comment out the initial delay in the function, right outside the if block.  */
            
        set_head(hold->get_next_node());
        delete(hold);
        size--;
    }
    return peek_value;
}

// Helper append function;
int Queue::append(int data, int priority){ 
    if (!get_size()) {
        set_head(new Node(data, priority, nullptr, nullptr));
        size++;
        return 1;
    }
    else if (get_size() == 1) {
        set_tail(new Node(data, priority, get_head(), nullptr));
        get_head()->set_next_node(get_tail());
        size++;
        return 1;
    }
    else{
        Node* last = new Node(data, priority, get_tail(), nullptr);
        get_tail()->set_next_node(last);
        set_tail(last);
        size++;
        return 1;
    }
    return 0;
}

// Helper arbitrary insert function;
int Queue::insert_before(Node* where, int data, int priority){
    Node* new_node = new Node(data, priority, where->get_previous_node(), where);
    if (where->get_previous_node()) {
        where->get_previous_node()->set_next_node(new_node);
    }
    where->set_previous_node(new_node);
    if (where == get_head()) {
        set_head(new_node);
    }
    size++;
    return 1;
}

// Print all contents of the queue;
int Queue::print_all() {
    if (!get_size()) {
        std::cout << "Empty.\n";
        std::cout << "Size: " << get_size() << '\n';
        return 1;
    }
    else if (get_size() == 1){
        std::cout << peek_priority() << ". " << peek_data() << "\nSize: " << get_size() << "\n";
        return 1;
    }
    else if (get_size() > 1){
        Node* iterator = get_head();
        while (iterator){
            std::cout << iterator->get_priority() << ". " << iterator->get_data() << '\n';
            iterator = iterator->get_next_node();
        }
        std::cout << "\nSize: " << get_size() << "\n";
        return 1;
    }
    else{
       return 0;
    }
}


/* GETTERS & SETTERS */

int Node::get_priority() {
    return priority;
}

int Node::get_data() {
    return data;
}

Node* Node::get_previous_node() {
    return previous_node;
}

Node* Node::get_next_node() {
    return next_node;
}

int Node::set_previous_node(Node* new_previous_node) {
    previous_node = new_previous_node;
    return 1;
}

int Node::set_next_node(Node* new_next_node) {
    next_node = new_next_node;
    return 1;
}

int Queue::get_size() {
    return size;
}

Node* Queue::get_head() {
    return head;
}

Node* Queue::get_tail() {
    return tail;
}

int Queue::peek_priority() {
    Node* head = get_head();
    if (head) return head->get_priority();
    return -1;
}

int Queue::peek_data() {
    Node* head = get_head();
    if (head) return head->get_data();
    return -1;
}

int Queue::set_head(Node* new_head) {
    head = new_head;
    return 1;
}

int Queue::set_tail(Node* new_tail) {
    tail = new_tail;
    return 1;
}