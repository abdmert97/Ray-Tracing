#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include "defs.h"
#include "Ray.h"
#include "Shape.h"
class Shape;

class BoundingBox
{

public:
	Vector3f min;
	Vector3f max;
	BoundingBox()
	{
		min = { INT16_MAX,INT16_MAX ,INT16_MAX };
		max = { INT16_MIN,INT16_MIN ,INT16_MIN };
	
	}
	BoundingBox(BoundingBox box1, BoundingBox box2)
	{
		min = getMin(box1.min, box2.min);
		max = getMin(box1.max, box2.max);
	}
	/*BoundingBox(Shape* shape1, Shape* shape2)
	{
		BoundingBox * firstBound  = shape1->getBounds();
		BoundingBox * secondBound = shape2->getBounds();
		min = getMin(firstBound->min, secondBound->min);
		max = getMax(firstBound->max, secondBound->max);
	}*/
	void extend(BoundingBox *boundingBox);
	void extend(Shape * shape);
	static Vector3f getMin(Vector3f v1, Vector3f v2);
	static Vector3f getMax(Vector3f v1, Vector3f v2);
	short isIntersect(Ray ray);
	void printBounds();
	Vector3f midPoint();
};
#endif

