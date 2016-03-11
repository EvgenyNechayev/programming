#include <iostream>
#include <vector>
#include <stdexcept>
#include <thread>
#include <chrono>
#include "threadsafe_stack.hpp"
#include "lock_free_stack.hpp"


std::vector<std::exception_ptr>  g_exceptions;

template <class T>
void ThreadPushTask1(T& container)
{
    for (int i=0; i < 10000; ++i)
    {
        srand(time(0));
        typename T::value_type const value = rand()%100;
//        std::cout << "Push: " << value << std::endl;

        container.push(value);
    }
}

template <class T>
void ThreadPopTask1(T& container)
{
    for (int i=0; i < 10000; ++i)
    {
        try
        {
            *container.pop();
//            std::cout << "Pop: " << *container.pop() << std::endl;
        }
        catch (...)
        {
            g_exceptions.push_back(std::current_exception());
        }
    }
}

template <class T, class TT>
void ThreadPushTask2(T& container, TT waitMs)
{
    for (;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
        srand(time(0));
        typename T::value_type const value = rand()%100;
        std::cout << "Push: " << value << std::endl;
        container.push(value);
    }
}

template <class T, class TT>
void ThreadPopTask2(T& container, TT waitMs)
{
    for (;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
        try
        {
            *container.pop();
            std::cout << "Pop: " << *container.pop() << std::endl;
        }
        catch (...)
        {
            g_exceptions.push_back(std::current_exception());
        }
    }
}

int main()
{
	// uncomment to run...

    // TASK 1:
    //==================================================================================
    threadsafe_stack<int, spinlock> MyStack; // 0.0350035 s
//    threadsafe_stack<int, std::mutex> MyStack; // 0.120012 s
    std::vector<std::thread> trV;

    MyStack.push(10);

    auto start = std::chrono::high_resolution_clock::now();
    trV.emplace_back(std::thread(ThreadPushTask1<decltype(MyStack)>, std::ref(MyStack)));
    trV.emplace_back(std::thread(ThreadPushTask1<decltype(MyStack)>, std::ref(MyStack)));
    trV.emplace_back(std::thread(ThreadPopTask1<decltype(MyStack)>, std::ref(MyStack)));

    for (auto& tr : trV)
    {
        tr.join();
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time is " << std::chrono::duration<double>(end-start).count() << " s" << std::endl;

    for (;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        for(auto &e: g_exceptions)
        {
            try
            {
                if(e)
                {
                    std::rethrow_exception(e);
                }
            }
            catch (const std::exception &e)
            {
                std::cout << "Caught exception: " << e.what() << std::endl;
            }
        }
        g_exceptions.clear();
    }

    // TASK 2:
    //==================================================================================

    lock_free_stack<int> stack;
    std::vector<std::thread> trV;

//    for (int i=0; i<10; ++i)
//    {
//        stack.push(i);
//    }

//    for (int i=0; i<10; ++i)
//    {
//        std::cout << std::endl << "Stack: " << *stack.pop() << std::endl;
//    }

    trV.emplace_back(std::thread(ThreadPushTask2<decltype(stack), int>, std::ref(stack), 100));
    trV.emplace_back(std::thread(ThreadPushTask2<decltype(stack), int>, std::ref(stack), 100));
    trV.emplace_back(std::thread(ThreadPopTask2<decltype(stack), int>, std::ref(stack), 150));

    for (auto& tr : trV)
    {
        tr.detach();
    }

    for (;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        for(auto &e: g_exceptions)
        {
            try
            {
                if(e)
                {
                    std::rethrow_exception(e);
                }
            }
            catch (const std::exception &e)
            {
                std::cout << "Caught exception: " << e.what() << std::endl;
            }
        }
        g_exceptions.clear();
    }

    std::cout << std::endl << "The end!" << std::endl;

    return 0;
}

