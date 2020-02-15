#include "Light.h"

/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3f PointLight::computeLightContribution(const Vector3f& p)
{

	Vector3f distance = p-position;
    float distanceSquare =  distance.x*distance.x+
                            distance.y*distance.y+
                            distance.z*distance.z;

    return intensity*(1/distanceSquare);
}
