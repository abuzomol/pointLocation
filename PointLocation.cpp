//
// Created by Muzamil on 6/23/20.
//

#include <iostream>
#include <cmath>
#include "constants.h"
#include "PointLocation.h"

void constructLeaves(std::vector<SuperNode>& tree, std::vector<LineSegment>& lineSegments, long long superHeight)
{
    for(LineSegment& line: lineSegments)
    {

    }
}

PointLocation::PointLocation(std::vector<LineSegment>& lineSegments)
{
    unsigned long long numOfXValues = 2 * lineSegments.size();
    unsigned long long numOfLeaves = numOfXValues  % VAL_SIZE == 0
                               ? numOfXValues / VAL_SIZE
                               : numOfXValues / VAL_SIZE + 1;
    unsigned int superHeight =
            ceil(log(numOfLeaves) / log(CHILD_SIZE)) + 1;

    // reserve the tree size : B^h - 1  + numOfLeaves
    this->superTree.reserve(pow(CHILD_SIZE,superHeight-1) -1 + numOfLeaves );
    constructLeaves(this->superTree, lineSegments, superHeight);

}

LineSegment& PointLocation::queryPointLocation(Point& point)
{

}