#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>


enum class COLOR {RED, GREEN, BLUE, LAST};
enum class FORM {BASE, RECT, CIRCLE};

class Shape
{
protected:
    COLOR m_color;
    FORM m_form;

public:
    Shape(COLOR c = COLOR::LAST, FORM f = FORM::BASE) : m_color(c), m_form(f) {}
    static Shape* ShapeMaker();
    friend std::ostream& operator<<(std::ostream&, Shape const&);
};

class Rect : public Shape
{
    int x1, y1, x2, y2;

public:
    Rect(COLOR c, int _x1, int _y1, int _x2, int _y2) : Shape(c, FORM::RECT), x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
};

class Circle : public Shape
{
    int x, y, r;

public:
    Circle(COLOR c, int _x, int _y, int _r) : Shape(c, FORM::CIRCLE), x(_x), y(_y), r(_r) {}
};

Shape* Shape::ShapeMaker()
{
    //Сгенерировать случайным образом:
    COLOR c = static_cast<COLOR>((rand()%static_cast<int>(COLOR::LAST)));
    switch(rand()&1)
    {
        case 0:
        {
            //Сгенерировать случайным образом
            int x1 = rand()%10, y1 = rand()%10, x2 = x1+rand()%50, y2 = y1+rand()%20;

            return new Rect(c, x1, y1, x2, y2);
        }
        case 1:
        {
            //Сгенерировать случайным образом
            int x = rand()%10, y = rand()%10, r = rand()%40;

            return new Circle(c, x, y, r);
        }
        default: break;
    }
    return new Shape(COLOR::LAST);
}

std::ostream& operator<<(std::ostream& out, Shape const& s)
{
    switch(s.m_form)
    {
         case FORM::BASE    : out << "base";    break;
         case FORM::RECT    : out << "rect";  break;
         case FORM::CIRCLE  : out << "circle";   break;
         default : out.setstate(std::ios_base::failbit);
    }
    out << ", ";
    switch(s.m_color)
    {
         case COLOR::RED   : out << "red";    break;
         case COLOR::GREEN : out << "green";  break;
         case COLOR::BLUE  : out << "blue";   break;
         default : out.setstate(std::ios_base::failbit);
    }

    out << std::endl;

    return out;
}


#endif // SHAPE_HPP
