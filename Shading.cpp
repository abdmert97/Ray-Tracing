#include "Shading.h"
#include "Light.h"
#include "Scene.h"


vec3 Shading::shading(int depth, Shape*& shape, IntersectionInfo& closestObjectInfo, Ray& ray,float n_t)
{
	// If depth is less then zero stop shading
	if (depth < 0)
	{
		vec3 color = { 0,0,0 };
		return color;
	}
	
	Material material = *materials[shape->matIndex];

	bool isInside = dot(ray.direction,closestObjectInfo.hitNormal) > 0;
	
	vec3 color = { 0,0,0 };
	glm::vec3 cameraVectorNormalized = normalize(ray.origin - closestObjectInfo.intersectionPoint);
	
	if(!isInside)
	{
		color = ambientLightList[shape->matIndex];

		PointLight* light;
		glm::vec3 lightVector;
		
		for (int l = 0; l < lightCount; l++)
		{
			light = lights[l];
			lightVector = (light->position - closestObjectInfo.intersectionPoint);
			// Shadows
			
			if (isShadow(light->position, closestObjectInfo.intersectionPoint))
			{
				continue;
			}
			// Shading
			glm::vec3 shaders;
			
			calculateColor(closestObjectInfo, material, light, lightVector, cameraVectorNormalized, shaders);
			color = color + shaders;

		}
	}
	//yukarý al
	if (depth <= 0)
		return color;
	if (material.materialType == Default)
		return color;
	if (material.materialType == Mirror)
		reflection->getReflection(depth, closestObjectInfo, material, color, cameraVectorNormalized);
	else if (material.materialType == Conductor || material.materialType == Dialectic)
		refraction->refraction(depth, ray, closestObjectInfo, material, color,ray.direction,n_t);
	return color;
}
bool Shading::isShadow(glm::vec3& lightPosition, glm::vec3& intersectionPoint)const
{
	glm::vec3 lightVector = lightPosition - intersectionPoint;
	
	Ray shadowRay = Ray((intersectionPoint + normalize(lightVector) * shadowRayEps), normalize(lightVector));
	Ray shadowRay_ = Ray(intersectionPoint, normalize(lightVector));
	float tFromIntersectionToLight = shadowRay_.gett(lightPosition);
	
	IntersectionInfo returnVal = rayIntersection->closestObject(shadowRay);
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
void Shading::calculateColor(IntersectionInfo& closestObjectInfo, Material material, PointLight* light, glm::vec3 lightVector, glm::vec3 cameraVectorNormalized, glm::vec3& shaders) const
{
	glm::vec3 lightVectorNormalized = normalize(lightVector);
	glm::vec3 intensity = light->computeLightContribution(closestObjectInfo.intersectionPoint);
	shaders = { 0,0,0 };
	glm::vec3 blinnPhongShade = blinnPhongShading(lightVectorNormalized, cameraVectorNormalized, material, intensity, closestObjectInfo.hitNormal);
	glm::vec3 diffuseShade = diffuseShading(lightVectorNormalized, material, intensity, closestObjectInfo.hitNormal);
	shaders = diffuseShade + blinnPhongShade;


}
glm::vec3 Shading::blinnPhongShading(glm::vec3 lightRayVector, glm::vec3& cameraRayVector, Material& material, glm::vec3& lightIntensity, glm::vec3& normal)const
{

	glm::vec3 halfVector = normalize(lightRayVector + cameraRayVector);
	
	float cosAlpha = glm::dot( halfVector,normalize(normal));

	if (cosAlpha < 0) cosAlpha = 0;
	float phong = pow(cosAlpha, material.phongExp);
	glm::vec3 blinnPhongLight = vec3(lightIntensity.x * material.specularRef.x*phong, lightIntensity.y * material.specularRef.y * phong, lightIntensity.z * material.specularRef.z * phong);

	return blinnPhongLight;

}
glm::vec3 Shading::diffuseShading(glm::vec3 lightRayVector, Material& material, glm::vec3& lightIntensity, glm::vec3& normal)const
{

	float cosTheta = dot(lightRayVector,normal);
	if (cosTheta < 0) cosTheta = 0;

	glm::vec3 diffuse =vec3(lightIntensity* cosTheta * material.diffuseRef);

	return diffuse;

}

