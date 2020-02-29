#include "RayIntersection.h"
#include <algorithm>




ReturnVal RayIntersection::BoundingBoxIntersection(Ray ray,Node *node,float *t_min,ReturnVal* retVal)
{
	float* t = new float();

	if(node->boundingBox->isIntersect(ray,t))
	{
		if(node->left != nullptr)
		{
			BoundingBoxIntersection(ray, node->left, t_min,retVal);
		}
		if(node->right != nullptr)
		{
			BoundingBoxIntersection(ray, node->right, t_min, retVal);
		}
		Shape* leftShape = objects[node->start];
		Shape* rightShape = objects[node->end];

		ReturnVal leftVal = leftShape->intersect(ray);
		ReturnVal rightVal = rightShape->intersect(ray);
		if (leftVal.isIntersect)
		{
			if (leftVal.t < *t_min)
			{
				*t_min = leftVal.t;
				leftVal.objectID = node->start;
				*retVal = leftVal;
			}
		}
		if (rightVal.isIntersect)
		{
			if (rightVal.t < *t_min)
			{
				*t_min = rightVal.t;
				rightVal.objectID = node->end;
				*retVal = rightVal;
			}
		}	
	}
	return *retVal;
}

ReturnVal RayIntersection::closestObject(Ray ray)
{
	Shape* shape;
	float closestObjectDistance = INT_MAX;
	ReturnVal closestObjectReturnVal = {};
	closestObjectReturnVal.objectID = -1;
	ReturnVal* returnValue = new ReturnVal();
	returnValue->objectID = -1;
	float* t = new float();
	*t = 99999;
	Node* node = boundingVolume->root;
	//BoundingBoxIntersection(ray,node,t,returnValue);

	/*if (returnValue.isIntersect)
	{
			closestObjectDistance = returnValue.t;
			closestObjectReturnVal.hitNormal = returnValue.hitNormal;
			closestObjectReturnVal.intersectionPoint = returnValue.intersectionPoint;
			closestObjectReturnVal.objectID = returnValue.objectID;
			closestObjectReturnVal.isIntersect = true;
	}*/
	//return *returnValue;
	// Selecting Closest object to the camera

	for (int o = 0; o < objectCount; o++)
	{
		shape = objects[o];
		*returnValue = shape->intersect(ray);
		if (returnValue->isIntersect)
		{
			float tValue = returnValue->t;
			if (tValue < closestObjectDistance) {
				closestObjectDistance = tValue;
				closestObjectReturnVal.hitNormal = returnValue->hitNormal;
				closestObjectReturnVal.intersectionPoint = returnValue->intersectionPoint;
				closestObjectReturnVal.objectID = o;
				closestObjectReturnVal.isIntersect = true;
				closestObjectReturnVal.t = tValue;
			}
		}
	}
	return closestObjectReturnVal;
}
