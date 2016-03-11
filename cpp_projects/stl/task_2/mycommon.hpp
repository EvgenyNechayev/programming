#ifndef MYCOMMON_HPP
#define MYCOMMON_HPP

#include <iostream>
#include <deque>
#include "myvector.hpp"
#include "mystring.hpp"
#include "mypoint.hpp"

namespace MySpace
{

template<typename T>
void DisplayParam(T const& lv)
{
    std::cout << "capacity=" << lv.capacity()
              << " size=" << lv.size()
              << " max_size=" << lv.max_size()
              << std::endl
              << "=================================="
              << std::endl;
}

template<typename T>
struct iterator_traits {};

template <typename T>
struct iterator_traits<T*>
{
    typedef T           value_type;
    typedef T*          iterator;
    typedef T const*    const_iterator;
    typedef T&          reference;
    typedef const T&	const_reference;
};

template<typename T>
class reverse_iterator
{
    T buf;

public:
    typedef typename iterator_traits<T>::value_type         value_type;
    typedef typename iterator_traits<T>::iterator           iterator;
    typedef typename iterator_traits<T>::const_iterator     const_iterator;
    typedef typename iterator_traits<T>::reference          reference;
    typedef typename iterator_traits<T>::const_reference	const_reference;

    reverse_iterator() : buf() {}
    explicit reverse_iterator(iterator lv) : buf(lv) {}

    inline bool operator==(reverse_iterator const& lv) const { return (buf == lv.buf); }
    inline bool operator!=(reverse_iterator const& lv) const { return (buf != lv.buf); }
    inline bool operator<(reverse_iterator const& lv) const { return (buf < lv.buf); }
    inline bool operator>(reverse_iterator const& lv) const { return !(operator<(lv)); }

    inline value_type   operator*() { return (*(buf-1)); }
    inline iterator     operator->() { return (&(operator*())); }

    inline reverse_iterator&	operator++() { --buf; return(*this); }
    inline reverse_iterator&	operator--() { ++buf; return(*this); }
    inline reverse_iterator     operator++(int) { reverse_iterator lv(*this); --buf; return lv; }
    inline reverse_iterator     operator--(int) { reverse_iterator lv(*this); ++buf; return lv; }

    inline reverse_iterator&	operator+=(int n) { buf -= n; return (*this); }
    inline reverse_iterator&	operator-=(int n) { buf += n; return (*this); }
    inline reverse_iterator     operator+(int n) const { return reverse_iterator(buf - n); }
    inline reverse_iterator     operator-(int n) const { return reverse_iterator(buf + n); }

    inline int operator-(reverse_iterator& lv) const { return (lv.buf - buf); }

    inline reference            operator[](int n) const { return (*(*this + n)); }
};


template<typename T>
std::ostream& operator<<(std::ostream& out, std::deque<T> const& lv)
{
    typename std::deque<T>::const_iterator first = lv.cbegin();
    typename std::deque<T>::const_iterator last  = lv.cend();

    while (first != last)
    {
        out << *first << " ";
        ++first;
    }
    out << std::endl;
    out << "==================================" << std::endl;

    return out;
}

template<>
std::ostream& operator<<(std::ostream& out, std::deque<MyPoint> const& lv)
{
    std::deque<MyPoint>::const_iterator first = lv.cbegin();
    std::deque<MyPoint>::const_iterator last  = lv.cend();

    while (first != last)
    {
        out << "x=" << first->GetX() << " y=" << first->GetY() << std::endl;
        ++first;
    }
    out << "==================================" << std::endl;

    return out;
}

template<>
std::ostream& operator<<(std::ostream& out, std::deque<MyString> const& lv)
{
    std::deque<MyString>::const_iterator first = lv.cbegin();
    std::deque<MyString>::const_iterator last  = lv.cend();

    while (first != last)
    {
        std::cout << first->Get() << " ";
        ++first;
    }
    out << std::endl;
    out << "==================================" << std::endl;

    return out;
}

}

#endif // MYCOMMON_HPP
