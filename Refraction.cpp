
#include "Refraction.h"
#include <algorithm>

Vector3f Refraction::refract(const Vector3f& incoming, const Vector3f& normal, const float& ior)
{
	float cosi = incoming.dotProduct(normal);
	float n_i = 1, n_t = ior;
	Vector3f n = normal;
	if (cosi < 0) { cosi = -cosi; }
	else { std::swap(n_i, n_t); n = normal*-1; }
	float eta = n_i / n_t;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0 ? Vector3f{0,0,0} : incoming*eta + n*(eta * cosi - sqrtf(k));
}
float Refraction::fresnel(const Vector3f& incoming, const Vector3f& normal, const float& refractionIndex,Material material)
{
	float kr = 0;
	float cosi = incoming.dotProduct(normal);
	if (cosi < -1) cosi = -1;
	if (cosi > 1) cosi = 1;
	float n_i = 1, n_t = refractionIndex;
	if (cosi > 0) { std::swap(n_i, n_t); }
	// Compute sini using Snell's law
	float sint = n_i / n_t * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
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
			kr = (Rs * Rs + Rp * Rp) / 2;
		else
			kr = (Rs + Rp) / 2;
	}

	return kr;
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
}
Vector3f Refraction::reflect(const Vector3f& incoming, const Vector3f& normal)
{
	return incoming -normal* incoming.dotProduct(normal) * 2 ;
}




void Refraction::refraction(int depth, Ray ray,ReturnVal intersection,Material material, Color& color,Vector3f incoming)
{
	Color refractionColor{0,0,0};
	
	// compute fresnel
	float kr = fresnel(incoming, intersection.hitNormal, material.refractionIndex,material);
	
	bool outside = ray.direction.dotProduct(intersection.hitNormal) < 0;
	Vector3f bias = intersection.hitNormal*0.001;
	// compute refraction if it is not a case of total internal reflection
	Vector3f hitPoint = ray.getPoint(intersection.t);
	if (kr < 1) {
		
		Vector3f refractionDirection = refract(incoming, intersection.hitNormal, material.refractionIndex).normalizeVector();
		Vector3f refractionRayOrig = outside ? hitPoint - bias : hitPoint + bias;
		
		Ray* reflectionRay = new Ray(refractionRayOrig, refractionDirection);
		ReturnVal refractionIntersect = rayIntersection->closestObject(*reflectionRay);
		if(refractionIntersect.isIntersect)
		{
			Shape* reflectionShape = objects[refractionIntersect.objectID];

			refractionColor = shading->shading(depth - 1, reflectionShape, refractionIntersect, *reflectionRay);
			//refractionColor = castRay(refractionRayOrig, refractionDirection, objects, lights, options, depth + 1);
		}
		else
		{
			refractionColor = { (unsigned char)(reflection->backgroundColor.r * material.mirrorRef.x),
				(unsigned char)(reflection->backgroundColor.g * material.mirrorRef.y),
				(unsigned char)(reflection->backgroundColor.b * material.mirrorRef.z) };
		}
		
	}

	Vector3f reflectionDirection = reflect(incoming, intersection.hitNormal).normalizeVector();
	Vector3f reflectionRayOrig = outside ? hitPoint + bias : hitPoint - bias;
	Color reflectionColor = {0,0,0};// = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options, depth + 1);
	if (material.materialType == Dialectic)
	{
		Vector3f abs = material.absorptionCoefficient;
		
		material.mirrorRef = {kr-abs.r,kr-abs.g,kr-abs.b};
	}
	
	reflection->getReflection(depth - 1, intersection, material, reflectionColor, incoming);

	// mix the two
	
	color = color + reflectionColor * kr  + refractionColor * (1 - kr);
	
}
void Refraction::printColor(const Color& color)
{
	cout << "red: " << (int)color.red << " green: " << (int)color.grn << " blue: " << (int)color.blu << endl;
}


