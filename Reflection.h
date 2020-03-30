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
	glm::vec3 backgroundColor;
	Shading* shading;
	RayIntersection* rayIntersection;

	Reflection( const float shadow_ray_eps,
		 vector<Shape*>* objects, const glm::vec3& background_color)
		:
		  shadowRayEps(shadow_ray_eps),
		  objects(objects),
		  backgroundColor(background_color)
	{
	}
	glm::vec3 reflect(const glm::vec3& incoming, const glm::vec3& normal);
	void getReflection(int depth, IntersectionInfo& intersectionInfo, Material material, glm::vec3& color,
	                   glm::vec3 cameraVectorNormalized);
};
#endif

