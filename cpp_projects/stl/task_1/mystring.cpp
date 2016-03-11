#include <string.h>
#include "mystring.hpp"


MyString::MyString(char const* p)
{
    pStr = new char[strlen(p) + 1];
    strcpy(pStr, p);
}

MyString::MyString(MyString const& lv)
{
    pStr = new char[strlen(lv.pStr) + 1];
    strcpy(pStr, lv.pStr);
}

MyString::MyString(MyString&& rv)
{
    pStr = rv.pStr;
    rv.pStr = nullptr;
}

MyString& MyString::operator=(const MyString& lv)
{
    if(this != &lv)
    {
        delete[] pStr;
        pStr = new char[strlen(lv.pStr) + 1];
        strcpy(pStr, lv.pStr);
    }

    return *this;
}

MyString& MyString::operator=(MyString&& rv)
{
    if(this != &rv)
    {
        delete [] pStr;
        pStr = rv.pStr;
        rv.pStr = nullptr;
    }

    return *this;
}
