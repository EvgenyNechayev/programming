#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

#include <stdexcept>
#include <string.h>

#include "mycommon.hpp"

namespace MySpace
{

template<typename T>
class MyVector
{
public:
    typedef T                   value_type;
    typedef value_type*         iterator;
    typedef value_type const*   const_iterator;
    typedef value_type&         reference;
    typedef value_type const&   const_reference;

    typedef MySpace::reverse_iterator<iterator>          reverse_iterator;
    typedef MySpace::reverse_iterator<const_iterator>    const_reverse_iterator;

private:
    int         mySize;
    int         myCapacity;
    value_type* pBuffer;

public:
    inline MyVector() : mySize(0), myCapacity(0), pBuffer(nullptr) {}
    inline explicit MyVector(int );
    inline MyVector(int , const_reference );
    inline MyVector(const_iterator , const_iterator );
    inline MyVector(const_reverse_iterator , const_reverse_iterator );
    inline MyVector(MyVector<T> const& );
    inline MyVector(MyVector<T>&& );
    inline ~MyVector() { delete [] pBuffer; }

    inline int capacity() const;
    inline int size() const;
    inline int max_size() const;
    inline bool empty() const;

    inline iterator begin() const;
    inline iterator end() const;
    inline const_iterator cbegin() const;
    inline const_iterator cend() const;

    inline reverse_iterator rbegin() const;
    inline reverse_iterator rend() const;
    inline const_reverse_iterator crbegin() const;
    inline const_reverse_iterator crend() const;

    inline reference front();
    inline const_reference front() const;
    inline reference back();
    inline const_reference back() const;
    inline void push_back(const_reference );
    inline void push_back(value_type&& );
    inline void pop_back();
    inline void insert(iterator , const_iterator );
    inline void insert(iterator , value_type&& );
    inline void insert(int , value_type&& );

    inline iterator erase(iterator , iterator );
    inline iterator erase(iterator );

    inline void reserve(int );
    inline void resize(int );
    inline void shrink_to_fit() { myCapacity = mySize; }

    inline reference operator[](int );
    inline const_reference operator[](int ) const;
    inline reference at(int ) throw(std::range_error);
    inline const_reference at(int ) const throw(std::range_error);

