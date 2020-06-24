//
// Created by Muzamil on 6/24/20.
//

#ifndef POINTLOCATION_NODE_H
#define POINTLOCATION_NODE_H

#include <vector>
#include "LineSegment.h"

class Node {
private:
    std::vector<LineSegment> val;
    std::vector<double> minMaxX;
};


#endif //POINTLOCATION_NODE_H
