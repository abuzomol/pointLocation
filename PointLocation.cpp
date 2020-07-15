//
// Created by Muzamil on 6/23/20.
//

#include <iostream>
#include <cmath>
#include <chrono>
#include <algorithm>
#include "constants.h"
#include "PointLocation.h"


#define all(c) (c).begin(),(c).end()
#define tr(c, i) for(decltype((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define pb push_back
#define mp make_pair

using namespace std;

//constructor that preprocess a list of lineSegments for point location queries.
//The constructor call three functions: constructLeaves, constructInternalNodes and fillSuperTree
PointLocation::PointLocation(std::vector<LineSegment> &lineSegments) {
    // find the number of last level leaves, and then the height of the tree
    unsigned long long numOfXValues = 2 * lineSegments.size();
    this->numOfLastLevelLeaves = numOfXValues % VAL_SIZE == 0
                                 ? numOfXValues / VAL_SIZE
                                 : numOfXValues / VAL_SIZE + 1;
    unsigned long long height =
            ceil(log(numOfLastLevelLeaves) / log(CHILD_SIZE));

    cout << "tree height: " << height << endl;
    // reserve the tree size : (B^h - 1) / (B-1)  + numOfLastLevelLeaves
    auto start = std::chrono::high_resolution_clock::now();
    unsigned long long numOfInternalNodes = (pow(CHILD_SIZE, height) - 1) / (CHILD_SIZE - 1);
    this->superTree.reserve(numOfInternalNodes + numOfLastLevelLeaves);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "time to  allocate " << this->superTree.capacity() << " superNodes for tree: " << elapsed.count()
              << endl;

    start = std::chrono::high_resolution_clock::now();
    constructLeaves(this->superTree, lineSegments);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "time to construct " << numOfLastLevelLeaves << " leaves: " << elapsed.count() << endl;

    start = std::chrono::high_resolution_clock::now();
    constructInternalNodes(this->superTree, height);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cout << "time to construct " << numOfInternalNodes << " internal nodes: " << elapsed.count() << endl;

    start = std::chrono::high_resolution_clock::now();
    cout << "filling SuperTree................. " << endl;
    fillSuperTREE(lineSegments);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cout << "time to fill the tree: " << elapsed.count();
}

//place the leaves at the beginning of the vector tree
void PointLocation::constructLeaves(std::vector<SuperNode> &tree, std::vector<LineSegment> &lineSegments) {
    //get xValues first. Note, that we can speed up this construction, if the values are stored in a vector separate from rest of superNode members.
    //It will speed up processes like sorting xValues, but we loose locality here.

    //allocate memory first. For performance, reserve is used instead of resize()
    std::vector<double> xValues;
    xValues.reserve(lineSegments.size() * 2);
    //get left and right x-values of lineSegments
    for (LineSegment &lineSegment: lineSegments) {
        xValues.emplace_back(lineSegment.getXLeft());
        xValues.emplace_back(lineSegment.getXRight());
    }

    auto start = std::chrono::high_resolution_clock::now();
    sort(xValues.begin(), xValues.end());
    auto finish = std::chrono::high_resolution_clock::now();


    //remove duplicates from xValues
    start = std::chrono::high_resolution_clock::now();
    vector<double>::iterator uniqueIterator;
    uniqueIterator = std::unique(xValues.begin(), xValues.end());
    xValues.resize(std::distance(xValues.begin(),uniqueIterator));
    finish = std::chrono::high_resolution_clock::now();

//    for(int i = 0; i < xValues.size(); i++)
//        cout << xValues[i] << " " ;
//    cout << endl;
//
//    tr(xValues, it)
//        cout << *it << " ";
//    cout << endl;
    //add xValues to leaves, and store them at the beginning of array.
    //fastest observed performance, is when we reserve a vector inside the for loop, fill it, and then emplace it back to the tree with index i

    for (unsigned long long i = 0; i < numOfLastLevelLeaves - 1; i++) {
        std::vector<double> values;
        values.reserve(VAL_SIZE);
        for (unsigned long long j = 0; j < VAL_SIZE; j++) {
            values.emplace_back(xValues[i * VAL_SIZE + j]);
        }
        //construct the superNode and add it to the tree
        tree.emplace_back(values, tree.size());
    }
    //add the last leaf
    std::vector<double> values;
    values.reserve(VAL_SIZE);
    unsigned long long t;

    for (t = 0; (numOfLastLevelLeaves - 1) * VAL_SIZE + t < xValues.size(); t++) {
        values.emplace_back(xValues[(numOfLastLevelLeaves - 1) * VAL_SIZE + t]);
    }
    // pad last leaf with infinities if not full
    for (t; t < VAL_SIZE; t++) {
        values.emplace_back(INFTY);
    }
    //construct last leaf and add it the tree
    tree.emplace_back(values, tree.size());

}

