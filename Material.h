#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "defs.h"
enum MaterialType
{
	Default,
	Mirror,
	Conductor,
	Dialectic
};
// Class to hold variables related to a material
class Material
{
public:
	int id;	// Material id
	MaterialType materialType;
	int phongExp;	// Phong exponent
	Vector3f ambientRef;	// Coefficients for ambient reflection
	Vector3f diffuseRef;	// Coefficients for diffuse reflection
	Vector3f specularRef;	// Coefficients for specular reflection
	Vector3f mirrorRef;		// Coefficients for mirror reflection
	Vector3f absorptionCoefficient;
	float absorptionIndex;
	float refractionIndex;
	Material(void);	// Constructor
	
private:
	// Write any other stuff here
};

#endif
