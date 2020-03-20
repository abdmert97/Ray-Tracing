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

template <typename T>
void SetMax(T& val0, const T& val1) { val0 = val0 < val1 ? val1 : val0; }

template<typename T>
void SetMin(T& val0, const T& val1) { val0 = val0 > val1 ? val1 : val0; }

int BoundingBox::isIntersect(Ray r)
{
	
	// Initialize with min and max values
	float tn = 0; // Initialize with 0 since rays go only forward
	float tf = std::numeric_limits<float>::max(); // Initialize with inf 
	// --

	// Check for x 
	float tnn = (min.x - r.origin.x) / r.direction.x;
	float tff = (max.x - r.origin.x) / r.direction.x;

	if (tnn > tff)
		std::swap(tnn, tff);

	SetMax(tn, tnn);
	SetMin(tf, tff);

	if (tf < tn)
		return -1;
	// --

	// Check for y
	tnn = (min.y - r.origin.y) / r.direction.y;
	tff = (max.y - r.origin.y) / r.direction.y;

	if (tnn > tff)
		std::swap(tnn, tff);

	SetMax(tn, tnn);
	SetMin(tf, tff);

	if (tf < tn)
		return -1;
	// --

	// Check for z
	tnn = (min.z - r.origin.z) / r.direction.z;
	tff = (max.z - r.origin.z) / r.direction.z;

	if (tnn > tff)
		std::swap(tnn, tff);

	SetMax(tn, tnn);
	SetMin(tf, tff);

	if (tf < tn)
		return -1;
	// -- 

	return 1;
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


