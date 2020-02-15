#include "RayIntersection.h"


ReturnVal RayIntersection::closestObject(Ray ray)
{
	Shape* shape;
	float closestObjectDistance = INT_MAX;
	ReturnVal closestObjectReturnVal = {};
	closestObjectReturnVal.objectID = -1;
	ReturnVal returnValue;
	// Selecting Closest object to the camera
	for (int o = 0; o < objectCount; o++)
	{
		shape = objects[o];
		returnValue = shape->intersect(ray);
		if (returnValue.isIntersect)
		{
			float tValue = returnValue.t;
			if (tValue < closestObjectDistance) {
				closestObjectDistance = tValue;
				closestObjectReturnVal.hitNormal = returnValue.hitNormal;
				closestObjectReturnVal.intersectionPoint = returnValue.intersectionPoint;
				closestObjectReturnVal.objectID = o;
				closestObjectReturnVal.isIntersect = true;
			}
		}
	}
	return closestObjectReturnVal;
}