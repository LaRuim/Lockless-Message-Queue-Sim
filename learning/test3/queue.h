class Node {
    int data;
    int priority;
    Node* next_node;
    Node* previous_node;

    public:
        Node(int data, int priority, Node* previous_node, Node* next_node);
        int get_data();
        int get_priority();
        Node* get_next_node();
        Node* get_previous_node();
        int set_next_node(Node* new_next_node);
        int set_previous_node(Node* new_previous_node);
};

class Queue {
    bool in_use;
    std::mutex Mutex; 
    std::condition_variable Condition;
    
    Node* head;
    Node* tail;
    int size;
    Node* get_head();
    Node* get_tail();
    int set_head(Node* new_head);
    int set_tail(Node* new_tail);
    int insert_before(Node* where, int data, int priority);

    public:
        Queue();
        int get_size();
        int peek_priority();
        int peek_data();
        int priority_enqueue(int data, int priority, int thread_number);
        int append(int data, int priority);
        int dequeue(int thread_number);
        int print_all();
        bool is_not_in_use();
};

const int TOTAL_THREADS = 4;
const int MAX_SIZE = 4;