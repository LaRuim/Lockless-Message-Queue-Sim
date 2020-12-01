#include <cstdio>
#include <ctime>
#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>
#include <memory>
#include "queue.h"

/* CONSTRUCTORS */

Node::Node(int data, Node* previous_node, Node* next_node) noexcept {
    this->data = data;
    this->previous_node = previous_node;
    this->next_node = next_node;
}

Queue::Queue() {
    
    head = NULL;
    tail = NULL;
    size = 0;
    in_use = false;
}

Thread::Thread(int uid) {
    id = uid;
    message_queue = new Queue();
}


/* QUEUE FUNCTIONS */

// The visible enqueue function;
int Queue::enqueue_message(int data, int thread_number) {
    append(data);
    return 0;
}

// The visible dequeue function
int Queue::dequeue(int thread_number) {
    srand(time(0));
    
    //Mutex.lock();
    
    int peek_value = peek_data();
    if (peek_value != -1) {
        auto store_head = head.load();
        while (!head.compare_exchange_weak(store_head, store_head->get_next_node())){};
        size--;
        if (!size) {
            set_tail(nullptr);
        }
        else {
            get_head()->set_previous_node(nullptr);
        }
        //std::cout << "Dequeued Message: " << peek_value+"\n";
    }
    //Mutex.unlock();
    return peek_value;
}

// Helper append function;
int Queue::append(int data){ 
    in_use = true;
    if (!get_size()) {
        set_head(new Node(data, nullptr, nullptr));
        set_tail(get_head());
        size++;
        in_use = false;
        return 1;
    }
    else if (get_size() == 1) {
        set_tail(new Node(data, get_head(), nullptr));
        get_head()->set_next_node(get_tail());
        size++;
        in_use = false;
        return 1;
    }
    else {
        Node* last = new Node(data, get_tail(), nullptr);
        get_tail()->set_next_node(last);
        set_tail(last);
        size++;
        in_use = false;
        return 1;
    }
    return 0;
}

// Helper arbitrary insert function;
int Queue::insert_before(Node* where, int data){
    in_use = true;
    Node* new_node = new Node(data, where->get_previous_node(), where);

    if (where->get_previous_node()) {
        where->get_previous_node()->set_next_node(new_node);
    }
    where->set_previous_node(new_node);
    if (where == get_head()) {
        set_head(new_node);
    }
    size++;
    in_use = false;
    return 1;
}

// Print all contents of the queue;
int Queue::print_all() {
    int size = get_size();
    if (!size) {
        std::cout << "Empty.\n";
        std::cout << "Size: " << size << '\n';
        return 1;
    }
    else if (size == 1){
        std::cout << peek_data() << "\nSize: " << size << "\n";
        return 1;
    }
    else if (size > 1){
        Node* iterator = get_head();
        int index = 0;
        while (iterator && index < size){
            std::cout << iterator->get_data() << '\n';
            iterator = iterator->get_next_node();
            index++;
        }
        std::cout << "\nSize: " << size << "\n";
        return 1;
    }
    else{
       return 0;
    }
}


/* GETTERS & SETTERS */


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


int Queue::peek_data() {
    Node* head = get_head();
    if (head) return head->get_data();
    return -1;
}

int Queue::set_head(Node* new_head) {
    
    auto store_head = head.load();
    while (!head.compare_exchange_weak(store_head, new_head)){};
    return 1;
}

int Queue::set_tail(Node* new_tail) {
    auto store_tail = tail.load();
    while (!tail.compare_exchange_weak(store_tail, new_tail)){};
    return 1;
}

bool Queue::is_not_in_use() {
    return !in_use;
}