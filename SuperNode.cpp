//
// Created by Muzamil on 6/24/20.
//

#include "SuperNode.h"

SuperNode::SuperNode(const std::vector<double> &val, const long long index) : val(std::move(val)), index(index) {}

const std::vector<double> &SuperNode::getVal() const {
    return val;
}

void SuperNode::setVal(const std::vector<double> &val) {
    SuperNode::val = val;
}

const std::vector<HalfLines> &SuperNode::getLeftHalfLines() const {
    return leftHalfLines;
}

void SuperNode::setLeftHalfLines(const std::vector<HalfLines> &leftHalfLines) {
    SuperNode::leftHalfLines = leftHalfLines;
}

const std::vector<HalfLines> &SuperNode::getRightHalfLines() const {
    return rightHalfLines;
}

void SuperNode::setRightHalfLines(const std::vector<HalfLines> &rightHalfLines) {
    SuperNode::rightHalfLines = rightHalfLines;
}

MultiSlabsLineSegment *SuperNode::getMiddle() const {
    return middle;
}

void SuperNode::setMiddle(MultiSlabsLineSegment *middle) {
    SuperNode::middle = middle;
}

long long int SuperNode::getIndex() const {
    return index;
}

void SuperNode::setIndex(long long int index) {
    SuperNode::index = index;
}
