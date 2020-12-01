class Node {
    int data;
    int priority;
    Node* next_node;
    Node* previous_node;

    public:
        Node(int data, Node* previous_node, Node* next_node);
        int get_data();
        int get_priority();
        Node* get_next_node();
        Node* get_previous_node();
        int set_next_node(Node* new_next_node);
        int set_previous_node(Node* new_previous_node);
};

class Queue {

    
    Node* head;
    Node* tail;
    Node* get_head();
    Node* get_tail();
    int set_head(Node* new_head);
    int set_tail(Node* new_tail);

    public:
        bool in_use;
        std::mutex Mutex; 
        std::condition_variable Condition;
        Queue();
        int size;
        int get_size();
        int peek_priority();
        int peek_data();
        int enqueue_message(int data, int thread_number);
        int append(int data);
        int dequeue(int thread_number);
        int print_all();
        bool is_not_in_use();
};

class Thread {
    public:
        int enqueue_to_another_thread(Thread* receiver, int message);
        Thread(int id);
        int id;
        int work_done;
        Queue* message_queue;
};

const int TOTAL_THREADS = 4;
const int MAX_SIZE = 4;
const int TASKS = 15;