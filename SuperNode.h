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
    std::vector<std::vector<LineSegment> > leftHalfLines;
    std::vector<std::vector<LineSegment> > rightHalfLines;
    std::vector<LineSegment> middle;
    unsigned long long index; // index tells the index of this node within the vector that implement a tree structure.
public:
    SuperNode(const std::vector<double> &val, unsigned long long index);

    const std::vector<double> &getVal() const;

    void setVal(const std::vector<double> &val);

    const std::vector<std::vector<LineSegment> > &getLeftHalfLines() const;

    void setLeftHalfLines(const std::vector<std::vector<LineSegment> > &leftHalfLines);

    const std::vector<std::vector<LineSegment> > &getRightHalfLines() const;

    void setRightHalfLines(const std::vector<std::vector<LineSegment> > &rightHalfLines);

    const std::vector<LineSegment> &getMiddle() const;

    void setMiddle(const std::vector<LineSegment> &middle);

    unsigned long long int getIndex() const;

    void setIndex(unsigned long long int index);

    double getIthVal(unsigned long long i);
};


#endif //POINTLOCATION_SUPERNODE_H
