#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <iterator>
#include "threadsafe_stack.hpp"


template <class T>
void printOut(char const* str, T const& container)
{
    std::cout << str << std::endl;
    std::copy(container.begin(), container.end(), std::ostream_iterator<typename T::value_type>(std::cout, " "));
    std::cout << std::endl;
}

void ThreadFuncTask1(std::string const& fileName)
{
    std::ifstream iFile(fileName.c_str());

    if (!iFile.is_open())
    {
        std::cout << "Error! Can't open file!" << std::endl;
        return;
    }

    std::vector<char> content;
    while (!iFile.eof())
    {
        content.push_back(iFile.get());
    }

    std::transform(content.begin(), content.end(), content.begin(), [](char ch) -> char { return (::isalpha(ch)) ? ::toupper(ch) : ch; });

    std::ofstream oFile(("_" + fileName).c_str());
    std::copy(content.begin(), content.end(), std::ostream_iterator<char>(oFile));

    iFile.close();
    oFile.close();
}

void ThreadFuncTask2(int delay, int m)
{
    for (int i=0; i < 3; ++i)
    {
        Sleep(1000 + delay*1000);
        std::cout << "Beep!!! delay=" << delay << " seconds melody=" << m << std::endl;
        Beep(m, 300);
    }
}

template <class T, class TT>
void ThreadFuncTask3(T first, T last, TT& time)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Processing in a thread..." << std::endl;
    std::transform(first, last, first, [](typename T::value_type& e) -> typename T::value_type { return std::abs(e); } );
    auto end = std::chrono::high_resolution_clock::now();
    time = end-start;
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

template <class T, class TT>
void ThreadPopTask4(T& container, TT waitMs)
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

        std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
    }
}

int main()
{
	// Uncomment to run...

    // TASK 1:
    //==================================================================================
    std::vector<std::thread> trV;
    char* fileName[] = {"1.txt", "2.txt", "3.txt", "4.txt"};

    for (auto fN : fileName)
    {
        trV.emplace_back(std::thread(ThreadFuncTask1, fN));
    }

    for (auto& tr : trV)
    {
        tr.join();
    }

    // TASK 2:
    //==================================================================================
//    std::vector<int> vMelody = {261, 293, 329, 349, 392, 440, 493};
//    std::vector<std::thread> trV;
//    int const numTreads = 5;
//    int const numSec = 3;

//    srand(time(0));
//    int waitSeconds = 0;
//    for (int i=0; i < numTreads; ++i)
//    {
//        int delay = rand() % numSec;
//        trV.emplace_back(std::thread(ThreadFuncTask2, delay, vMelody.at(rand() % (vMelody.size()))));
//        waitSeconds += 3*delay;
//    }

//    for (auto& tr : trV)
//    {
//        tr.detach();
//    }

//    std::this_thread::sleep_for(std::chrono::seconds(waitSeconds + 2));

    // TASK 3:
    //==================================================================================
//    std::deque<int> d;
//    srand(time(0));
//    int numElements = 10000; //rand() % 1000;

//    d.resize(numElements);
//    std::generate(d.begin(), d.end(), []() { return ((rand() % 2) ? 1 : -1)*(rand() % 100); });

//    int const numCores = std::thread::hardware_concurrency();
//    std::cout << "Number of cores: " << numCores << std::endl;
//    if (0 == numCores)
//    {
//        std::cout << "A number of cores equals 0, can't continue..." << std::endl;
//        return 1;
//    }

//    std::cout << "Please choose a number of threads: 1.." << numCores << std::endl;

//    int numTreads;
//    for (;;)
//    {
//        std::string strNumTreads;
//        getline(std::cin, strNumTreads);
//        numTreads = std::atoi(strNumTreads.c_str());
//        if (0 == numTreads || numTreads > numCores)
//        {
//            std::cout << std::endl << "Please choose a number of threads: 1.." << numCores << std::endl;
//        }
//        else
//        {
//            std::cout << std::endl << "You've chosen " << numTreads << " threads" << std::endl;
//            break;
//        }
//    }

//    int partOfDeque = d.size()/numTreads;

//    std::vector<std::chrono::duration<double>> procTime(numTreads);
//    std::vector<std::thread> trV;

//    auto first = d.begin();
//    for (int i=0; i < numTreads-1; ++i)
//    {
//        auto last = first;
//        std::advance(last, partOfDeque);
//        trV.emplace_back(std::thread(ThreadFuncTask3<decltype(d.begin()), std::chrono::duration<double>>, first, last, std::ref(procTime[i])));
//        first = last;
//    }

//    if (first != d.end())
//    {
//        std::chrono::duration<double> time;
//        ThreadFuncTask3<decltype(d.begin()), std::chrono::duration<double>>(first, d.end(), std::ref(time));
//        procTime[numTreads-1] = time;
//    }

//    for (auto& tr : trV)
//    {
//        tr.join();
//    }

//    for (auto time : procTime)
//    {
//        std::cout << "Time is " << time.count() << " s" << std::endl;
//    }

//    std::chrono::duration<double> wholeProcTime = std::accumulate(procTime.begin(), procTime.end(), std::chrono::duration<double>());
//    std::cout << "Whole time of processing of deque is " << wholeProcTime.count() << " s" << std::endl;

    // TASK 4:
    //==================================================================================

//    threadsafe_stack<int> MyStack;
//    std::vector<std::thread> trV;
//
//    MyStack.push(10);
//
//    for (int i=0; i < 2; ++i)
//    {
//        trV.emplace_back(std::thread(ThreadPushTask4<decltype(MyStack), int>, std::ref(MyStack), 300));
//    }
//
//    trV.emplace_back(std::thread(ThreadPopTask4<decltype(MyStack), int>, std::ref(MyStack), 145));
//
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

