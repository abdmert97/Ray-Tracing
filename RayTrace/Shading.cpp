#include "Shading.h"

extern Scene* pScene;
glm::vec3 Shading::shading(int depth, Shape*& shape, IntersectionInfo& closestObjectInfo, Ray& ray,float n_t)
{
	// If depth is less then zero stop shading
	if (depth < 0)
	{
		glm::vec3 color = { 0,0,0 };
		return color;
	}
	
	Material material = *materials[shape->matIndex];

	bool isInside = glm::dot(ray.direction,closestObjectInfo.hitNormal) > 0;
	
	glm::vec3 color = { 0,0,0 };
	glm::vec3 cameraVectorNormalized = glm::normalize(ray.origin - closestObjectInfo.intersectionPoint);
	
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
	//yukar� al
	if (depth <= 0)
		return color;
	if (material.materialType == Default)
		return color;
	if (material.materialType == Mirror)
		getReflection(depth, closestObjectInfo, material, color, cameraVectorNormalized);
	else if (material.materialType == Conductor || material.materialType == Dialectic)
		refraction(depth, ray, closestObjectInfo, material, color,ray.direction,n_t);
	return color;
}
bool Shading::isShadow(glm::vec3& lightPosition, glm::vec3& intersectionPoint)const
{
	glm::vec3 lightVector = lightPosition - intersectionPoint;
	
	Ray shadowRay = Ray((intersectionPoint + normalize(lightVector) * shadowRayEps), normalize(lightVector));
	Ray shadowRay_ = Ray(intersectionPoint, normalize(lightVector));
	float tFromIntersectionToLight = shadowRay_.gett(lightPosition);
	
	IntersectionInfo returnVal = pScene->closestObject(shadowRay);
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
	glm::vec3 blinnPhongLight = glm::vec3(lightIntensity.x * material.specularRef.x*phong, lightIntensity.y * material.specularRef.y * phong, lightIntensity.z * material.specularRef.z * phong);

	return blinnPhongLight;

}
glm::vec3 Shading::diffuseShading(glm::vec3 lightRayVector, Material& material, glm::vec3& lightIntensity, glm::vec3& normal)const
{

	float cosTheta = dot(lightRayVector,normal);
	if (cosTheta < 0) cosTheta = 0;

	glm::vec3 diffuse =glm::vec3(lightIntensity* cosTheta * material.diffuseRef);

	return diffuse;

}
glm::vec3 Shading::reflect(const glm::vec3& incoming, const glm::vec3& normal)
{
	return incoming - normal * glm::dot(incoming, normal) * glm::vec3(2);
}
void Shading::getReflection(int depth, IntersectionInfo& intersectionInfo, Material material, glm::vec3& color, glm::vec3 cameraVectorNormalized)
{
	if (depth <= 0) return;
	if (material.materialType == Mirror || material.materialType == Conductor || material.materialType == Dialectic)
	{

		glm::vec3 reflectionDirection = normalize(reflect(cameraVectorNormalized * glm::vec3(-1), intersectionInfo.hitNormal));
		Ray reflectionRay = Ray(intersectionInfo.intersectionPoint + reflectionDirection * shadowRayEps, reflectionDirection);

		IntersectionInfo reflectionInfo = pScene->closestObject(reflectionRay);

		if (reflectionInfo.isIntersect)
		{
			Shape* reflectionShape = (*objects)[reflectionInfo.objectID];

			glm::vec3 reflectanceColor = { 0,0,0 };

			reflectanceColor = reflectanceColor + shading((depth - 1), reflectionShape, reflectionInfo, reflectionRay);

			glm::vec3 reflectance = {
				(reflectanceColor.x * material.mirrorRef.x),
				(reflectanceColor.y * material.mirrorRef.y),
				(reflectanceColor.z * material.mirrorRef.z) };

			color = color + reflectance;
		}
		else
		{
			glm::vec3 reflectanceColor = {
				(pScene->backgroundColor.r * material.mirrorRef.x),
				(pScene->backgroundColor.g * material.mirrorRef.y),
				(pScene->backgroundColor.b * material.mirrorRef.z) };
			color = color + reflectanceColor;
		}
	}
}
glm::vec3 Shading::refract(const glm::vec3& incoming, const glm::vec3& normal, const float& refractionIndex, float n_i)
{

	float cosi = glm::dot(incoming, normal);

	if (cosi < -1) cosi = -1;
	if (cosi > 1) cosi = 1;

	float etai = n_i, etat = refractionIndex;
	glm::vec3 n = normal;
	if (cosi < 0) { cosi = -cosi; }
	else { std::swap(etai, etat); n = normal * glm::vec3(-1); }
	float eta = etai / etat;

	float k = 1 - eta * eta * (1 - cosi * cosi);
	//return k < 0 ? glm::vec3{ 0,0,0 } : (incoming+n*cosi)*eta- n*sqrt(1-eta*eta*(1-cosi*cosi));
	return k < 0 ? glm::vec3{ 0,0,0 } : incoming * eta + n * (eta * cosi - sqrtf(k));
}
float Shading::fresnel(const glm::vec3& incoming, const glm::vec3& normal, const float& refractionIndex, Material material, float n_i)
{
	float kr = 0;

	float cosi = dot(incoming, normal);
	if (cosi < -1) cosi = -1;
	if (cosi > 1) cosi = 1;
	float  n_t = refractionIndex;
	if (cosi > 0) { std::swap(n_i, n_t); }

	// Compute sini using Snell's law
	float sint = (n_i / n_t) * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection

	float cost = sqrtf(std::max(0.f, 1 - sint * sint));
	cosi = fabsf(cosi);

	float Rs = material.materialType == Dialectic ?
		((n_t * cosi) - (n_i * cost)) / ((n_t * cosi) + (n_i * cost)) :
		(n_t * n_t + material.absorptionIndex * material.absorptionIndex - 2 * n_t * cosi + 1) /
		(n_t * n_t + material.absorptionIndex * material.absorptionIndex + 2 * n_t * cosi + 1);
	float Rp = material.materialType == Dialectic ?
		((n_i * cosi) - (n_t * cost)) / ((n_i * cosi) + (n_t * cost)) :
		(n_t * n_t + material.absorptionIndex * material.absorptionIndex * cosi * cosi - 2 * n_t * cosi + 1) /
		(n_t * n_t + material.absorptionIndex * material.absorptionIndex * cosi * cosi + 2 * n_t * cosi + 1);

	//	cout << Rs << " " << Rp << endl;

	if (material.materialType == Dialectic)
	{
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
	else
	{
		kr = (Rs + Rp) / 2;

	}
	return kr;
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
}


void Shading::refraction(int depth, Ray ray, IntersectionInfo& intersection, Material material, glm::vec3& color, glm::vec3 rayDirection, float n_i)
{
	glm::vec3 refractionColor{ 0,0,0 };
	float n_t = material.refractionIndex;

	float kr = fresnel(rayDirection, intersection.hitNormal, n_t, material, n_i);
	//kt transmit



	if (material.materialType == Dialectic && kr < 1) {

		glm::vec3 hitPoint = intersection.intersectionPoint;
		bool outside = glm::dot(ray.direction, intersection.hitNormal) < 0;
		glm::vec3 bias = intersection.hitNormal * glm::vec3(0.01);
		glm::vec3 refractionDirection = normalize(refract(rayDirection, intersection.hitNormal, n_t, n_i));
		glm::vec3 refractionRayOrig = outside ? hitPoint - bias : hitPoint + bias;

		Shape* intersectShape = (*objects)[intersection.objectID];
		Ray reflectionRay = Ray(refractionRayOrig, refractionDirection);
		IntersectionInfo refractionIntersect = pScene->closestObject(reflectionRay);
		if (refractionIntersect.isIntersect)
		{
			Shape* refractionShape = (*objects)[refractionIntersect.objectID];

			refractionColor = shading(depth - 1, refractionShape, refractionIntersect, reflectionRay, n_i);

		}

	}




	if (material.materialType == Dialectic)
	{
		glm::vec3 abs = material.absorptionCoefficient;

		material.mirrorRef = { 1,1,1 };
	}


	glm::vec3 reflectionColor = { 0,0,0 };
	glm::vec3 cameraVector = glm::normalize(ray.origin - intersection.intersectionPoint);
	if (depth > 0)
		getReflection(depth, intersection, material, reflectionColor, cameraVector);


	// mix the two
	if (material.materialType == Dialectic)
		color = color + reflectionColor * kr + refractionColor * (1 - kr);
	else
		color = color + reflectionColor * kr;
}