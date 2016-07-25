/*
  ==============================================================================

    DelaunayWrapper.cpp
    Created: 13 Jul 2016 12:12:03pm
    Author:  Alex

  ==============================================================================
*/

#include "QuickHullWrapper.h"

void QuickHullTriangulation::triangulate(vector<Vector3<float>> points)
{
    Vector3<float> aux;
    
    for(int i = 0; i < points.size(); i++) {
        
        aux.x=points[i].x;
        aux.y=points[i].y;
        aux.z=points[i].z;
        pointCloud.push_back(aux);
    }
    
    auto hull = qh.getConvexHull(pointCloud, true, false);
    auto indexBuffer = hull.getIndexBuffer();
    auto vertexBuffer = hull.getVertexBuffer();
    
    for(int i=0;i<indexBuffer.size();i++)
    {
        aux.x=vertexBuffer[indexBuffer[i]].x;
        aux.y=vertexBuffer[indexBuffer[i]].y;
        aux.z=vertexBuffer[indexBuffer[i]].z;
        triangles.push_back(aux);
    }
    
}


 Vector3<float> QuickHullTriangulation::getTrianglePoint(int i)
{
    return triangles[i];
}
