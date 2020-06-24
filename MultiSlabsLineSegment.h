//
// Created by Muzamil on 6/24/20.
//

#ifndef POINTLOCATION_MULTISLABSLINESEGMENT_H
#define POINTLOCATION_MULTISLABSLINESEGMENT_H

#include <vector>
#include "Point.h"
#include "MiddleNode.h"

/* A class that preprocess lineSegments that crosses multislabs determined by elements of the vector xValues, and answer point location queries
 *
 *
 */

class MultiSlabsLineSegment {
private:
    std::vector<MiddleNode> middleTree;
public:
    MultiSlabsLineSegment(std::vector<LineSegment>& lineSegments, std::vector<double> &xValues);
    LineSegment& queryPointLocation(Point& point);
};


#endif //POINTLOCATION_MULTISLABSLINESEGMENT_H
