#ifndef _REFRACTION_H_
#define _REFRACTION_H_

#include  "defs.h"
#include  "Ray.h"
#include  "Shape.h"
#include "Image.h"
#include "Material.h"
#include "Light.h"
#include "Shading.h"
#include "Reflection.h"

class Shading;
class Reflection;
class RayIntersection;
class Refraction
{
public:
	Refraction(Reflection* const reflection, Shading* const shading, const vector<Shape*>& objects,
		RayIntersection* const ray_ýntersection)
		: reflection(reflection),
		  shading(shading),
		  objects(objects),
		  rayIntersection(ray_ýntersection)
	{
	}

	Reflection* reflection;
	Shading* shading;
	vector<Shape*> objects;
	RayIntersection *rayIntersection;
	
	Color dialecticRefraction(Ray ray,Material material);
	float fresnel(const Vector3f& incoming, const Vector3f& normal, const float& refractionIndex, Material material, float n_i);
	void printColor(const Color& color);
	void refraction(int depth, Ray ray, ReturnVal& intersection, Material material, Color& color, Vector3f rayDirection, float n_i);
	Vector3f refract(const Vector3f& incoming, const Vector3f& normal, const float& refractionIndex, float n_i);
	
	Vector3f reflect(const Vector3f& incoming, const Vector3f& normal);
	
};
#endif

