
#include "Reflection.h"
Vector3f Reflection::reflect(const Vector3f& incoming, const Vector3f& normal)
{
	return incoming - normal * incoming.dotProduct(normal) * 2;
}
void Reflection::getReflection(int depth, IntersectionInfo& intersectionInfo, Material material, Color& color, Vector3f cameraVectorNormalized)
{
	if (material.materialType != Default)
	{
		
		Vector3f reflectionDirection = reflect(cameraVectorNormalized * -1, intersectionInfo.hitNormal).normalizeVector();
		Ray* reflectionRay = new Ray(intersectionInfo.intersectionPoint + reflectionDirection * shadowRayEps, reflectionDirection);

		IntersectionInfo reflectionInfo = rayIntersection->closestObject(*reflectionRay);

		if (reflectionInfo.isIntersect)
		{
			Shape* reflectionShape = objects[reflectionInfo.objectID];
		
			Color reflectanceColor = { 0,0,0 };

			reflectanceColor = reflectanceColor + shading->shading((depth - 1), reflectionShape, reflectionInfo, *reflectionRay);
			
			Color reflectance = {
				(reflectanceColor.red * material.mirrorRef.x),
				(reflectanceColor.grn * material.mirrorRef.y),
				(reflectanceColor.blu * material.mirrorRef.z) };
				
			color = color + reflectance;
		}
		else
		{
			Color reflectanceColor = {
				(backgroundColor.r * material.mirrorRef.x),
				(backgroundColor.g * material.mirrorRef.y),
				(backgroundColor.b * material.mirrorRef.z) };
			color = color + reflectanceColor;
		}
	}
}
