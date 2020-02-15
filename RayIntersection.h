#ifndef _RAYINTERSECTION_H_
#define _RAYINTERSECTION_H_

#include  "defs.h"
#include  "Ray.h"
#include  "Shape.h"
#include "Image.h"
#include "Material.h"
#include "Light.h"
#include "Shading.h"

class RayIntersection
{
	vector<Shape*> objects;
	int objectCount;
public:
	RayIntersection(const vector<Shape*>& objects, int object_count)
		: objects(objects),
		  objectCount(object_count)
	{
	}

	ReturnVal closestObject(Ray ray);
};

#endif
