#ifndef THREADSAFE_QUEUE_HPP
#define THREADSAFE_QUEUE_HPP

#include <stdexcept>
#include <queue>
#include <mutex>
#include <condition_variable>


template <typename T>
class threadsafe_queue
{
    mutable std::mutex m;
    std::queue<T> data;
    std::condition_variable cond_var;

public:

    typedef T   value_type;

    class threadsafe_queue_empty: public std::runtime_error
    {
    public:
        threadsafe_queue_empty(std::string const& err) : std::runtime_error(err) {}
    };

    threadsafe_queue() noexcept {}
    threadsafe_queue(threadsafe_queue const& other) noexcept
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    threadsafe_queue& operator=(threadsafe_queue const& other) noexcept
    {
        std::unique_lock<std::mutex> lockThis(this->m, std::defer_lock);
        std::unique_lock<std::mutex> lockOther(other.m, std::defer_lock);
        std::lock(this->m, other.m);
        data = other.data;
    }

    void push(T value) noexcept
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(value);
        cond_var.notify_one();
    }

    void pop(T& value)
    {
        std::unique_lock<std::mutex> lock(m);
        cond_var.wait(lock, [this]() { return !this->data.empty(); });
        value = data.front(); // move_no_except
        data.pop();
    }

    std::shared_ptr<T> pop()
    {
        std::unique_lock<std::mutex> lock(m);
        cond_var.wait(lock, [this]() { return !this->data.empty(); });
        std::shared_ptr<T> result(std::make_shared<T>(data.front()));
        data.pop();

        return result;
    }
};

#endif // THREADSAFE_QUEUE_HPP
