#ifndef MYSTACK_HPP
#define MYSTACK_HPP

#include <stdexcept>

template <typename T, int max = 10>
class MyStack
{
    int apex;
    T   items[max];
public:
    class MyStackErr: public std::runtime_error
    {
    public:
        MyStackErr(std::string const& err) : runtime_error(err) {}
    };

    MyStack() noexcept : apex(0) {}
    MyStack(MyStack const& ) noexcept;
    MyStack(MyStack&& ) noexcept;

    bool isEmpty() const noexcept { return 0 == apex; }
    bool isFull()  const noexcept { return max == apex; }
    int size()     const noexcept { return apex; }
    int capacity() const noexcept { return max; }
    T&       top()       throw(MyStackErr);
    T const& top() const throw(MyStackErr);
//    void push(T const& ) throw(MyStackErr);
    template<typename TT> void push(TT&& )      throw(MyStackErr);
    void pop()           throw(MyStackErr);

    T& operator[](int) throw(MyStackErr);
    T const& operator[](int) const throw(MyStackErr);
};

template <typename T, int max>
MyStack<T, max>::MyStack(MyStack<T, max> const& lv) noexcept
{
    apex = lv.apex;
    for (int i = 0; i < lv.size(); ++i)
    {
        items[i] = lv.items[i];
    }
}

template <typename T, int max>
MyStack<T, max>::MyStack(MyStack<T, max>&& rv) noexcept
{
    apex = rv.apex;
    for (int i = 0; i < rv.size(); ++i)
    {
        items[i] = std::move(rv.items[i]);
    }
}

template <typename T, int max>
T& MyStack<T, max>::top() throw(MyStackErr)
{
    if (isEmpty()) throw MyStackErr("Stack is empty!");
    return items[apex - 1];
}

template <typename T, int max>
T const& MyStack<T, max>::top() const throw(MyStackErr)
{
    if (isEmpty()) throw MyStackErr("Stack is empty!");
    return items[apex - 1];
}

template <typename T, int max>
template <typename TT>
void MyStack<T, max>::push(TT&& item) throw(MyStackErr)
{
    if (isFull()) throw MyStackErr("Stack is full!");
    items[apex++] = std::forward<TT>(item);
}

//template <typename T, int max>
//void MyStack<T, max>::push(T&& item) throw(MyStackErr)
//{
//    if (isFull()) throw MyStackErr("Stack is full!");
//    items[apex++] = std::move(item);
//}

template <typename T, int max>
void MyStack<T, max>::pop() throw(MyStackErr)
{
    if (isEmpty()) throw MyStackErr("Stack is empty!");
    --apex;
}

template <typename T, int max>
T& MyStack<T, max>::operator[](int i) throw(MyStackErr)
{
    if (i >= 0 && i < apex)
        return items[i];
    else throw MyStackErr("Incorrect index!");
}

template <typename T, int max>
T const& MyStack<T, max>::operator[](int i) const throw(MyStackErr)
{
    if (i >= 0 && i < apex)
        return items[i];
    else throw MyStackErr("Incorrect index!");
}

#endif // MYSTACK_HPP
