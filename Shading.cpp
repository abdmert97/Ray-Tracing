#include "Shading.h"
#include "Light.h"
#include "Scene.h"
int shadowcount = 0;

Color Shading::shading(int depth, Shape*& shape, ReturnVal& closestObjectReturnVal, Ray& ray,float n_t)
{
	if (depth < 0)
	{
		Color color = { 0,0,0 };
		return color;
	}
	shadowcount++;
	Material material = *materials[shape->matIndex - 1];
	// ambient diffuse specular yapma obje i�indeyse
	// ray ve normal ayn� y�ndeyse yapma
	bool isInside = ray.direction.dotProduct(closestObjectReturnVal.hitNormal) > 0;
	
	Color color = { 0,0,0 };
	Vector3f cameraVector = (ray.origin - closestObjectReturnVal.intersectionPoint);
	Vector3f cameraVectorNormalized = cameraVector.normalizeVector();
	if(!isInside)
	{
		color = ambientLightList[shape->matIndex - 1];


		PointLight* light;
		Vector3f lightVector;
		
	
		for (int l = 0; l < lightCount; l++)
		{
			light = lights[l];
			lightVector = (light->position - closestObjectReturnVal.intersectionPoint);
			// Shadows
			Vector3f lightPosition = light->position;
			if (isShadow(lightPosition, closestObjectReturnVal.intersectionPoint))
			{
				continue;
			}
			// Shading
			Vector3f shaders;
			
			calculateColor(closestObjectReturnVal, material, light, lightVector, cameraVectorNormalized, shaders);
			color = color + shaders;
		}
	}
	

	// Reflection
	if (material.materialType == Default)
		return color;
	if (material.materialType == Mirror)
		reflection->getReflection(depth, closestObjectReturnVal, material, color, cameraVectorNormalized);
	else
		refraction->refraction(depth, ray, closestObjectReturnVal, material, color,ray.direction,n_t);
	return color;
}
bool Shading::isShadow(Vector3f& lightPosition, Vector3f& intersectionPoint)const
{
	Vector3f lightVector = lightPosition - intersectionPoint;
	Ray* shadowRay = new Ray((intersectionPoint + lightVector.normalizeVector() * shadowRayEps), lightVector.normalizeVector());
	Ray* shadowRay_ = new Ray(intersectionPoint, lightVector.normalizeVector());
	float tFromIntersectionToLight = shadowRay_->gett(lightPosition);
	ReturnVal returnVal;
	Shape* shadowShape;
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
	}
	return false;
}
void Shading::calculateColor(ReturnVal& closestObjectReturnVal, Material material, PointLight* light, Vector3f lightVector, Vector3f cameraVectorNormalized, Vector3f& shaders) const
{
	Vector3f lightVectorNormalized = lightVector.normalizeVector();
	Vector3f intensity = light->computeLightContribution(closestObjectReturnVal.intersectionPoint);
	shaders = { 0,0,0 };
	Vector3f blinnPhongShade = blinnPhongShading(lightVectorNormalized, cameraVectorNormalized, material, intensity, closestObjectReturnVal.hitNormal);
	Vector3f diffuseShade = diffuseShading(lightVectorNormalized, material, intensity, closestObjectReturnVal.hitNormal);
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

