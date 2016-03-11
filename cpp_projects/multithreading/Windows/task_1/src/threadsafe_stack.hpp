#ifndef THREADSAFE_STACK_HPP
#define THREADSAFE_STACK_HPP

#include <stdexcept>
#include <stack>
#include <mutex>
#include <memory>


template <class T>
class threadsafe_stack
{
    std::stack<T> data;
    mutable std::mutex m;

public:

    typedef T   value_type;

    class threadsafe_stack_empty: public std::runtime_error
    {
    public:
        threadsafe_stack_empty(std::string const& err) : std::runtime_error(err) {}
    };

    threadsafe_stack() noexcept {}
    threadsafe_stack(threadsafe_stack const& other) noexcept
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    threadsafe_stack& operator=(threadsafe_stack const& other) noexcept
    {
        std::lock(this->m, other.m);
        std::lock_guard<std::mutex> lockThis(this->m, std::adopt_lock);
        std::lock_guard<std::mutex> lockOther(other.m, std::adopt_lock);
        data = other.data;
    }

    void push(T value) noexcept
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(value);
    }

    void pop(T& value) throw(threadsafe_stack_empty)
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
        {
            throw threadsafe_stack_empty("Stack is empty!");
        }
        value = data.top();
        data.pop();
    }

    std::shared_ptr<T> pop() throw(threadsafe_stack_empty)
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
        {
            throw threadsafe_stack_empty("Stack is empty!");
        }
        std::shared_ptr<T> result(std::make_shared<T>(data.top()));
        data.pop();

        return result;
    }
};



#endif // THREADSAFE_STACK_HPP
