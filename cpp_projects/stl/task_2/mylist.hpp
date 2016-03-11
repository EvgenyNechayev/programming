#ifndef MYLIST_HPP
#define MYLIST_HPP

#include <utility>
#include "myvector.hpp"
#include "myalgorithms.hpp"

using namespace MySpace;

namespace MySpace
{

template<typename T>
class MyList : public MyVector<T> {
public:
    typedef typename MyVector<T>::value_type        value_type;
    typedef typename MyVector<T>::iterator          iterator;
    typedef typename MyVector<T>::const_iterator    const_iterator;
    typedef typename MyVector<T>::reference         reference;
    typedef typename MyVector<T>::const_reference   const_reference;

    typedef MySpace::reverse_iterator<iterator>          reverse_iterator;
    typedef MySpace::reverse_iterator<const_iterator>    const_reverse_iterator;

public:
    inline			MyList() : MyVector<T>() {}
    inline explicit	MyList(int n) : MyVector<T>(n) {}
    inline			MyList(int n, const_reference v) : MyVector<T>(n, v) {}
    inline			MyList(const_iterator first, const_iterator last) : MyVector<T>(first, last) {}
    inline			MyList(const_reverse_iterator first, const_reverse_iterator last) : MyVector<T>(first, last) {}
    inline			MyList(MyList<T> const& lv) : MyVector<T>(lv) {}
    inline			MyList(MyList<T>&& rv) : MyVector<T>(rv) {}

    inline int              size() const                { return (MyVector<T>::size()); }
    inline iterator         begin()                     { return (MyVector<T>::begin()); }
    inline const_iterator	cbegin() const              { return (MyVector<T>::cbegin()); }
    inline iterator         end()                       { return (MyVector<T>::end()); }
    inline const_iterator	cend() const                { return (MyVector<T>::cend()); }
    inline void             push_back(const_iterator lv)    { MyVector<T>::insert(end(), lv); }
    inline void             push_back(value_type&& rv)      { MyVector<T>::insert(end(), std::forward<T>(rv)); }
    inline void             push_front(const_iterator lv)   { MyVector<T>::insert(begin(), lv); }
    inline void             push_front(value_type&& rv)     { MyVector<T>::insert(begin(), std::forward<T>(rv)); }
    inline void             pop_front()                 { MyVector<T>::erase(begin()); }

    inline void             insert(iterator pos, const_iterator lv) { MyVector<T>::insert(pos, lv); }
    inline void             insert(iterator pos, value_type&& rv)   { MyVector<T>::insert(pos, rv); }
    inline void             insert(int pos, value_type&& rv)        { MyVector<T>::insert(pos, rv); }

    inline const_reference	front() const               { return (*begin()); }
    inline reference		front()                     { return (*begin()); }

    inline void             remove(const_iterator predicate);
    inline void             remove(value_type&& rv);
    template<typename F>
    inline void             remove_if(F predicate);

    inline void             unique()                    { MyVector<T>::erase(MyUnique(begin(), end()), end()); }

    inline void             reverse()                   { MyReverse(begin(), end()); }
    template<typename F>
    inline void             sort(F compare)             { MyQuickSort(begin(), end(), compare); }
    void                    merge(MyList<T>& lv);
};

template <typename T>
template <typename F>
void MyList<T>::remove_if(F predicate)
{
    for (iterator it = begin(), ite = end(); it != ite; ++it)
    {
        if (predicate(*it))
        {
            MyVector<T>::erase(it);
            break;
        }
    }
}

template <typename T>
void MyList<T>::remove(const_iterator lv)
{
    remove(std::move(*lv));
}

template <typename T>
void MyList<T>::remove(value_type&& rv)
{
    for (iterator it = begin(), ite = end(); it != ite; ++it)
    {
        if (*it == rv)
        {
            MyVector<T>::erase(it);
            break;
        }
    }
}

template <typename T>
void MyList<T>::merge(MyList<T>& lv)
{
    iterator last1 = end() - 1;
    iterator last2 = lv.end() - 1;

    int cnt = 0;

    int align = end() - begin();
    while (last1 >= end() - align && last2 >= lv.begin())
    {

//        5 5 0 1 2 3 4 5
//                    .
//        0 1 2 3 4 5
//                .

        if (*last1 >= *last2)
        {
            T buf = *last1;
            push_front(std::move(buf));

            last1 = end() - 1 - ++cnt;
        }
        else
        {
            push_front(std::move(*last2));

            last1 = end() - 1 - cnt;
            --last2;
        }        
    }

    if (last2 >= lv.begin())
    {
        while (last2 >= lv.begin())
        {
            push_front(std::move(*last2--));
            last2--;
        }
    }
    else
    {
        while (last1 >= end() - align)
        {
            T buf = *last1;
            push_front(std::move(buf));

            last1 = end() - 1 - ++cnt;
        }
    }

    MyVector<T>::erase(last1 + 2, end());
}

}

#endif // MYLIST_HPP
