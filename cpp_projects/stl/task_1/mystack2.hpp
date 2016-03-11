#ifndef MyStack2_HPP
#define MyStack2_HPP

#include <stdexcept>
#include "single_linked_list.hpp"

template <typename T, int max = 10>
class MyStack2
{
    int apex;
    SingleLinkedList<T>* items;
public:
    class MyStack2Err: public std::runtime_error
    {
    public:
        MyStack2Err(std::string const& err) : runtime_error(err) {}
    };

    MyStack2() : apex(0)
    {
        items = new SingleLinkedList<T>;
    }
    MyStack2(MyStack2 const& );
    MyStack2(MyStack2&& );

    ~MyStack2()
    {
        delete items;
    }

    bool isEmpty() const noexcept { return items->isEmpty(); }
    bool isFull()  const noexcept { return max == apex; }
    int size()     const noexcept { return apex; }
    int capacity() const noexcept { return max; }
    T&       top()       throw(MyStack2Err);
    T const& top() const throw(MyStack2Err);
    void push(T const& ) throw(MyStack2Err);
    void push(T&& )      throw(MyStack2Err);
    void pop()           throw(MyStack2Err);
};

template <typename T, int max>
MyStack2<T, max>::MyStack2(MyStack2<T, max> const& lv)
{
    apex = lv.apex;
    items = new SingleLinkedList<T>;

    SingleLinkedList<T> temp;
    while (!lv.isEmpty())
    {
        temp.push(lv.items->front());
        lv.items->pop();
        items->push(temp.front());
        temp.pop();
    }
}

template <typename T, int max>
MyStack2<T, max>::MyStack2(MyStack2<T, max>&& rv)
{
    apex = rv.apex;
    items = new SingleLinkedList<T>;
    items = rv.items;
    rv.items = nullptr;
}

template <typename T, int max>
T& MyStack2<T, max>::top() throw(MyStack2Err)
{
    if (isEmpty()) throw MyStack2Err("Stack is empty!");
    return items->front();
}

template <typename T, int max>
T const& MyStack2<T, max>::top() const throw(MyStack2Err)
{
    if (isEmpty()) throw MyStack2Err("Stack is empty!");
    return items->front();
}

template <typename T, int max>
void MyStack2<T, max>::push(T const& item) throw(MyStack2Err)
{
    if (isFull()) throw MyStack2Err("Stack is full!");
    ++apex;
    items->push(item);
}

template <typename T, int max>
void MyStack2<T, max>::push(T&& item) throw(MyStack2Err)
{
    if (isFull()) throw MyStack2Err("Stack is full!");
    ++apex;
    items.push(std::move(item));
}

template <typename T, int max>
void MyStack2<T, max>::pop() throw(MyStack2Err)
{
    if (isEmpty()) throw MyStack2Err("Stack is empty!");
    --apex;
    items->pop();
}

#endif // MyStack2_HPP
