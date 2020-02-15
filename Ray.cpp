#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction)
    : origin(origin), direction(direction)
{
}

/* Takes a parameter t and returns the point accoring to t. t is the parametric variable in the ray equation o+t*d.*/
Vector3f Ray::getPoint(float t) const
{
	return origin+ direction*t;
}

/* Takes a point p and returns the parameter t according to p such that p = o+t*d. */
float Ray::gett(const Vector3f & p) const
{
     Vector3f difference = p-origin ; // t*d
     float t = 0;
     if(difference.x != 0)
        t= difference.x / direction.x;
     else if(difference.y != 0)
        t= difference.y / direction.y;
     else if(difference.z != 0)
        t= difference.z / direction.z;
     return t;
// ifler kaldýrýlabilir
}

