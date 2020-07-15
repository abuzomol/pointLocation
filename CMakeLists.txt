cmake_minimum_required(VERSION 3.16)
project(pointLocation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O0")

add_executable(pointLocation main.cpp PointLocation.cpp PointLocation.h LineSegment.cpp LineSegment.h Point.cpp Point.h constants.h SuperNode.cpp SuperNode.h HalfLines.cpp HalfLines.h MultiSlabsLineSegment.cpp MultiSlabsLineSegment.h MiddleNode.cpp MiddleNode.h Node.cpp Node.h SuperLeaf.cpp SuperLeaf.h)