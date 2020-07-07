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
    unsigned long long numOfLastLevelLeaves;
    //construct last level of superTree based on sorted xValues of lineSegments.
    void constructLeaves(std::vector<SuperNode> &tree, std::vector<LineSegment> &lineSegments);

    //construct internal nodes of a superTree bottom up. Leaves should have been already constructed using constructLeaves function
    void constructInternalNodes(std::vector<SuperNode> &tree, unsigned long long height);
    //partition the list of lineSegments into left to a boundary, right to a boundary, crossing one or more slab, and not crossing any boundary
    void partitionLineSegments(const std::vector<double>& boundaries,
                               const std::vector<LineSegment>& lineSegments,
                               std::vector<std::vector<LineSegment> >& left,
                               std::vector<std::vector<LineSegment> >& right,
                               std::vector<LineSegment>& middle,
                               std::vector<std::vector<LineSegment> >& remainingLineSegments);
    //fill SuperTree with lineSegments
    void fillNode(SuperNode& node, std::vector<LineSegment>& lineSegments);
    void fillSuperTREE(std::vector<LineSegment>& lineSegments);

public:
    //constructor that preprocess a list of lineSegments for point location queries
    PointLocation(std::vector<LineSegment> &lineSegments);

    //a function that answers a single point query
    LineSegment queryPointLocation(Point &point);

    // an abstract function that returns parent of node
    SuperNode &getParent(SuperNode &node);

    //returns i-th child of node if exists, otherwise returns nullptr
    SuperNode &getIthChild(SuperNode &node, unsigned long long i);

    bool isLastLevelLeaf(SuperNode& node);

    SuperNode& getRoot();
};

#endif //POINTLOCATION_POINTLOCATION_H
