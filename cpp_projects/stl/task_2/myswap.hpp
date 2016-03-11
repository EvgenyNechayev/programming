#ifndef SWAP_HPP
#define SWAP_HPP

#include <utility>

namespace MySpace
{

template<typename T>
void MySwap(T& a, T& b)
{
    T t(std::move(a));

    a = std::move(b);
    b = std::move(t);
}

}

#endif // SWAP_HPP
