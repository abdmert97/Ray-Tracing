
#include "Reflection.h"
#include "RayTracing/glm/detail/func_geometric.inl"

glm::vec3 Reflection::reflect(const glm::vec3& incoming, const glm::vec3& normal)
{
	return incoming - normal * glm::dot(incoming,normal) * glm::vec3(2);
}
void Reflection::getReflection(int depth, IntersectionInfo& intersectionInfo, Material material, glm::vec3& color, glm::vec3 cameraVectorNormalized)
{
	if(depth <=0) return;
	if (material.materialType == Mirror ||material.materialType == Conductor || material.materialType == Dialectic)
	{
		
		glm::vec3 reflectionDirection = normalize(reflect(cameraVectorNormalized * glm::vec3(-1), intersectionInfo.hitNormal));
		Ray reflectionRay =Ray(intersectionInfo.intersectionPoint + reflectionDirection * shadowRayEps, reflectionDirection);

		IntersectionInfo reflectionInfo = rayIntersection->closestObject(reflectionRay);

		if (reflectionInfo.isIntersect)
		{
			Shape* reflectionShape = (*objects)[reflectionInfo.objectID];

			glm::vec3 reflectanceColor = { 0,0,0 };

			reflectanceColor = reflectanceColor + shading->shading((depth - 1), reflectionShape, reflectionInfo, reflectionRay);

			glm::vec3 reflectance = {
				(reflectanceColor.x * material.mirrorRef.x),
				(reflectanceColor.y * material.mirrorRef.y),
				(reflectanceColor.z * material.mirrorRef.z) };
				
			color = color + reflectance;
		}
		else
		{
			glm::vec3 reflectanceColor = {
				(backgroundColor.r * material.mirrorRef.x),
				(backgroundColor.g * material.mirrorRef.y),
				(backgroundColor.b * material.mirrorRef.z) };
			color = color + reflectanceColor;
		}
	}
}
