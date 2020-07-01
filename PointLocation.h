//
// Created by Muzamil on 6/23/20.
//

#ifndef POINTLOCATION_POINTLOCATION_H
#define POINTLOCATION_POINTLOCATION_H

#include <vector>
#include "LineSegment.h"
#include "SuperNode.h"
#include "Point.h"

//This class preprocess a list of (horizontal) lineSegments and answers point location queries.
class PointLocation {
private:
    std::vector<SuperNode> superTree;

    //construct last level of superTree based on sorted xValues of lineSegments.
    void constructLeaves(std::vector<SuperNode> &tree, std::vector<LineSegment> &lineSegments, unsigned long long superHeight,
                         unsigned long long numOfLeaves);

    //construct internal nodes of a superTree bottom up. Leaves should have been already constructed using constructLeaves function
    void constructInternalNodes(std::vector<SuperNode> &tree, unsigned long long superHeight, unsigned long long noOfLastLevelLeaves);

    //fill SuperTree with lineSegments
    void fillSuperTREE(std::vector<LineSegment>& lineSegments);

public:
    //constructor that preprocess a list of lineSegments for point location queries
    PointLocation(std::vector<LineSegment> &lineSegments);

    //a function that answers a single point query
    LineSegment &queryPointLocation(Point &point);

    // an abstract function that returns parent of node
    SuperNode &getParent(SuperNode &node);

    //returns i-th child of node if exists, otherwise returns nullptr
    SuperNode &getIthChild(SuperNode &node, unsigned long long i);


};

#endif //POINTLOCATION_POINTLOCATION_H