//build internal nodes bottom up after leaves were constructed.
void PointLocation::constructInternalNodes(std::vector<SuperNode> &tree, unsigned long long height) {
    unsigned long long numOfNodes; //number of actual nodes (not including the padded ones)
    unsigned long long numOfLevelNodes; //number of nodes at level i
    //go through every level of the tree and fill the tree vector from left to right.

    long long i, j, k; // read as value k at node j in level i
    long long offset; // a dummy variable to speed up calculation, reserving two multiplications
    for (i = height - 1; i > -1; i--) {
        numOfLevelNodes = pow(CHILD_SIZE, i); // numOfLevelNodes = B^i
        unsigned long long step = pow(CHILD_SIZE,
                                      height - i); //location of the node with maxValue within subtree
        //only true at the level before last level
        if (i == height - 1) {
            numOfNodes = numOfLastLevelLeaves % CHILD_SIZE == 0 ? numOfLastLevelLeaves / CHILD_SIZE :
                         numOfLastLevelLeaves / CHILD_SIZE + 1;
        } else {
            numOfNodes = numOfLevelNodes; // numOfLevelNodes = B^i
        }

//        cout << "step:" << step << endl;
//        cout << "level " << i;
        auto start = std::chrono::high_resolution_clock::now();
        for (j = step / CHILD_SIZE - 1; j < numOfLastLevelLeaves; j += step) {
            vector<double> values;
            values.reserve(VAL_SIZE);
            //cout << " " << j << " ";
            for (k = 0; k < VAL_SIZE && j + k < numOfLastLevelLeaves; k++) {
                values.emplace_back(tree[j + k].getVal().back());
                //values.emplace_back(tree[j * step + k].val.back());
            }
            //pad last values of the node with some values from last leaves level with infinities
            //TODO() fix last node in level i outside the for loop of j
            for (k; k < VAL_SIZE; k++) {
                values.emplace_back(INFTY);
            }
            tree.emplace_back(values, tree.size());
        }
        auto finish = std::chrono::high_resolution_clock::now();
        vector<double> values;

        std::chrono::duration<double> elapsed = finish - start;
        //cout << "\ntime to allocate memory of " << numOfNodes << " nodes at level: " << i << ": " << elapsed.count();
        //pad last nodes within level with infinities
        for (j; j < numOfLevelNodes; j++) {
            vector<double> values;
            values.reserve(VAL_SIZE);

            for (k = 0; k < VAL_SIZE; k++) {
                values.emplace_back(INFTY);
            }
            tree.emplace_back(values, tree.size());
        }
    }
}

//augment tree nodes with lineSegments top down.
void PointLocation::fillSuperTREE(std::vector<LineSegment> &lineSegments) {
    sort(lineSegments.begin(), lineSegments.end(), YLeftLessThan());
    //LineSegment lineSeg(12, 14, 15);
    //lineSegments.push_back(lineSeg);
    fillNode(this->getRoot(), lineSegments);
}

//function that recursively fill in the left HalfLines, RightHalfLines, and MultiSlabsLineSegments
void PointLocation::fillNode(SuperNode &root, std::vector<LineSegment> &lineSegments) {
    std::vector<LineSegment> middle;
    std::vector<std::vector<LineSegment> > left(VAL_SIZE);
    std::vector<std::vector<LineSegment> > right(VAL_SIZE);
    std::vector<std::vector<LineSegment> > remainingLineSegments(CHILD_SIZE);

    partitionLineSegments(root.getVal(), lineSegments, left, right, middle, remainingLineSegments);

    root.setLeftHalfLines(left);
    root.setRightHalfLines(right);
    root.setMiddle(middle);

    for (int i = 0; i < CHILD_SIZE; i++) {
        if (remainingLineSegments[i].empty()) {
            continue;
        } else {
            if (!this->isLastLevelLeaf(root)) {
                this->fillNode(this->getIthChild(root, i), remainingLineSegments[i]);
            }
            else {
                tr(root.getVal(),it)
                    cout << *it << " ";
                cout << endl;

                tr(remainingLineSegments[i], it)
                    cout << *it << " ";
                cout <<endl;
                cout << "out of boundary index: " << endl ;
            }
        }
    }
}

