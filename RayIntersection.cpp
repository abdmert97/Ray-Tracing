#include "RayIntersection.h"
#include <algorithm>



void RayIntersection::BoundingBoxIntersection(Ray ray,Node *node,IntersectionInfo* retVal)
{

	float t_min = retVal->t;
	
	float t_intersection = node->boundingBox->isIntersect(ray);

	if(t_intersection != -1)
	{
		
		if(node->left != nullptr)
		{
			BoundingBoxIntersection(ray, node->left,retVal);
		}
		if(node->right != nullptr)
		{
			BoundingBoxIntersection(ray, node->right, retVal);
		}
	
		if(node->left == nullptr || node->right == nullptr)
		{
		
			for (int i = node->start; i <= node->end; i++)
			{
				Shape* shape = objects[i];
				IntersectionInfo intesectionInfo = shape->intersect(ray);
				if (intesectionInfo.isIntersect == true)
				{
				
					if (intesectionInfo.t <= t_min)
					{
						t_min = intesectionInfo.t;
						intesectionInfo.objectID = i;
						*retVal = intesectionInfo;
					}
					
				}
			}
		}
	}
	
	
	
}

IntersectionInfo RayIntersection::closestObject(Ray ray)
{
	

	IntersectionInfo returnValue{};
	
	float t = 9999;
	returnValue.t = t;
	Node* node = boundingVolume->root;
	if(t>1)
	BoundingBoxIntersection(ray,node,&returnValue);
	
	// Selecting Closest object to the camera
	
	return  returnValue;
}

