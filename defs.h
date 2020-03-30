#ifndef _DEFS_H_
#define _DEFS_H_
#include <iostream>
#include <cmath>
#include "RayTracing/glm/vec3.hpp"

class Scene;
using namespace std;
/* Structure to hold return value from ray intersection routine.
This should hold information related to the intersection point,
for example, coordinate of the intersection point, surface normal at the intersection point etc.
Think about the variables you will need for this purpose and declare them here inside of this structure. */

/* 3 dimensional vector holding floating point numbers.
Used for both coordinates and color.
Use x, y, z for coordinate computations, and use r, g, b for color computations.
Note that you do not have to use this one if you use any vector computation library like Eigen. */


typedef struct IntersectionInfo
{
    bool isIntersect;
    glm::vec3 intersectionPoint;
    float t;
    glm::vec3 hitNormal;
	int objectID;

	IntersectionInfo()
	{
		isIntersect = false;
		objectID = -1;
		intersectionPoint = { 0,0,0 };
		t = -1;
		hitNormal = { 0,0,0 };
	}
	IntersectionInfo operator=(IntersectionInfo rhs)
	{
		isIntersect = rhs.isIntersect;
		intersectionPoint = rhs.intersectionPoint;
		t = rhs.t;
		hitNormal = rhs.hitNormal;
		objectID = rhs.objectID;

		return *this;
	}
} IntersectionInfo;


static float determinant(float matrix[3][3])
{
    float a = matrix[0][0];
    float b = matrix[1][0];
    float c = matrix[2][0];
    float d = matrix[0][1];
    float e = matrix[1][1];
    float f = matrix[2][1];
    float g = matrix[0][2];
    float h = matrix[1][2];
    float i = matrix[2][2];


        return a*(e*i-h*f) + b*(g*f-d*i)+c*(d*h-e*g);
}

//
// The global variable through which you can access the scene data
//
extern Scene* pScene;

#endif
