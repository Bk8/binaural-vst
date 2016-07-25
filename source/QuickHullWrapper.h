/*
  ==============================================================================

    Delaunay.h
    Created: 12 Jul 2016 9:43:40am
    Author:  Alex

  ==============================================================================
*/
//https://github.com/ironwallaby/delaunay
//https://github.com/timbennett/delaunay
//https://www.youtube.com/watch?v=JbD-HsmBt_0

#ifndef DELAUNAY_H_INCLUDED
#define DELAUNAY_H_INCLUDED

#include "convexhull/quickhull.hpp"
#include <vector>

using namespace std;
using namespace quickhull;

class Point2D
{
    public:
    Point2D()
    {
        
    }
    Point2D(float x,float y)
    {
        this->x=x;
        this->y=y;
    }
    
    float x,y;
};



class QuickHullTriangulation
{

public:

    void triangulate(vector<Vector3<float>> points);
    
     Vector3<float> getTrianglePoint(int i);
    
    int getTriangleCount()
    {
        return triangles.size();
    }


    

private:
    vector<Vector3<float>> triangles;
    QuickHull<float> qh; // Could be double as well
    std::vector<Vector3<float>> pointCloud;

};



#endif  // DELAUNAY_H_INCLUDED
