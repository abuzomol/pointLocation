//
// Created by Muzamil on 6/23/20.
//

#ifndef POINTLOCATION_POINTLOCATION_H
#define POINTLOCATION_POINTLOCATION_H

#include <vector>
#include "LineSegment.h"
#include "SuperNode.h"
#include "Point.h"

/*This class takes preprocess a list of (horizontal) lineSegments and answer point location queries.
 */
class PointLocation {
private:
    std::vector<SuperNode> superTree;
public:
    //constructor that preprocess a list of lineSegments for point location queries
    PointLocation(std::vector<LineSegment>& lineSegments);
    LineSegment& queryPointLocation(Point& point);
};
#endif //POINTLOCATION_POINTLOCATION_H
