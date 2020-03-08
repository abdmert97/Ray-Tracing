#ifndef _REFLECTION_H_
#define _REFLECTION_H_

#include  "defs.h"
#include  "Ray.h"
#include  "Shape.h"
#include "Image.h"
#include "Material.h"
#include "Light.h"
#include "Shading.h"
#include "RayIntersection.h"
class Shading;
class RayIntersection;
class Reflection
{
	
	float shadowRayEps;
	vector<Shape*> *objects;
	
public:
	Vector3f backgroundColor;
	Shading* shading;
	RayIntersection* rayIntersection;

	Reflection( const float shadow_ray_eps,
		 vector<Shape*>* objects, const Vector3f& background_color)
		:
		  shadowRayEps(shadow_ray_eps),
		  objects(objects),
		  backgroundColor(background_color)
	{
	}
	Vector3f reflect(const Vector3f& incoming, const Vector3f& normal);
	void getReflection(int depth, IntersectionInfo& intersectionInfo, Material material, Color& color,
	                   Vector3f cameraVectorNormalized);
};
#endif

