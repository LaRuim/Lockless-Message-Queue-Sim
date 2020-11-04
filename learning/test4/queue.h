class Node {
    std::string data;
    int priority;
    Node* next_node;
    Node* previous_node;

    public:
        Node(std::string data, int priority, Node* previous_node, Node* next_node);
        std::string get_data();
        int get_priority();
        Node* get_next_node();
        Node* get_previous_node();
        int set_next_node(Node* new_next_node);
        int set_previous_node(Node* new_previous_node);
};

class Queue {

    
    Node* head;
    Node* tail;
    int size;
    Node* get_head();
    Node* get_tail();
    int set_head(Node* new_head);
    int set_tail(Node* new_tail);
    int insert_before(Node* where, std::string data, int priority);

    public:
        bool in_use;
        std::mutex Mutex; 
        std::condition_variable Condition;
        Queue();
        int get_size();
        int peek_priority();
        std::string peek_data();
        int priority_enqueue(std::string data, int priority, int thread_number);
        int append(std::string data, int priority);
        std::string dequeue(int thread_number);
        int print_all();
        void enqueue_job(int thread_number);
        bool is_not_in_use();
};

const int TOTAL_THREADS = 4;
const int MAX_SIZE = 4;