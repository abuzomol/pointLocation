//
// Created by Muzamil on 6/23/20.
//

#ifndef POINTLOCATION_POINTLOCATION_H
#define POINTLOCATION_POINTLOCATION_H

#include <vector>
#include "LineSegment.h"
#include "SuperNode.h"
#include "Point.h"

//This class takes preprocess a list of (horizontal) lineSegments and answers point location queries.
class PointLocation {
private:
    std::vector<SuperNode> superTree;

    void constructInternalNodes(std::vector<SuperNode> &tree, long long superHeight, long long noOfLastLevelLeaves);

    void constructLeaves(std::vector<SuperNode> &tree, std::vector<LineSegment> &lineSegments, long long superHeight,
                         long long numOfLeaves);

public:
    //constructor that preprocess a list of lineSegments for point location queries
    PointLocation(std::vector<LineSegment> &lineSegments);
    //a function that answers a single point query
    LineSegment &queryPointLocation(Point &point);
    // an abstract function that returns
    SuperNode &getParent(SuperNode &node);
    //returns i-th child of node if exists, and return nullptr if it doesn't.
    SuperNode &getIthChild(SuperNode& node, unsigned long long i);


};

#endif //POINTLOCATION_POINTLOCATION_H
