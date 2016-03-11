#ifndef SINGLE_LINKED_LIST_HPP
#define SINGLE_LINKED_LIST_HPP

template <typename T>
class SingleLinkedList
{
    struct Node
    {
        T item;
        Node* next;
    };
    Node* head;

public:
    SingleLinkedList() : head(nullptr) {}
    ~SingleLinkedList() { while (!isEmpty()) pop(); }
    bool isEmpty() const { return nullptr == head; }
    T& front();
    T const& front() const;
    void push(T const& );
    void push(T&& );
    void pop();
};

template <typename T>
T& SingleLinkedList<T>::front()
{
    return head->item;
}

template <typename T>
T const& SingleLinkedList<T>::front() const
{
    return head->item;
}

template <typename T>
void SingleLinkedList<T>::push(T const& item)
{
    Node* newNode = new Node;
    newNode->item = item;
    newNode->next = head;
    head = newNode;
}

template <typename T>
void SingleLinkedList<T>::push(T&& item)
{
    Node* newNode = new Node;
    newNode->item = std::move(item);
    newNode->next = head;
    head = newNode;
}

template <typename T>
void SingleLinkedList<T>::pop()
{
    Node* oldNode = head;
    head = oldNode->next;
    delete oldNode;
}

#endif // SINGLE_LINKED_LIST_HPP
