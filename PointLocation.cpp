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
                                    long long superHeight,
                                    long long numOfLeaves) {
    //get xValues first. Note, that we can speed up this construction, if the values are stored in a vector separate from rest of superNode members.
    //It will speed up processes like sorting xValues, but we loose locality here.
    std::vector<double> xValues;
    xValues.reserve(lineSegments.size() * 2);
    for (LineSegment &lineSegment: lineSegments) {
        xValues.emplace_back(lineSegment.getXLeft());
        xValues.emplace_back(lineSegment.getXRight());
    }
    sort(xValues.begin(), xValues.end());
    // add xValues to leaves, and store them at the beginning of array

    for (long long i = 0; i < numOfLeaves - 1; i++) {
        std::vector<double> values;
        values.reserve(VAL_SIZE);
        for (long long j = 0; j < VAL_SIZE; j++) {
            values.emplace_back(xValues[i * VAL_SIZE + j]);
        }
        tree.emplace_back(values, i);
    }
    //add the last leaf
    std::vector<double> values;
    values.reserve(VAL_SIZE);
    int t;
    for (t = 0; (numOfLeaves - 1) * VAL_SIZE + t < xValues.size(); t++) {
        values.emplace_back(xValues[(numOfLeaves - 1) * VAL_SIZE + t]);
    }
    // pad last leaf with infinities if not full
    for (t; t < VAL_SIZE; t++) {
        values.emplace_back(INFTY);
    }
    tree.emplace_back(values, numOfLeaves - 1);
}


void PointLocation::constructInternalNodes(std::vector<SuperNode> &tree, long long superHeight,
                                           long long noOfLastLevelLeaves) {

    long long numOfNodes;
    long long numOfLevelNodes;
    //go through every level of the tree and fill the tree vector from left to right.
    long long i,j,k, offset;
    for (i = superHeight - 2; i > -1; i--) {
        numOfLevelNodes = pow(CHILD_SIZE, i);
        long long step = pow(CHILD_SIZE, superHeight - 1 - i);
        //cout << "\nstep size:" << step;

        if (i == superHeight - 2) {
            numOfNodes = noOfLastLevelLeaves % CHILD_SIZE == 0 ? noOfLastLevelLeaves / CHILD_SIZE :
                         noOfLastLevelLeaves / CHILD_SIZE + 1;
       //     cout << "\nnumber of nodes: " << numOfNodes;

        } else {
            numOfNodes = numOfLevelNodes;
        }

        auto start = std::chrono::high_resolution_clock::now();
        //j < numOfNodes &&
        for (j = 0 ; j*step  < noOfLastLevelLeaves; j++) {
            vector<double> values;
            values.reserve(VAL_SIZE);
            for ( k= 0; k < VAL_SIZE && j*step + k < noOfLastLevelLeaves; k++) {
                values.emplace_back(tree[j*step + k].getVal().back());
            }
            //pad last values of the node with some vqlues from last leaves level with infinities
           /* for(k ; k < VAL_SIZE; k++)
            {
                values.emplace_back(INFTY);
            }
            */
            tree.emplace_back(values, tree.size() + 1);
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        cout << "\ntime to allocate memory for nodes at level: " << i << ": " << elapsed.count();
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

PointLocation::PointLocation(std::vector<LineSegment> &lineSegments) {
    long long numOfXValues = 2 * lineSegments.size();
    long long numOfLastLevelLeaves = numOfXValues % VAL_SIZE == 0
                                     ? numOfXValues / VAL_SIZE
                                     : numOfXValues / VAL_SIZE + 1;
    unsigned int superHeight =
            ceil(log(numOfLastLevelLeaves) / log(CHILD_SIZE)) + 1;

    // reserve the tree size : B^h - 1  + numOfLastLevelLeaves
    auto start = std::chrono::high_resolution_clock::now();
    this->superTree.reserve(pow(CHILD_SIZE, superHeight - 1) - 1 + numOfLastLevelLeaves);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "time to allocate memory for tree: " << elapsed.count();
    cout<< "\nnumber of leaves: " << numOfLastLevelLeaves;
    start = std::chrono::high_resolution_clock::now();
    constructLeaves(this->superTree, lineSegments, superHeight, numOfLastLevelLeaves);
    finish = std::chrono::high_resolution_clock::now();
    std::cout << "\ntime to construct leaves: " << elapsed.count();

    start = std::chrono::high_resolution_clock::now();
    constructInternalNodes(this->superTree, superHeight, numOfLastLevelLeaves);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;

    cout <<"\ntime to construct internal Nodes: " << elapsed.count();
}

LineSegment &PointLocation::queryPointLocation(Point &point) {

}