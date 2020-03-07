#ifndef _RAYINTERSECTION_H_
#define _RAYINTERSECTION_H_

#include  "defs.h"
#include  "Ray.h"
#include  "Shape.h"
#include "Image.h"
#include "Material.h"
#include "Light.h"
#include "Shading.h"
#include "BoundingVolume.h"
class BoundingVolume;
class RayIntersection
{
	vector<Shape*> objects;
	
	int objectCount;
public:
	BoundingVolume* boundingVolume;
	RayIntersection(const vector<Shape*>& objects, int object_count)
		: objects(objects),
		  objectCount(object_count)
	{
	}

	void updateRetVal(IntersectionInfo& retVal,const IntersectionInfo& newVal);
	IntersectionInfo BoundingBoxIntersection(Ray ray, Node* node, float *t_min, IntersectionInfo* retVal);
	void BoundingBoxIntersection(Ray ray, Node* node, IntersectionInfo* retVal);
	IntersectionInfo closestObject(Ray ray);
	IntersectionInfo closestObjectRefraction(Ray ray, Shape* shape);
};

#endif