//partition lineSegments into left, right middle, and non-boundary intersecting lineSegments.
//left.size() = right.size() = boundaries.size() = remainingLineSegments - 1
void PointLocation::partitionLineSegments(const vector<double> &boundaries,
                                          const vector<LineSegment> &lineSegments,
                                          vector<vector<LineSegment> > &left,
                                          vector<vector<LineSegment> > &right,
                                          vector<LineSegment> &middle,
                                          vector<vector<LineSegment> > &remainingLineSegments) {

    for (auto lineSegment : lineSegments) {
        double xLeft = lineSegment.getXLeft();
        double xRight = lineSegment.getXRight();

        //find first boundary index such that max xLeft <= Boundary
        auto itStart = lower_bound(boundaries.begin(), boundaries.end(), xLeft);
        //find first boundary index such that max xRight <= boundary
        auto itEnd = lower_bound(boundaries.begin(), boundaries.end(), xRight);

        //case lineSegment starts after lastValue i.e boundaries[VALSIZE-1] < xLeft
        if (itStart == boundaries.end()) { // not needed
            remainingLineSegments.back().push_back(lineSegment);
        } else {
            //case lineSegment entirely inside slab i (between boundaries[i-1] < xLeft < xRight < boundaries[i] ), boundaries[-1] = -infinity
            if (itStart == itEnd) {
                if (xRight < *itEnd)
                    remainingLineSegments[itStart - boundaries.begin()].push_back(lineSegment);
                    //case xRight == *itEnd or xRight = boundaries[i]
                else
                    left[itEnd - boundaries.begin()].push_back(lineSegment);
            }
                //case intersecting one or more boundaries
            else {
                // case lineSegment starts before boundaries[i]
                if (xLeft < *itStart) {
                    left[itStart - boundaries.begin()].push_back(lineSegment);
                    //handle right end point
                    if (itEnd == boundaries.end()) // need guard here to avoid Segfault
                    {
                        //add to the right of last boundary
                        right.back().push_back(lineSegment);
                        // case multi-slab i.e. xLeft < Boundary[VALSIZE-2]
                        if (itEnd - itStart > 1) {
                            middle.push_back(lineSegment);
                        }
                    } else {
                        //xRight doesn't touch the boundary[i]
                        if (xRight < *itEnd) {
                            right[itEnd - boundaries.begin() - 1].push_back(lineSegment);
                            //case multi-slab
                            if (itEnd - itStart > 1) {
                                middle.push_back(lineSegment);
                            }
                        } else {
                            // it is a case of multi-slab because xLeft <= boundary[i] < xRight = boundary[j] for j >i
                            middle.push_back(lineSegment);
                        }
                    }
                }
                    //case xLeft == *itStart
                else {
                    //handle right end point
                    if (itEnd == boundaries.end()) // need guard here to avoid Segfault
                    {
                        //add to the right of last boundary
                        right.back().push_back(lineSegment);
                        // already a case of multi-slab
                        middle.push_back(lineSegment);

                    } else {
                        //xRight doesn't touch the boundary[i]
                        if (xRight < *itEnd) {
                            right[itEnd - boundaries.begin() - 1].push_back(lineSegment);
                            //case multi-slab
                            if (itEnd - itStart > 1) {
                                middle.push_back(lineSegment);
                            }
                        } else {
                            // it is a case of multi-slab because xLeft <= boundary[i] < xRight = boundary[j] for j >i
                            middle.push_back(lineSegment);
                        }
                    }
                }
            }
        }
    }

//    cout << "left" << endl;
//    for (int i = 0; i < left.size(); i++) {
//        cout << boundaries[i] << ": ";
//        tr(left[i], ij)cout << *ij << " ";
//        cout << endl;
//    }
//    cout << "right" << endl;
//    for (int i = 0; i < right.size(); i++) {
//        cout << boundaries[i] << ": ";
//        tr(right[i], ij)cout << *ij << " ";
//        cout << endl;
//    }
//
//    cout << "middle" << endl;
//    tr(middle, ij)cout << *ij << " ";
//    cout << endl;
}


LineSegment PointLocation::queryPointLocation(Point &point) {
    LineSegment lineSegment(-1, INFTY, -1);
    return lineSegment;
}


bool PointLocation::isLastLevelLeaf(SuperNode &node) {
    return node.getIndex() < numOfLastLevelLeaves;
}

//root located at the end of vector tree
SuperNode &PointLocation::getRoot() {
    return this->superTree.back();
}

SuperNode &PointLocation::getIthChild(SuperNode &node, unsigned long long i) {
    unsigned long long index = superTree.size() - node.getIndex();
    unsigned long long ithChildIndex = superTree.size() - 1 - index * (CHILD_SIZE) + i;

    //cout << "node Index: " << node.getIndex() << " with " << i << " child index: " << ithChildIndex << endl;
    return superTree[ithChildIndex];
}