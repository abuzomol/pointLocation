//
// Created by Muzamil on 6/23/20.
//

#include <iostream>
#include <cmath>
#include <chrono>
#include <tuple>
#include "constants.h"
#include "PointLocation.h"

using namespace std;

void PointLocation::constructLeaves(std::vector<SuperNode> &tree, std::vector<LineSegment> &lineSegments,
                                    unsigned long long superHeight,
                                    unsigned long long numOfLeaves) {
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
    sort(xValues.begin(), xValues.end());

    //add xValues to leaves, and store them at the beginning of array.
    //fastest observed performance, is when we reserve a vector inside the for loop, fill it, and then emplace it back to the tree with index i
    for (unsigned long long i = 0; i < numOfLeaves - 1; i++) {
        std::vector<double> values;
        values.reserve(VAL_SIZE);
        for (unsigned long long j = 0; j < VAL_SIZE; j++) {
            values.emplace_back(xValues[i * VAL_SIZE + j]);
        }
        //construct the superNode and add it to the tree
        tree.emplace_back(values, i);
    }
    //add the last leaf
    std::vector<double> values;
    values.reserve(VAL_SIZE);
    unsigned long long t;
    for (t = 0; (numOfLeaves - 1) * VAL_SIZE + t < xValues.size(); t++) {
        values.emplace_back(xValues[(numOfLeaves - 1) * VAL_SIZE + t]);
    }
    // pad last leaf with infinities if not full
    for (t; t < VAL_SIZE; t++) {
        values.emplace_back(INFTY);
    }
    //construct last leaf and add it the tree
    tree.emplace_back(values, numOfLeaves - 1);
}


void PointLocation::constructInternalNodes(std::vector<SuperNode> &tree, unsigned long long superHeight,
                                           unsigned long long noOfLastLevelLeaves) {
    unsigned long long numOfNodes; //number of actual nodes (not including the padded ones)
    unsigned long long numOfLevelNodes; //number of nodes at level i
    //go through every level of the tree and fill the tree vector from left to right.

    long long i, j, k; // read as value k at node j in level i
    unsigned long long offset; // a dummy variable to speed up calculation, reserving two multiplications
    for (i = superHeight - 2; i > -1; i--) {
        numOfLevelNodes = pow(CHILD_SIZE, i); // numOfLevelNodes = B^i
        unsigned long long step = pow(CHILD_SIZE,
                                      superHeight - 1 - i); //location of the node with maxValue within subtree
        //only true at the level before last level
        if (i == superHeight - 2) {
            numOfNodes = noOfLastLevelLeaves % CHILD_SIZE == 0 ? noOfLastLevelLeaves / CHILD_SIZE :
                         noOfLastLevelLeaves / CHILD_SIZE + 1;
        } else {
            numOfNodes = numOfLevelNodes; // numOfLevelNodes = B^i
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (j = 0; j * step < noOfLastLevelLeaves; j++) {
            vector<double> values;
            values.reserve(VAL_SIZE);
            for (k = 0; k < VAL_SIZE && j * step + k < noOfLastLevelLeaves; k++) {
                values.emplace_back(tree[j * step + k].getVal().back());
                //values.emplace_back(tree[j * step + k].val.back());
            }
            //pad last values of the node with some values from last leaves level with infinities
            //TODO() fix last node in level i outside the for loop of j
            for (k; k < VAL_SIZE; k++) {
                values.emplace_back(INFTY);
            }
            tree.emplace_back(values, tree.size() + 1);
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
            tree.emplace_back(values, tree.size() + 1);
        }
    }
}

void PointLocation::fillSuperTREE(std::vector<LineSegment> &lineSegments) {
    sort(lineSegments.begin(), lineSegments.end(), YLeftLessThan());


}

//constructor that preprocess a list of lineSegments for point location queries.
//The constructor call three functions: constructLeaves, constructInternalNodes and fillSuperTree
PointLocation::PointLocation(std::vector<LineSegment> &lineSegments) {
    // find the number of last level leaves, and then the height of the tree
    unsigned long long numOfXValues = 2 * lineSegments.size();
    unsigned long long numOfLastLevelLeaves = numOfXValues % VAL_SIZE == 0
                                              ? numOfXValues / VAL_SIZE
                                              : numOfXValues / VAL_SIZE + 1;
    unsigned long long superHeight =
            ceil(log(numOfLastLevelLeaves) / log(CHILD_SIZE)) + 1;

    // reserve the tree size : B^h - 1  + numOfLastLevelLeaves
    auto start = std::chrono::high_resolution_clock::now();
    this->superTree.reserve(pow(CHILD_SIZE, superHeight - 1) - 1 + numOfLastLevelLeaves);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "\ntime to allocate "<<  this->superTree.capacity() <<" superNodes for tree: " << elapsed.count();

    start = std::chrono::high_resolution_clock::now();
    constructLeaves(this->superTree, lineSegments, superHeight, numOfLastLevelLeaves);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "\ntime to construct " << numOfLastLevelLeaves << " leaves: " << elapsed.count();

    start = std::chrono::high_resolution_clock::now();
    constructInternalNodes(this->superTree, superHeight, numOfLastLevelLeaves);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cout << "\ntime to construct " << this->superTree.size() - numOfLastLevelLeaves << " internal Nodes: "
         << elapsed.count();

    start = std::chrono::high_resolution_clock::now();
    fillSuperTREE(lineSegments);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cout << "\ntime to fill the tree: " << elapsed.count();
}

LineSegment &PointLocation::queryPointLocation(Point &point) {
    LineSegment lineSegment(-1, INFTY, -1);
    return lineSegment;
}

