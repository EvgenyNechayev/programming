#ifndef MYPOINT_HPP
#define MYPOINT_HPP

#include <cmath>
namespace MySpace
{

class MyPoint
{
    int x;
    int y;

public:
    MyPoint() : x(0), y(0) {}
    MyPoint(int _x, int _y) : x(_x), y(_y) {}

    inline int GetX() const { return x; }
    inline int GetY() const { return y; }

    inline bool operator==(MyPoint const& lv) { return (abs(x) == abs(lv.x) && abs(lv.y) == abs(lv.y)); }
    inline bool operator!=(MyPoint const& lv) { return !(operator==(lv)); }
    inline bool operator<(MyPoint const& lv) { return (abs(x) < abs(lv.x) && abs(lv.y) < abs(lv.y)); }
    inline bool operator>(MyPoint const& lv) { return !(operator<(lv)); }
    inline bool operator<=(MyPoint const& lv) { return (abs(x) <= abs(lv.x) && abs(lv.y) <= abs(lv.y)); }
    inline bool operator>=(MyPoint const& lv) { return (abs(x) >= abs(lv.x) && abs(lv.y) >= abs(lv.y)); }
};

}

#endif // MYPOINT_HPP