    inline MyVector& operator=(MyVector const& );
    inline MyVector& operator=(MyVector&& );
};

template<typename T>
inline MyVector<T>::MyVector(int size)
{
    myCapacity = size;
    mySize = size;
    pBuffer = new T[size];
}

template<typename T>
inline MyVector<T>::MyVector(int size, MyVector<T>::const_reference init)
{
    mySize = size;
    myCapacity = size;
    pBuffer = new T[size];

    for (int i = 0; i < size; i++)
        pBuffer[i] = init;
}

template<typename T>
inline MyVector<T>::MyVector(MyVector<T>::const_iterator first, MyVector<T>::const_iterator last)
{
    mySize = last - first;
    myCapacity = mySize;
    pBuffer = new T[mySize];

    for (int i = 0; first != last; ++i, ++first)
        pBuffer[i] = *first;
}

template<typename T>
inline MyVector<T>::MyVector(const_reverse_iterator first, const_reverse_iterator last)
{
    mySize = last - first;
    myCapacity = mySize;

    pBuffer = new T[mySize];

    for (int i = 0; first != last; ++i, ++first)
        pBuffer[i] = *first;
}

template<typename T>
inline MyVector<T>::MyVector(MyVector<T> const& lv)
{
    mySize = lv.mySize;
    myCapacity = lv.myCapacity;
    pBuffer = new T[mySize];

    memcpy(pBuffer, lv.pBuffer, mySize * sizeof(T));
}

template<typename T>
inline MyVector<T>::MyVector(MyVector<T>&& rv)
{
    mySize = rv.mySize;
    myCapacity = rv.myCapacity;
    pBuffer = rv.pBuffer;
    rv.pBuffer = nullptr;
}

template<typename T>
inline int MyVector<T>::capacity()const
{
    return myCapacity;
}

template<typename T>
inline int MyVector<T>::size() const
{
    return mySize;
}

template<typename T>
inline int MyVector<T>::max_size() const
{
    return INT_MAX / sizeof(MyVector<T>);
}

template<typename T>
inline bool MyVector<T>::empty() const
{
    return 0 == mySize;
}

template<typename T>
inline MyVector<T>& MyVector<T>::operator=(MyVector<T> const& lv)
{
    if(this != &lv)
    {
        delete [] pBuffer;
        mySize = lv.mySize;
        myCapacity = lv.myCapacity;
        pBuffer = new T[mySize];

        memcpy(pBuffer, lv.pBuffer, mySize * sizeof(T));
    }

    return *this;
}

template<typename T>
inline MyVector<T>& MyVector<T>::operator=(MyVector<T>&& rv)
{
    if(this != &rv)
    {
        mySize = rv.mySize;
        myCapacity = rv.myCapacity;
        pBuffer = rv.pBuffer;
        rv.pBuffer = nullptr;
    }

    return *this;
}

template<typename T>
inline typename MyVector<T>::iterator MyVector<T>::begin() const
{
    return pBuffer;
}

template<typename T>
inline typename MyVector<T>::iterator MyVector<T>::end() const
{
    return pBuffer + size();
}

template<typename T>
inline typename MyVector<T>::const_iterator MyVector<T>::cbegin() const
{
    return pBuffer;
}

template<typename T>
inline typename MyVector<T>::const_iterator MyVector<T>::cend() const
{
    return pBuffer + size();
}

template<typename T>
inline typename MyVector<T>::reverse_iterator MyVector<T>::rbegin() const
{
    return (reverse_iterator(end()));
}

template<typename T>
inline typename MyVector<T>::reverse_iterator MyVector<T>::rend() const
{
    return (reverse_iterator(begin()));
}

template<typename T>
inline typename MyVector<T>::const_reverse_iterator MyVector<T>::crbegin() const
{
    return (const_reverse_iterator(end()));
}

template<typename T>
inline typename MyVector<T>::const_reverse_iterator MyVector<T>::crend() const
{
    return (const_reverse_iterator(begin()));
}

template<typename T>
inline typename MyVector<T>::reference MyVector<T>::front()
{
    return pBuffer[0];
}

template<typename T>
inline typename MyVector<T>::const_reference MyVector<T>::front() const
{
    return pBuffer[0];
}

template<typename T>
inline typename MyVector<T>::reference MyVector<T>::back()
{
    return pBuffer[size - 1];
}

template<typename T>
inline typename MyVector<T>::const_reference MyVector<T>::back() const
{
    return pBuffer[size - 1];
}

template<typename T>
inline void MyVector<T>::push_back(MyVector<T>::const_reference lv)
{
    if (mySize >= myCapacity)
        reserve(myCapacity + 5);
    pBuffer[mySize++] = lv;
}

template<typename T>
inline void MyVector<T>::push_back(value_type&& rv)
{
    if (mySize >= myCapacity)
        reserve(myCapacity + 5);
    pBuffer[mySize++] = rv;
}

template<typename T>
inline void MyVector<T>::pop_back()
{
    --mySize;
}

template<typename T>
inline void MyVector<T>::insert(MyVector<T>::iterator pos, MyVector<T>::const_iterator lv)
{
    int align = pos - begin();

    if (mySize >= myCapacity)
        reserve(myCapacity + 5);

    pos = begin() + align;

    for (iterator it = end(); it != pos; --it)
        *it = *(it - 1);

    *pos = *lv;
    ++mySize;
}

template<typename T>
inline void MyVector<T>::insert(MyVector<T>::iterator pos, value_type&& rv)
{
    int align = pos - begin();

    if (mySize >= myCapacity)
        reserve(myCapacity + 5);

    pos = begin() + align;

    for (iterator it = end(); it != pos; --it)
        *it = *(it - 1);

    *pos = rv;

    ++mySize;
}

template<typename T>
inline void MyVector<T>::insert(int pos, value_type&& rv)
{
    if (mySize >= myCapacity)
        reserve(myCapacity + 5);

    for (int i = size() - 1; i > pos; --i)
        pBuffer[i] = pBuffer[i-1];

    pBuffer[pos] = rv;

    ++mySize;
}

template<typename T>
inline typename MyVector<T>::iterator MyVector<T>::erase(MyVector<T>::iterator first, MyVector<T>::iterator last)
{
    iterator it1 = first, it2 = last + 1;
    while (it2 < end())
    {
        *it1 = *it2;
        ++it1;
        ++it2;
    }

    mySize -= last - first;

    return it2;
}

template<typename T>
inline typename MyVector<T>::iterator MyVector<T>::erase(MyVector<T>::iterator pos)
{
    iterator it = pos;
    while (it < end())
    {
        *it = *(it + 1);
        ++it;
    }

    --mySize;

    return it;
}

template<typename T>
inline void MyVector<T>::reserve(int capacity)
{
    if (myCapacity >= capacity) return;

    T* newBuffer = new T[capacity];
    int newSize = capacity < mySize ? capacity : mySize;

    if (nullptr != pBuffer)
    {
        memcpy(newBuffer, pBuffer, newSize * sizeof(T));
    }
    else
    {
        memset(newBuffer, 0, newSize * sizeof(T));
    }

    delete [] pBuffer;
    pBuffer = newBuffer;
    myCapacity = capacity;
}

template<typename T>
inline void MyVector<T>::resize(int size)
{
    mySize = size;
    reserve(size);
}

template<typename T>
inline typename MyVector<T>::reference MyVector<T>::at(int index) throw(std::range_error)
{
    if (index >= 0 && index < size() - 1)
        return pBuffer[index];
    throw std::range_error("Bad range!!!");
}

template<typename T>
inline typename MyVector<T>::const_reference MyVector<T>::at(int index) const throw(std::range_error)
{
    if (index >= 0 && index < size() - 1)
        return pBuffer[index];
    throw std::range_error("Bad range!!!");
}

template<typename T>
inline typename MyVector<T>::reference MyVector<T>::operator[](int index)
{
    return pBuffer[index];
}

template<typename T>
inline typename MyVector<T>::const_reference MyVector<T>::operator[](int index) const
{
    return pBuffer[index];
}

template<typename T>
std::ostream& operator<<(std::ostream& out, MyVector<T> const& lv)
{
    typename MyVector<T>::const_iterator first = lv.cbegin();
    typename MyVector<T>::const_iterator last  = lv.cend();

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
std::ostream& operator<<(std::ostream& out, MyVector<MyString> const& lv)
{
    MyVector<MyString>::const_iterator first = lv.cbegin();
    MyVector<MyString>::const_iterator last  = lv.cend();

    while (first != last)
    {
        std::cout << first->Get() << " ";
        ++first;
    }
    out << std::endl;
    out << "==================================" << std::endl;

    return out;
}

template<>
std::ostream& operator<<(std::ostream& out, MyVector<MyPoint> const& lv)
{
    MyVector<MyPoint>::const_iterator first = lv.cbegin();
    MyVector<MyPoint>::const_iterator last  = lv.cend();

    while (first != last)
    {
        out << "x=" << first->GetX() << " y=" << first->GetY() << std::endl;
        ++first;
    }
    out << "==================================" << std::endl;

    return out;
}

template<>
std::ostream& operator<<(std::ostream& out, MyVector<MyPoint*> const& lv)
{
    MyVector<MyPoint*>::const_iterator first = lv.cbegin();
    MyVector<MyPoint*>::const_iterator last  = lv.cend();

    while (first != last)
    {
        out << "x=" << (*first)->GetX() << " y=" << (*first)->GetY() << std::endl;
        ++first;
    }
    out << "==================================" << std::endl;

    return out;
}

}

#endif // MYVECTOR_HPP
