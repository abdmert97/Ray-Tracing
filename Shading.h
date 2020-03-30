#ifndef _SHADING_H_
#define _SHADING_H_

#include  "defs.h"
#include  "Ray.h"
#include  "Shape.h"
#include "Image.h"
#include "Material.h"
#include "Light.h"
#include "Reflection.h"
#include  "Refraction.h"
#include "RayIntersection.h"
class Reflection;
class Refraction;
class RayIntersection;
class Shading
{

private:
	
	float shadowRayEps;
	vector<Material*> materials;
	glm::vec3* ambientLightList;
	int lightCount;
	int objectCount;
	vector<PointLight*> lights;
	vector<Shape*> *objects;
public:
	Reflection* reflection;
	RayIntersection* rayIntersection;
	Refraction* refraction;
	Shading( float shadow_ray_eps, const vector<Material*>& materials, glm::vec3* ambient_light_list,
		int light_count, int object_count, const vector<PointLight*>& point_lights,vector<Shape*>* objects)
		: 
		  shadowRayEps(shadow_ray_eps),
		  materials(materials),
		  ambientLightList(ambient_light_list),
		  lightCount(light_count),
		  objectCount(object_count),
		  lights(point_lights),
		  objects(objects)
	{
	}

private:
	bool isShadow(glm::vec3& lightPosition, glm::vec3& intersectionPoint)const;
	void calculateColor(IntersectionInfo& closestObjectInfo, Material material, PointLight* light, glm::vec3 lightVector,
	glm::vec3 cameraVectorNormalized, glm::vec3& shaders) const;
	glm::vec3 blinnPhongShading(glm::vec3 lightRayVector, glm::vec3& cameraRayVector, Material& material,
	glm::vec3& lightIntensity, glm::vec3& normal) const;
	glm::vec3 diffuseShading(glm::vec3 lightRayVector, Material& material, glm::vec3& lightIntensity, glm::vec3& normal)const;
public:
	glm::vec3 shading(int depth, Shape*& shape, IntersectionInfo& closestObjectInfo, Ray& ray, float n_t =1);
	
};



#endif