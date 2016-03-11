#ifndef MYQUEUE_HPP
#define MYQUEUE_HPP

#include <stdexcept>

template <typename T, int max = 10>
class MyQueue
{
    struct Node
    {
        T item;
        Node* next;
    };
    char* pBuffer;
    Node* head;
    Node* tail;
    int current;
    int n;
public:
    class MyQueueErr: public std::runtime_error
    {
    public:
        MyQueueErr(std::string const& err) : runtime_error(err) {}
    };

    MyQueue() : head(nullptr), tail(nullptr), n(0), current(0)
    {
        pBuffer = new char[max * sizeof(Node)];
    }

    ~MyQueue();

    bool isEmpty() const noexcept { return nullptr == head; }
    bool isFull()  const noexcept { return max == n; }
    int size()     const noexcept { return n; }
    int capacity() const noexcept { return max; }
    T& front()              throw(MyQueueErr);
    T const& front() const  throw(MyQueueErr);
    void push(T const& ) throw(MyQueueErr);
    void pop()          throw(MyQueueErr);
};

template <typename T, int max>
MyQueue<T, max>::~MyQueue()
{
    delete [] pBuffer;
}

template <typename T, int max>
T& MyQueue<T, max>::front() throw(MyQueueErr)
{
    if (isEmpty()) throw MyQueueErr("Queue is empty!");
    return head->item;
}

template <typename T, int max>
T const& MyQueue<T, max>::front() const throw(MyQueueErr)
{
    if (isEmpty()) throw MyQueueErr("Queue is empty!");
    return head->item;
}

template <typename T, int max>
void MyQueue<T, max>::push(T const& item) throw(MyQueueErr)
{
    if (isFull()) throw MyQueueErr("Queue is full!");
    Node* newNode = reinterpret_cast<Node*>(pBuffer + (current * sizeof(Node)));
    newNode->item = item;
    newNode->next = nullptr;

    ++current;
    if (++n == max)
        current = 0;

    if (nullptr == tail) { tail = newNode; }
    else
    {
        tail->next = newNode;
        if (nullptr == head)
        {
            unsigned int tempTailAddr = reinterpret_cast<unsigned int>(tail);
            head = reinterpret_cast<Node*>(tempTailAddr);
        }
        tail = newNode;
    }
}

template <typename T, int max>
void MyQueue<T, max>::pop() throw(MyQueueErr)
{
    if (isEmpty()) throw MyQueueErr("Queue is empty!");
    head = head->next;
    --n;
}

#endif // MYQUEUE_HPP
