//
// Created by Muzamil on 6/24/20.
//

#ifndef POINTLOCATION_SUPERNODE_H
#define POINTLOCATION_SUPERNODE_H

#include <vector>
#include "HalfLines.h"
#include "MultiSlabsLineSegment.h"

/*A superNode is a class that stores xValues of lineSegment, and build a structure to answer point location for letHalfLines, rightHalfLines, and multi-slabs lineSegments.
 */
class SuperNode {
private:
    std::vector<double> val;
    std::vector<HalfLines> leftHalfLines;
    std::vector<HalfLines> rightHalfLines;
    MultiSlabsLineSegment* middle;
    long long index; // index tells the index of this node within the vector that implement a tree structure.
public:
    SuperNode(const std::vector<double> &val, const long long index);
};


#endif //POINTLOCATION_SUPERNODE_H
