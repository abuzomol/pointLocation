//
// Created by Muzamil on 6/24/20.
//

#include "HalfLines.h"

HalfLines::HalfLines(std::vector<LineSegment> &lineSegments, funcType) : tree(std::move(lineSegments)) {
}