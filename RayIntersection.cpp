#include "RayIntersection.h"
#include <algorithm>



void RayIntersection::BoundingBoxIntersection(Ray ray,Node *node,IntersectionInfo* retVal)
{
	
	float t_min = retVal->t;
	


	if(node->boundingBox->isIntersect(ray) != -1)
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
			const Shape* shape = objects[node->ObjectIDs[0]];
			if (shape->bounds->isIntersect(ray) != -1)
			{
				IntersectionInfo intesectionInfo = shape->intersect(ray);
				if (intesectionInfo.isIntersect == true)
				{
					if (intesectionInfo.t <= t_min)
					{
						t_min = intesectionInfo.t;
						intesectionInfo.objectID = node->ObjectIDs[0];
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
	

	returnValue.t = 9999;
	Node* node = boundingVolume->root;

	BoundingBoxIntersection(ray,node,&returnValue);
	
	// Selecting Closest object to the camera
	
	return  returnValue;
}

