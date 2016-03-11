#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <iterator>
#include <mutex>
#include <future>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include "shape.hpp"
#include "threadsafe_queue.hpp"
#include <atomic>


template <class T>
void printOut(char const* str, T const& container)
{
    std::cout << str << std::endl;
    std::copy(container.begin(), container.end(), std::ostream_iterator<typename T::value_type>(std::cout, " "));
    std::cout << std::endl;
}

template <>
void printOut(char const* str, std::vector<std::unique_ptr<Shape>> const& container)
{
    std::cout << str << std::endl;
    for (auto& c : container)
    {
        std::cout << *c << " ";
    }
    std::cout << std::endl;
}

std::mutex g_m;

void ThreadFuncTask1(int num)
{
    std::vector<std::unique_ptr<Shape>> vec;
    Shape baseShape;

    for (int i=0; i < num; ++i)
    {
        vec.emplace_back(std::unique_ptr<Shape>(baseShape.ShapeMaker()));
    }

    std::lock_guard<std::mutex> lock(g_m);
    printOut("Shapes: ", vec);
}

template <class T>
auto ThreadFuncTask2(T first, T last) -> typename T::value_type
{
    typename T::value_type sum = std::accumulate(first, last, 0);

    return sum;
}

template <class T>
void ThreadFuncTask3(T value)
{
    if (std::floor(value) == value)
    {
        std::cout << "A number is integral!" << std::endl;
    }
    else
    {
        throw std::runtime_error("A number is fractional!");
    }
}

std::vector<std::exception_ptr>  g_exceptions;

template <class T, class TT>
void ThreadPushTask4(T& container, TT waitMs)
{
    for (;;)
    {
        srand(time(0));
        typename T::value_type const value = rand()%100;
        std::cout << "Push: " << value << std::endl;
        container.push(value);

        std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
    }
}

template <class T>
void ThreadPopTask4(T& container)
{
    for (;;)
    {
        try
        {
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
	// Uncomment to run...

    // TASK 1:
    //==================================================================================
    std::vector<std::thread> trV;
    std::srand(time(NULL));

    int numThreads = 0;
    while(!(numThreads = rand()%std::thread::hardware_concurrency())) {}
    std::cout << std::endl << "numThreads: " << numThreads << std::endl;

    int const numShape = rand()%10;
    std::cout << std::endl << "numShape = " << numShape << std::endl;

    for (int i=0; i < numThreads; ++i)
    {
        trV.emplace_back(std::thread(ThreadFuncTask1, numShape));
    }

    for (auto& tr : trV)
    {
        tr.join();
    }

    // TASK 2:
    //==================================================================================

//    srand(time(NULL));

//    int numThreads = 0;
//    while(!(numThreads = rand()%std::thread::hardware_concurrency())) {}
//    std::cout << std::endl << "numThreads: " << numThreads << std::endl;

//    int const numElements = rand()%20;
//    std::cout << std::endl << "numElements = " << numElements << std::endl;

//    std::vector<int> vec;
//    vec.resize(numElements);
//    std::generate(vec.begin(), vec.end(), []() { return rand()%100; });

//    int const partOfVector = vec.size()/numThreads;

//    printOut("Vector", vec);

//    std::vector<std::future<int>> fV;

//    auto first = vec.begin();
//    for (int i=0; i < numThreads-1; ++i)
//    {
//        auto last = first;
//        std::advance(last, partOfVector);
//        fV.emplace_back(std::async(ThreadFuncTask2<decltype(vec.begin())>, first, last));
//    }

//    int sum = 0;
//    if (first != vec.end())
//    {
//        sum = ThreadFuncTask2<decltype(vec.begin())>(first, vec.end());
//        std::cout << std::endl << "In main sum = " << sum << std::endl;
//    }

//    for (auto& f : fV)
//    {
//        if (f.valid())
//        {
//            int part_sum = f.get();
//            std::cout << std::endl << "In thread sum = " << part_sum << std::endl;
//            sum += part_sum;
//        }
//    }

//    std::cout << std::endl << "In main WHOLE sum = " << sum << std::endl;

    // TASK 3:
    //==================================================================================

//    std::cout << std::endl << "Enter a number = ";

//    std::string strNumber;
//    getline(std::cin, strNumber);
//    double number = std::atof(strNumber.c_str());

//    std::cout << std::endl << "You've entered " << number << std::endl;

//    std::future<void> f = std::async(ThreadFuncTask3<decltype(number)>, number);

//    try
//    {
//        f.get();
//    }
//    catch(std::runtime_error& e)
//    {
//        std::cout << std::endl << "Caught exception: " << e.what() << std::endl;
//    }

    // TASK 4:
    //==================================================================================

//    threadsafe_queue<int> MyQueue;
//    std::vector<std::thread> trV;
//
//    MyQueue.push(10);
//
//    trV.emplace_back(std::thread(ThreadPushTask4<decltype(MyQueue), int>, std::ref(MyQueue), 3000));
//    trV.emplace_back(std::thread(ThreadPushTask4<decltype(MyQueue), int>, std::ref(MyQueue), 2000));
//    trV.emplace_back(std::thread(ThreadPopTask4<decltype(MyQueue)>, std::ref(MyQueue)));
//
//    for (auto& tr : trV)
//    {
//        tr.detach();
//    }
//
//    for (;;)
//    {
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//        for(auto &e: g_exceptions)
//        {
//            try
//            {
//                if(e)
//                {
//                    std::rethrow_exception(e);
//                }
//            }
//            catch (const std::exception &e)
//            {
//                std::cout << "Caught exception: " << e.what() << std::endl;
//            }
//        }
//        g_exceptions.clear();
//    }

    std::cout << std::endl << "The end!" << std::endl;

    return 0;
}

