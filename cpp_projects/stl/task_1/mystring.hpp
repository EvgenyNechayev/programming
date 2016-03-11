#ifndef MYSTRING_HPP
#define MYSTRING_HPP

class MyString
{
    char* pStr;

public:
    MyString(char const* = "default");
    MyString(MyString const& );
    MyString(MyString&& );

    ~MyString() { delete [] pStr; }

    MyString& operator=(const MyString& );
    MyString& operator=(MyString&& );

    const char* Get() const { return pStr; }
};

#endif // MYSTRING_HPP
