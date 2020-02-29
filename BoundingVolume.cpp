#include "BoundingVolume.h"
#include <ctime>


void BoundingVolume::build(Node* node, int left, int right)
{

	Shape* shapeLeft = objects[left];
	Shape* shapeRight = objects[right];
	node->start = left;
	node->end = right;
	
	for(int i = left; i<= right;i++)
	{
		Shape* shape = objects[i];
		BoundingBox* shapeBound = shape->getBounds();
		node->boundingBox->extend(shapeBound);
	}
	if (left + 1 >= right) return;
	Vector3f midPoint = node->boundingBox->midPoint();

	int arrayMid = (left + right) / 2;
	int leftPoint = left;
	for (int i = left; i <= right; i++)
	{
		Shape* shape = objects[i];
		BoundingBox* shapeBound = shape->getBounds();
		if(isInLeft(midPoint,shapeBound))
		{
			std::swap(objects[i], objects[leftPoint++]);
		}
	}
	if (leftPoint == left || leftPoint == right) {
		leftPoint = (left + right) / 2; 
	}

	if (left < leftPoint) {
		node->left = new Node;
		level++;
		build(node->left, left, leftPoint);
	}
	if (leftPoint < right) {
		level++;
		node->right = new Node;
		build(node->right, leftPoint, right);
	}
	
}
bool BoundingVolume::isInLeft(const Vector3f& midPoint, BoundingBox* bounding_box)
{
	if (midPoint.x > (bounding_box->min.x + bounding_box->max.x) / 2)
		return true;
	return false;
}
