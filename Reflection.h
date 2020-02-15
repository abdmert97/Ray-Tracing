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
	RayIntersection* rayIntersection;
	Shading* shading;
	float shadowRayEps;
	vector<Shape*> objects;
	Vector3f backgroundColor;
public:
	Reflection(RayIntersection* const ray_ýntersection, Shading* const shading, const float shadow_ray_eps,
		const vector<Shape*>& objects, const Vector3f& background_color)
		: rayIntersection(ray_ýntersection),
		  shading(shading),
		  shadowRayEps(shadow_ray_eps),
		  objects(objects),
		  backgroundColor(background_color)
	{
	}

	void getReflection(int depth, ReturnVal& closestObjectReturnVal, Material material, Color& color,
	                   Vector3f cameraVectorNormalized);
};
#endif

