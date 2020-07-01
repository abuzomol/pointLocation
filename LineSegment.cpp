//
// Created by Muzamil on 6/23/20.
//

#include "LineSegment.h"

LineSegment::LineSegment(double xLeft, double xRight, double yLeft) : xLeft(xLeft), xRight(xRight), yLeft(yLeft){}

double LineSegment::getXLeft() const {
    return xLeft;
}

void LineSegment::setXLeft(double xLeft) {
    LineSegment::xLeft = xLeft;
}

double LineSegment::getXRight() const {
    return xRight;
}

void LineSegment::setXRight(double xRight) {
    LineSegment::xRight = xRight;
}

double LineSegment::getYLeft() const {
    return yLeft;
}

void LineSegment::setYLeft(double yLeft) {
    LineSegment::yLeft = yLeft;
}

bool operator==(const LineSegment &lineSegment1, const LineSegment &lineSegment2) {
    return lineSegment1.getXLeft() == lineSegment2.getXLeft() && lineSegment1.getXRight() == lineSegment2.getXRight() &&
           lineSegment1.getYLeft() == lineSegment2.getYLeft();
}

std::ostream &operator<<(std::ostream &os, const LineSegment &lineSegment) {
    os << '(' << lineSegment.getXLeft() << ',' << lineSegment.getXRight() << ',' << lineSegment.getYLeft()
       << ')';
    return os;
}