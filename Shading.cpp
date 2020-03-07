#include "Shading.h"
#include "Light.h"
#include "Scene.h"


Color Shading::shading(int depth, Shape*& shape, IntersectionInfo& closestObjectInfo, Ray& ray,float n_t)
{
	// If depth is less then zero stop shading
	if (depth < 0)
	{
		Color color = { 0,0,0 };
		return color;
	}
	
	Material material = *materials[shape->matIndex];

	bool isInside = ray.direction.dotProduct(closestObjectInfo.hitNormal) > 0;
	
	Color color = { 0,0,0 };
	Vector3f cameraVector = (ray.origin - closestObjectInfo.intersectionPoint);
	Vector3f cameraVectorNormalized = cameraVector.normalizeVector();
	if(!isInside)
	{
		color = ambientLightList[shape->matIndex];

		PointLight* light;
		Vector3f lightVector;
		
		for (int l = 0; l < lightCount; l++)
		{
			light = lights[l];
			lightVector = (light->position - closestObjectInfo.intersectionPoint);
			// Shadows
			Vector3f lightPosition = light->position;
			if (isShadow(lightPosition, closestObjectInfo.intersectionPoint))
			{
				continue;
			}
			// Shading
			Vector3f shaders;
			
			calculateColor(closestObjectInfo, material, light, lightVector, cameraVectorNormalized, shaders);
			color = color + shaders;

		}
	}
	

	// Reflection
	if (material.materialType == Default)
		return color;
	if (material.materialType == Mirror)
		reflection->getReflection(depth, closestObjectInfo, material, color, cameraVectorNormalized);
	else
		refraction->refraction(depth, ray, closestObjectInfo, material, color,ray.direction,n_t);
	return color;
}
bool Shading::isShadow(Vector3f& lightPosition, Vector3f& intersectionPoint)const
{
	Vector3f lightVector = lightPosition - intersectionPoint;
	
	Ray* shadowRay = new Ray((intersectionPoint + lightVector.normalizeVector() * shadowRayEps), lightVector.normalizeVector());
	Ray* shadowRay_ = new Ray(intersectionPoint, lightVector.normalizeVector());
	float tFromIntersectionToLight = shadowRay_->gett(lightPosition);
	IntersectionInfo returnVal = rayIntersection->closestObject(*shadowRay);
	Shape* shadowShape;
	if (returnVal.t < tFromIntersectionToLight)
	{
		return true;
	}
	/*
	for (int o = 0; o < objectCount; o++) {
		shadowShape = objects[o];
		returnVal = (shadowShape->intersect(*shadowRay));
		if (returnVal.isIntersect)
		{
			if (returnVal.t < tFromIntersectionToLight)
			{
				return true;
			}
		}
	}*/
	return false;
}
void Shading::calculateColor(IntersectionInfo& closestObjectInfo, Material material, PointLight* light, Vector3f lightVector, Vector3f cameraVectorNormalized, Vector3f& shaders) const
{
	Vector3f lightVectorNormalized = lightVector.normalizeVector();
	Vector3f intensity = light->computeLightContribution(closestObjectInfo.intersectionPoint);
	shaders = { 0,0,0 };
	Vector3f blinnPhongShade = blinnPhongShading(lightVectorNormalized, cameraVectorNormalized, material, intensity, closestObjectInfo.hitNormal);
	Vector3f diffuseShade = diffuseShading(lightVectorNormalized, material, intensity, closestObjectInfo.hitNormal);
	shaders = diffuseShade + blinnPhongShade;
}
Vector3f Shading::blinnPhongShading(Vector3f lightRayVector, Vector3f& cameraRayVector, Material& material, Vector3f& lightIntensity, Vector3f& normal)const
{
	Vector3f halfVector = (lightRayVector + cameraRayVector).normalizeVector();

	float cosAlpha = halfVector.dotProduct(normal);

	if (cosAlpha < 0) cosAlpha = 0;

	Vector3f blinnPhongLight = material.specularRef.scalarProduct(lightIntensity) * pow(cosAlpha, material.phongExp);

	return blinnPhongLight;

}
Vector3f Shading::diffuseShading(Vector3f lightRayVector, Material& material, Vector3f& lightIntensity, Vector3f& normal)const
{

	float cosTheta = lightRayVector.dotProduct(normal);
	if (cosTheta < 0) cosTheta = 0;

	Vector3f diffuse = material.diffuseRef.scalarProduct(lightIntensity) * cosTheta;

	return diffuse;

}

