#pragma once
#include <iostream>

class IDrawPrimitive
{
public:
    virtual ~IDrawPrimitive() = 0;

    virtual void draw() const = 0;
};
IDrawPrimitive::~IDrawPrimitive() {}

class Point : public IDrawPrimitive
{
public:
    Point(int x, int y)
        : m_x{ x }, m_y{ y }
    { }
    ~Point() = default;

    void draw() const override
    {
        std::cout << "Point::draw: " << "x= " << m_x << " y=" << m_y << std::endl;
    }

    int getX() const { return m_x; }
    int getY() const { return m_y; }
private:
    int m_x;
    int m_y;
};

class Line : public IDrawPrimitive
{
public:
    Line(const Point& p1, const Point& p2)
        : m_p1{ p1 }, m_p2{ p2 }
    { }
    ~Line() = default;

    void draw() const override
    {
        std::cout << "Line: P1.x=" << getP1().getX()
            << " P1.y=" << getP1().getY()
            << " P2.x=" << getP2().getX()
            << " P2.y=" << getP2().getY()
            << std::endl;
    }

    Point getP1() const { return m_p1; }
    Point getP2() const { return m_p2; }
private:
    Point m_p1;
    Point m_p2;
};

class Box : public IDrawPrimitive
{
public:
    Box(const Point& topLeft, const Point& bottomRight)
        : m_topLeft{ topLeft },
        m_bottomRight{ bottomRight }
    { }
    ~Box() = default;

    void draw() const override
    {
        std::cout << "Box: topLeft.x=" << getTopLeft().getX()
            << " topLeft.y=" << getTopLeft().getY()
            << " bottomRight.x=" << getBottomRight().getX()
            << " bottomRight.y=" << getBottomRight().getY()
            << std::endl;
    }

    Point getTopLeft() const { return m_topLeft; }
    Point getBottomRight() const { return m_bottomRight; }
private:
    Point m_topLeft;
    Point m_bottomRight;
};

class Circle : public IDrawPrimitive
{
public:
    Circle(const Point& center, int radius)
        : m_center{ center },
        m_radius{ radius }
    { }
    ~Circle() = default;

    void draw() const override
    {
        std::cout << "Circle: centerTopLeft.x=" << getCenter().getX()
            << " centerTopLeft.y=" << getCenter().getY()
            << " radius=" << m_radius << std::endl;
    }

    Point getCenter() const { return m_center; };
    int getRadius() const { return m_radius; }
private:
    Point m_center;
    int m_radius;
};