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
		
			for (int i = 0; i < node->ObjectIDs.size(); i++)
			{
				const Shape* shape = objects[node->ObjectIDs[i]];
				if (shape->bounds->isIntersect(ray) == -1) continue;;
				IntersectionInfo intesectionInfo = shape->intersect(ray);
				if (intesectionInfo.isIntersect == true)
				{
					if (intesectionInfo.t <= t_min)
					{
						t_min = intesectionInfo.t;
						intesectionInfo.objectID = node->ObjectIDs[i];
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

	BoundingBoxIntersection(ray,node,&returnValue);
	
	// Selecting Closest object to the camera
	
	return  returnValue;
}

