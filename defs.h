#ifndef _DEFS_H_
#define _DEFS_H_
#include <iostream>
#include <cmath>

class Scene;
using namespace std;
/* Structure to hold return value from ray intersection routine.
This should hold information related to the intersection point,
for example, coordinate of the intersection point, surface normal at the intersection point etc.
Think about the variables you will need for this purpose and declare them here inside of this structure. */

/* 3 dimensional vector holding floating point numbers.
Used for both coordinates and color.
Use x, y, z for coordinate computations, and use r, g, b for color computations.
Note that you do not have to use this one if you use any vector computation library like Eigen. */
typedef struct Vector3f
{
	union
	{
		float x;
		float r;
	};
	union
	{
		float y;
		float g;
	};
	union
	{
		float z;
		float b;
	};
	 Vector3f operator=(Vector3f v2)
    {
        this->x = v2.x;
        this->y = v2.y;
        this->z = v2.z;
        return *this;
    }
    float dotProduct(const Vector3f v1) const
    {
        return x*v1.x+y*v1.y+z*v1.z;
    }
     float length() const
    {
        return sqrt(x*x+y*y+z*z);
    }
    Vector3f normalizeVector()const
    {

        float magnitude = sqrt(x*x+y*y+z*z);
        if(magnitude == 0)
        {
            Vector3f resultVector = {0,0,0}; 
             return resultVector;
        }
        else
        {
            Vector3f resultVector = {x/magnitude , y / magnitude, z/magnitude};

            return resultVector;
        }

    }
     Vector3f scalarProduct(Vector3f v2)const
     {
         Vector3f result = {x*v2.x , y*v2.y , z*v2.z};
         return result;

     }


} Vector3f;
inline Vector3f operator+(Vector3f v1,Vector3f v2)
    {
        Vector3f resultVector{v1.x+v2.x,v1.y+v2.y,v1.z+v2.z};
        return resultVector;
    }

inline Vector3f operator-(Vector3f v1,Vector3f v2)
    {
        Vector3f resultVector{v1.x-v2.x,v1.y-v2.y,v1.z-v2.z};
        return resultVector;
    }
inline Vector3f operator*( Vector3f  p, float f)
{
    Vector3f resultVector  ={f*p.x,f*p.y,f*p.z};

 return resultVector;
}
inline Vector3f operator*( Vector3f  u, Vector3f v)
{
    Vector3f resultVector  ={u.y*v.z - u.z*v.y,
                             u.z*v.x - u.x*v.z,
                             u.x*v.y - u.y*v.x};

 return resultVector;
}
inline Vector3f operator/(Vector3f v1,Vector3f v2)
    {
        Vector3f resultVector{v1.x/v2.x,v1.y/v2.y,v1.z/v2.z};
        return resultVector;
    }
inline Vector3f operator/(Vector3f v1,float f)
{
    Vector3f resultVector{v1.x/f,v1.y/f,v1.z/f};
    return resultVector;
}
inline ostream& operator<<(ostream& os, const Vector3f v  )
    {
        return os<<"x: " << v.x << " y: "<< v.y << " z: "<<v.z<<endl;

    }

typedef struct IntersectionInfo
{
    bool isIntersect;
    Vector3f intersectionPoint;
    float t;
    Vector3f hitNormal;
	int objectID;
} IntersectionInfo;


static float determinant(float matrix[3][3])
{
    float a = matrix[0][0];
    float b = matrix[1][0];
    float c = matrix[2][0];
    float d = matrix[0][1];
    float e = matrix[1][1];
    float f = matrix[2][1];
    float g = matrix[0][2];
    float h = matrix[1][2];
    float i = matrix[2][2];


        return a*(e*i-h*f) + b*(g*f-d*i)+c*(d*h-e*g);
}

//
// The global variable through which you can access the scene data
//
extern Scene* pScene;

#endif
