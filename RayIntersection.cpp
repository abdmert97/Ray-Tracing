#include "RayIntersection.h"
#include <algorithm>



void RayIntersection::BoundingBoxIntersection(Ray ray,Node *node,IntersectionInfo* retVal)
{
	float t_min = retVal->t;
	
	for (int i = 0 ; i < node->ObjectIDs.size();i++)
	{
		const Shape* shape = objects[node->ObjectIDs[i]];
		Ray rayTransformed = Ray(ray.origin, ray.direction);
		rayTransformed = shape->applyTransform(rayTransformed);
		if (shape->bounds->isIntersect(rayTransformed) <= t_min)
		{
			
			IntersectionInfo intesectionInfo = shape->intersect(ray,&rayTransformed);
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
	
	/*float t_min = retVal->t;
	float t_int = node->boundingBox->isIntersect(ray);
	if (t_int != -1 && t_int<= t_min )
	{
		if(node->left == nullptr && node->right == nullptr)
		{
			const Shape* shape = objects[node->ObjectIDs[0]];
			if (shape->bounds->isIntersect(ray) <=t_min)
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
		if (node->left != nullptr)
		{
			BoundingBoxIntersection(ray, node->left, retVal);
		}

		if (node->right != nullptr)
		{
			BoundingBoxIntersection(ray, node->right, retVal);
		}
	}*/
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

