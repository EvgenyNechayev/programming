#ifndef MYALGORITHMS_HPP
#define MYALGORITHMS_HPP

#include <iostream>
#include "myswap.hpp"
#include "myvector.hpp"
#include "mycommon.hpp"

namespace MySpace
{

template<typename T, typename F>
void MyFill(T first, T last, F& fill)
{
    while (first != last)
    {
        *first++ = fill;
    }
}

template<typename T>
T MyUnique(T first, T last)
{
    T res = first;

    if (first != last)
    {
        while (++first != last)
        {
            if (*first != *res)
            {
                *++res = *first;
            }
        }
    }

    return ++res;
}

template<typename T, typename F>
T MyPartition(T first, T last, F compare)
{
    typedef typename iterator_traits<T>::iterator iterator;

    iterator i = first - 1;
    iterator j = first;

    while (j <= last)
    {
        if ((*compare)(j, last))
        {
            ++i;
            MySwap(*j, *i);
        }
        ++j;
    }

    return i;
}

template<typename T, typename F>
void MySortStep(T first, T last, F compare)
{
    if (first >= last) return;

    T m = MyPartition(first, last, compare);

    MySortStep(first, m - 1, compare);
    MySortStep(m + 1, last, compare);
}

template <typename T, typename F>
void MyQuickSort(T first, T last, F compare)
{
    MySortStep(first, last - 1, compare);
}

template <typename T>
void MyReverse(T first, T last)
{
    --last;
    while (first < last)
    {
        MySwap(*first++, *last--);
    }
}

}

#endif // MYALGORITHMS_HPP









