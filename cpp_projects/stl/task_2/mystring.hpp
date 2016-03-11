#ifndef MYSTRING_HPP
#define MYSTRING_HPP

namespace MySpace
{

class MyString
{
    char* pStr;

public:
    MyString(char const* = "default");
    MyString(MyString const& );
    MyString(MyString&& );

    ~MyString() { delete [] pStr; }

    MyString& operator=(MyString const& );
    MyString& operator=(MyString&& );
    char operator[](int i) { return pStr[i]; }

    char const* Get() const { return pStr; }
};

}

#endif // MYSTRING_HPP
