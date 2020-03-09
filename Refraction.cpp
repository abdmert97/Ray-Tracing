
#include "Refraction.h"
#include <algorithm>
											
Vector3f Refraction::refract(const Vector3f& incoming, const Vector3f& normal, const float& refractionIndex,float n_i)
{
	
	float cosi =  incoming.dotProduct(normal);

	if (cosi < -1) cosi = -1;
	if (cosi > 1) cosi = 1;

	float etai = n_i, etat = refractionIndex;
	Vector3f n = normal;
	if (cosi < 0) { cosi = -cosi; }
	else { std::swap(etai, etat); n = normal*-1; }
	float eta = etai / etat;

	float k = 1 - eta * eta * (1 - cosi * cosi);
	//return k < 0 ? Vector3f{ 0,0,0 } : (incoming+n*cosi)*eta- n*sqrt(1-eta*eta*(1-cosi*cosi));
	return k < 0 ? Vector3f{ 0,0,0 } : incoming*eta + n*(eta * cosi - sqrtf(k));
}
float Refraction::fresnel(const Vector3f& incoming, const Vector3f& normal, const float& refractionIndex,Material material,float n_i)
{
	float kr = 0;

	float cosi = incoming.dotProduct(normal);
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
Vector3f Refraction::reflect(const Vector3f& incoming, const Vector3f& normal)
{
	return incoming - normal* incoming.dotProduct(normal) * 2 ;
}


void Refraction::refraction(int depth, Ray ray,IntersectionInfo& intersection,Material material, Color& color,Vector3f rayDirection,float n_i)
{
	
	Color refractionColor{ 0,0,0 };
	float n_t =material.refractionIndex;
	
	float kr = fresnel(rayDirection, intersection.hitNormal, n_t,material,n_i);
	//kt transmit

	

	if (material.materialType == Dialectic && kr < 1 ) {
		
		Vector3f hitPoint = intersection.intersectionPoint;
		bool outside = ray.direction.dotProduct(intersection.hitNormal) < 0;
		Vector3f bias = intersection.hitNormal * 0.01;
		Vector3f refractionDirection = refract(rayDirection, intersection.hitNormal,n_t,n_i).normalizeVector();
		Vector3f refractionRayOrig = outside ? hitPoint - bias : hitPoint + bias;

		Shape* intersectShape = (*objects)[intersection.objectID];
		Ray reflectionRay = Ray(refractionRayOrig, refractionDirection);
		IntersectionInfo refractionIntersect = rayIntersection->closestObject(reflectionRay);
		if(refractionIntersect.isIntersect)
		{
			Shape* refractionShape = (*objects)[refractionIntersect.objectID];
			
			refractionColor = shading->shading(depth - 1, refractionShape, refractionIntersect, reflectionRay,n_i);
			
		}
		
	}




	if (material.materialType == Dialectic)
	{
		Vector3f abs = material.absorptionCoefficient;

		material.mirrorRef = { 1,1,1 };
	}

	
	Color reflectionColor = { 0,0,0 };
	Vector3f cameraVector = (ray.origin - intersection.intersectionPoint).normalizeVector();
	reflection->getReflection(depth, intersection, material, reflectionColor, cameraVector);

	// mix the two
	if (material.materialType == Dialectic)
		color = color + reflectionColor * kr + refractionColor * (1 - kr);
	else
		color = color + reflectionColor * kr;
}



void Refraction::printColor(const Color& color)
{
	cout << "red: " << (int)color.red << " green: " << (int)color.grn << " blue: " << (int)color.blu << endl;
}


