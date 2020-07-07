//
// Created by Muzamil on 6/24/20.
//

#ifndef POINTLOCATION_HALFLINES_H
#define POINTLOCATION_HALFLINES_H

#include <vector>
#include "Point.h"
#include "Node.h"


typedef const double &funcType(const double &, const double &);

class HalfLines {
    std::vector<LineSegment> tree;
public:
    HalfLines(std::vector<LineSegment> &lineSegments, funcType);

    LineSegment query(Point &point);
};


#endif //POINTLOCATION_HALFLINES_H
