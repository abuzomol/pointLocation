//
// Created by Muzamil on 6/23/20.
//

#ifndef POINTLOCATION_POINT_H
#define POINTLOCATION_POINT_H

#include <ostream>

class Point {
private:
    double x, y;
public:
    Point(double x, double y) : x(x), y(y) {}

    double getY() const;

    void setY(double y);

    double getX() const;

    void setX(double x);

    friend std::ostream& operator<<(std::ostream& os, const Point& point);
};


std::ostream& operator<<(std::ostream& os, const Point& point);

struct XLessThan
{
    bool operator()(const Point& a, const Point& b)
    {
        return a.getX() < b.getX();
    }
};

#endif //POINTLOCATION_POINT_H
