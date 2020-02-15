
#include "Reflection.h"

void Reflection::getReflection(int depth, ReturnVal& closestObjectReturnVal, Material material, Color& color, Vector3f cameraVectorNormalized)
{
	if (material.mirrorRef.length() > 0.0)
	{
		Vector3f reflectionDirection = (cameraVectorNormalized * -1 + closestObjectReturnVal.hitNormal * (closestObjectReturnVal.hitNormal.dotProduct(cameraVectorNormalized)) * 2).normalizeVector();
		Ray* reflectionRay = new Ray(closestObjectReturnVal.intersectionPoint + reflectionDirection * shadowRayEps, reflectionDirection);

		ReturnVal closestReflection = rayIntersection->closestObject(*reflectionRay);

		if (closestReflection.isIntersect)
		{
			Shape* reflectionShape = objects[closestReflection.objectID];
			Vector3f reflectance = material.mirrorRef;
			Color reflectanceColor = { 0,0,0 };

			reflectanceColor = reflectanceColor + shading->shading((depth - 1), reflectionShape, closestReflection, *reflectionRay);

			reflectance = { reflectanceColor.red * reflectance.x,
				reflectanceColor.grn * reflectance.y,
				reflectanceColor.blu * reflectance.z };

			color = color + reflectance;
		}
		else
		{

			Color reflectanceColor = { (unsigned char)(backgroundColor.r * material.mirrorRef.x),
				(unsigned char)(backgroundColor.g * material.mirrorRef.y),
				(unsigned char)(backgroundColor.b * material.mirrorRef.z) };
			color = color + reflectanceColor;
		}
	}
}
