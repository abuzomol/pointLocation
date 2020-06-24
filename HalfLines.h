//
// Created by Muzamil on 6/24/20.
//

#ifndef POINTLOCATION_HALFLINES_H
#define POINTLOCATION_HALFLINES_H

#include <vector>
#include "Point.h"
#include "Node.h"

class HalfLines {
    std::vector<Node> tree;
public:
    HalfLines(std::vector<LineSegment> & lineSegments);
    LineSegment query(Point& point);
};


#endif //POINTLOCATION_HALFLINES_H
