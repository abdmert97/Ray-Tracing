#ifndef _SHADING_H_
#define _SHADING_H_

#include  "defs.h"
#include  "Ray.h"
#include  "Shape.h"
#include "Image.h"
#include "Material.h"
#include "Light.h"
#include "Reflection.h"

class Reflection;

class Shading
{

private:
	Reflection* reflection;
	float shadowRayEps;
	vector<Material*> materials;
	Color* ambientLightList;
	int lightCount;
	int objectCount;
	vector<PointLight*> lights;
	vector<Shape*> objects;
public:
	Shading(Reflection* reflection, float shadow_ray_eps, const vector<Material*>& materials, Color* ambient_light_list,
		int light_count, int object_count, const vector<PointLight*>& point_lights, const vector<Shape*>& objects)
		: reflection(reflection),
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
	bool isShadow(Vector3f& lightPosition, Vector3f& intersectionPoint)const;
	void calculateColor(ReturnVal& closestObjectReturnVal, Material material, PointLight* light, Vector3f lightVector,
	Vector3f cameraVectorNormalized, Vector3f& shaders) const;
	Vector3f blinnPhongShading(Vector3f lightRayVector, Vector3f& cameraRayVector, Material& material,
	Vector3f& lightIntensity, Vector3f& normal) const;
	Vector3f diffuseShading(Vector3f lightRayVector, Material& material, Vector3f& lightIntensity, Vector3f& normal)const;
public:
	Color shading(int depth, Shape*& shape, ReturnVal& closestObjectReturnVal, Ray& ray);
	
};



#endif