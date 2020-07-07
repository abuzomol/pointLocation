//
// Created by Muzamil on 6/24/20.
//

#include "SuperNode.h"

SuperNode::SuperNode(const std::vector<double> &val, unsigned long long index) : val(std::move(val)), index(index) {}

const std::vector<double> &SuperNode::getVal() const {
    return val;
}

void SuperNode::setVal(const std::vector<double> &val) {
    SuperNode::val = val;
}

const std::vector<std::vector<LineSegment> > &SuperNode::getLeftHalfLines() const {
    return leftHalfLines;
}

void SuperNode::setLeftHalfLines(const std::vector<std::vector<LineSegment> > &leftHalfLines) {
    SuperNode::leftHalfLines = leftHalfLines;
}

const std::vector<std::vector<LineSegment> > &SuperNode::getRightHalfLines() const {
    return rightHalfLines;
}

void SuperNode::setRightHalfLines(const std::vector<std::vector<LineSegment> > &rightHalfLines) {
    SuperNode::rightHalfLines = rightHalfLines;
}

const std::vector<LineSegment> &SuperNode::getMiddle() const {
    return middle;
}

void SuperNode::setMiddle(const std::vector<LineSegment> &middle) {
    SuperNode::middle = middle;
}

unsigned long long int SuperNode::getIndex() const {
    return index;
}

void SuperNode::setIndex(unsigned long long int index) {
    SuperNode::index = index;
}

double SuperNode::getIthVal(unsigned long long i) { return val[i]; }

