#include "BoundingVolume.h"
#include <ctime>


void BoundingVolume::build(Node* node)
{
	int size = node->ObjectIDs.size();
/*	for (int i : node->ObjectIDs)
		cout << i;
	cout << endl;*/
	vector<Vector3f> midpoints;
	for(int i = 0; i< size;i++)
	{
		Shape* shape = objects[node->ObjectIDs[i]];
		BoundingBox *bounds = shape->getBounds();
		node->boundingBox->extend(bounds);
		midpoints.push_back(bounds->midPoint());
		
	}
	if (size <= 2) return;
	
	
	std::sort(midpoints.begin(), midpoints.end());
	Vector3f midPoint = midpoints[midpoints.size() / 2];
	
	node->left = new Node;
	node->right = new Node;
	for (int i = 0; i < size; i++)
	{
		Shape* shape = objects[node->ObjectIDs[i]];
		BoundingBox* shapeBound = shape->getBounds();
		if (i<=size/2)
		{
			node->left->ObjectIDs.push_back(node->ObjectIDs[i]);
		}
		else
			node->right->ObjectIDs.push_back(node->ObjectIDs[i]);
	}
	build(node->left);
	build(node->right);
}
bool BoundingVolume::isInLeft(const Vector3f& midPoint, BoundingBox* bounding_box)
{
	if (midPoint.x > (bounding_box->min.x + bounding_box->max.x) / 2)
		return true;
	return false;
}
bool BoundingVolume::sortVector(const Vector3f &v1,const Vector3f &v2)
{
	return v1.x >= v2.x;
}
