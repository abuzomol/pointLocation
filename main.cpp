#include <iostream>
#include <fstream>
#include <chrono>
#include "constants.h"
#include "PointLocation.h"

using namespace std;

void test()
{
    std::vector<LineSegment> lineSegments;
    lineSegments.reserve(8);
    lineSegments.emplace_back(0, 5, 3);
    lineSegments.emplace_back(1, 4, 6);
    lineSegments.emplace_back(2, 13, 8);
    lineSegments.emplace_back(3, 7, 1);
    lineSegments.emplace_back(6, 9, 7);
    lineSegments.emplace_back(8, 11, 5);
    lineSegments.emplace_back(10, 14, 4);
    lineSegments.emplace_back(12, 15, 2);
    // Test Queries
    std::vector<Point> points;
    points.reserve(9);
    points.emplace_back(7, 1.5);
    points.emplace_back(13, 2.5);
    points.emplace_back(3, 3.5);
    points.emplace_back(10, 4.5);
    points.emplace_back(8, 5.5);
    points.emplace_back(3, 6.5);
    points.emplace_back(8, 7.5);
    points.emplace_back(7, 8.5);
    points.emplace_back(16, 15);

    //preprocessing
    sort(lineSegments.begin(), lineSegments.end(), YLeftLessThan());
    for(auto& line: lineSegments)
        cout << line <<endl;

    PointLocation pointLocation(lineSegments);

}

void getLineSegments(vector<LineSegment>& lineSegments, long long size)
{
    lineSegments.reserve(size);
    string fileName = "/Users/muzamil/Desktop/point-location/objects.100M.1583107550.random";

    char* lineBlock = new char[sizeof(double) * 3];

    auto start = std::chrono::high_resolution_clock::now();
    ifstream finLines(fileName, ios::binary);
    //read lineSegments from file
    for (unsigned int i = 0; i < lineSegments.capacity(); i++)
    {
        double* doubleBlock = (double*)lineBlock;

        finLines.read(lineBlock, sizeof(double) * 3);
        lineSegments.emplace_back(doubleBlock[0], doubleBlock[1], doubleBlock[2]);
    }
    finLines.close();

    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Time to read lines       : " << elapsed.count()
              << " s\n";

}

void getPoints(vector<Point>& points, long long size)
{
    points.reserve(size);
    string fileName = "/Users/muzamil/Desktop/point-location/objects.100M.1583107551.random";

    char* lineBlock = new char[sizeof(double) * 2];

    auto start = std::chrono::high_resolution_clock::now();
    ifstream finLines(fileName, ios::binary);
    //read lineSegments from file
    for (unsigned int i = 0; i < points.capacity(); i++)
    {
        double* doubleBlock = (double*)lineBlock;

        finLines.read(lineBlock, sizeof(double) * 2);
        points.emplace_back(doubleBlock[0] *-1 , doubleBlock[1] *-1);
    }
    finLines.close();

    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Time to read points       : " << elapsed.count()
              << " s\n";
}

int main() {
    vector<LineSegment> lineSegments;
    vector<Point> points;
    vector<long long> lineSizes = {1000000, 10000000};
    vector<long long> pointSizes = {500000,1000000,2000000,5000000,10000000};
    getLineSegments(lineSegments, lineSizes[1]);
    getPoints(points, pointSizes[0]);

    //test();
    return 0;
}
