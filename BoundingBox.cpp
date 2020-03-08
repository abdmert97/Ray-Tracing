#include "BoundingBox.h"

void BoundingBox::extend(BoundingBox* boundingBox)
{
	min = getMin(min, boundingBox->min);
	max = getMax(max, boundingBox->max);
}

void BoundingBox::extend(Shape* shape)
{
	BoundingBox* boundingBox = shape->getBounds();
	min = getMin(min, boundingBox->min);
	max = getMax(max, boundingBox->max);
}

Vector3f  BoundingBox::getMin(Vector3f v1, Vector3f v2)
{
	return Vector3f{
		{v1.x < v2.x ? v1.x : v2.x},
		{v1.y < v2.y ? v1.y : v2.y},
		{v1.z < v2.z ? v1.z : v2.z}
	};
}
Vector3f BoundingBox::getMax(Vector3f v1, Vector3f v2)
{
	return Vector3f{
		{v1.x < v2.x ? v2.x : v1.x},
		{v1 .y < v2.y ? v2.y : v1.y},
		{v1.z < v2.z ? v2.z : v1.z}
	};
}

float BoundingBox::isIntersect(Ray ray)
{
	
	auto rayDirection = ray.direction;

	auto rayOrigin = ray.origin;
	
	float tmin = (min.x - rayOrigin.x) / rayDirection.x;
	float tmax = (max.x - rayOrigin.x) / rayDirection.x;

	if (tmin > tmax) swap(tmin, tmax);

	float tymin = (min.y - rayOrigin.y) / rayDirection.y;
	float tymax = (max.y - rayOrigin.y) / rayDirection.y;

	if (tymin > tymax) swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return -1;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (min.z - rayOrigin.z) / rayDirection.z;
	float tzmax = (max.z - rayOrigin.z) / rayDirection.z;

	if (tzmin > tzmax) swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return -1;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	
	return tmin;
}

void BoundingBox::printBounds()
{
	cout << "min: " << min << endl;
	cout << "max: " << max << endl;
}

Vector3f BoundingBox::midPoint()
{
	return (min + max) / 2;
}


