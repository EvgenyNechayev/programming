#ifndef SWAP_HPP
#define SWAP_HPP

#include <utility>
using namespace std;

template <typename T>
void Swap(T& a, T& b)
{
    T t(std::move(a));

    a = std::move(b);
    b = std::move(t);
}


#endif // SWAP_HPP
