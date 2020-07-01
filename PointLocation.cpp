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

//root located at the end of vector tree
SuperNode& PointLocation::getRoot()
{
    return this->superTree.back();
}

//place the leaves at the beginning of the vector tree
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

//build internal nodes bottom up after leaves were constructed.
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
//augment tree nodes with lineSegments top down.
void PointLocation::fillSuperTREE(std::vector<LineSegment> &lineSegments) {
    sort(lineSegments.begin(), lineSegments.end(), YLeftLessThan());

    std::vector<LineSegment> middle;
    std::vector<std::vector<LineSegment> > left(VAL_SIZE);
    std::vector<std::vector<LineSegment> > right(VAL_SIZE);
    std::vector<std::vector<LineSegment> > remainingLineSegments(CHILD_SIZE);

    SuperNode node =  this->getRoot();
    partitionLineSegments(node.getVal(),lineSegments, left,right,middle, remainingLineSegments);


}

//constructor that preprocess a list of lineSegments for point location queries.
//The constructor call three functions: constructLeaves, constructInternalNodes and fillSuperTree
PointLocation::PointLocation(std::vector<LineSegment> &lineSegments) {
    // find the number of last level leaves, and then the height of the tree
    unsigned long long numOfXValues = 2 * lineSegments.size();
    this->numOfLastLevelLeaves = numOfXValues % VAL_SIZE == 0
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


void PointLocation::partitionLineSegments(const vector<double>& slabs,
                           vector<LineSegment>& lineSegments,
                           vector<vector<LineSegment> >& left,
                           vector<vector<LineSegment> >& right,
                           vector<LineSegment>& middle,
                           vector<vector<LineSegment> >& remainingLineSegments)
{
    // TODO() remove if statements
    for (auto lineSegment : lineSegments)

    {
        // for loop that scan every line segment
        for (int i = 0; i < VAL_SIZE; i++)
        {
            // if the lineSegment crosses or touches boundary i
            if (lineSegment.getXLeft() <= slabs[i]
                && lineSegment.getXRight() >= slabs[i])
            {
                // to the left of first boundary
                if (i == 0 && lineSegment.getXLeft() < slabs[i])
                {
                    left[i].push_back(lineSegment);
                }
                // starts at slab i-1
                if (i > 0 && slabs[i-1] < lineSegment.getXLeft()
                    && lineSegment.getXLeft() < slabs[i])
                {
                    left[i].push_back(lineSegment);
                }
                // to the right of last boundary

                if (i == VAL_SIZE - 1
                    && lineSegment.getXRight() > slabs[i])
                {
                    right[i].push_back(lineSegment);
                }

                // ends at slab i
                if (i < VAL_SIZE - 1
                    && lineSegment.getXRight() < slabs[i+1]
                    && lineSegment.getXRight() > slabs[i])
                {
                    right[i].push_back(lineSegment);
                }
                // case lineSegment crosses slab i
                if (i < VAL_SIZE - 1
                    && lineSegment.getXRight() >= slabs[i+1])
                {
                    if (!middle.empty() && middle.back() == lineSegment)
                        continue;
                    middle.push_back(lineSegment);
                }
            }
                // case it does not cross any boundary
            else
            {
                // case it ends before first boundary
                if (i == 0 && lineSegment.getXRight() < slabs[i])
                {
                    remainingLineSegments[i].push_back(lineSegment);
                }
                // case it starts after last boundary
                if (i == VAL_SIZE - 1
                    && lineSegment.getXLeft() > slabs[i])
                {
                    remainingLineSegments[i + 1].push_back(lineSegment);
                }
                // case it is between two boundaries i and i +1 i.e. b_i < left
                // x < b_i+1

                if (i > 0 && lineSegment.getXLeft() > slabs[i-1]
                    && lineSegment.getXRight() < slabs[i])
                {
                    remainingLineSegments[i].push_back(lineSegment);
                }
            }
        }
    }
}


LineSegment &PointLocation::queryPointLocation(Point &point) {
    LineSegment lineSegment(-1, INFTY, -1);
    return lineSegment;
}

bool PointLocation::isLeaf(SuperNode& node)
{
    return node.getIndex() < numOfLastLevelLeaves;
}

SuperNode & PointLocation::getIthChild(SuperNode &node, unsigned long long i)
{
    unsigned long long index = superTree.size()  - node.getIndex();
    unsigned long long ithChildIndex = superTree.size()  - ((index+1)*CHILD_SIZE - (CHILD_SIZE - 1) + i);
    return superTree[ithChildIndex];
}