//
// Created by Muzamil on 6/23/20.
//

#ifndef POINTLOCATION_LINESEGMENT_H
#define POINTLOCATION_LINESEGMENT_H

/*LineSegment is a class storing horizontal lineSegments with xLeft, xRight, and y = yLeft.
*/
#include <ostream>

class LineSegment {
private:
    double xLeft, xRight, yLeft;
public:
    LineSegment(double xLeft, double xRight, double yLeft);

    double getXLeft() const;

    void setXLeft(double xLeft);

    double getXRight() const;

    void setXRight(double xRight);

    double getYLeft() const;

    void setYLeft(double yLeft);

    friend std::ostream& operator<<(std::ostream& os,
                                    const LineSegment& lineSegment);
};

std::ostream& operator<<(std::ostream& os, const LineSegment& lineSegment);

struct YLeftLessThan
{
    bool operator()(const LineSegment& a, const LineSegment& b)
    {
        return a.getYLeft() < b.getYLeft();
    }
};

#endif //POINTLOCATION_LINESEGMENT_H
