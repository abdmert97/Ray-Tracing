#include "Material.h"

Material::Material(void)
{
	phongExp = 0;
	
	Vector3f ambientRef = {0,0,0};	// Coefficients for ambient reflection
	Vector3f diffuseRef = { 0,0,0 };	// Coefficients for diffuse reflection
	Vector3f specularRef = { 0,0,0 };	// Coefficients for specular reflection
	if(materialType == Default)
		Vector3f mirrorRef = { 0,0,0 };		// Coefficients for mirror reflection
	else
		Vector3f mirrorRef = { 1,1,1 };
	refractionIndex = 1;
}
