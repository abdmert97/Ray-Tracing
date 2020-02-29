#include "Shape.h"
#include "Scene.h"
#include <cstdio>
#include <cmath>

BoundingBox* Shape::getBounds()
{
	return new BoundingBox;
}

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex, ShapeType type)
    : id(id), matIndex(matIndex) ,shapeType(type)
{
	bounds = nullptr;
}

Sphere::Sphere(void)
{}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *pVertices, ShapeType type)
    : Shape(id, matIndex,type)
{
this->radius = R;
this->center = (*pVertices)[cIndex-1];

}

/* Sphere-ray intersection routine. You will implement this.
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc.
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray & ray) const
{
    ReturnVal returnValue = {};
    returnValue.isIntersect = false;
    Vector3f direction = ray.direction;
    Vector3f origin = ray.origin;
    Vector3f center = this->center;

    float t = (direction*-1).dotProduct(origin-center);
    //dummy variables
    float x =  direction.dotProduct(origin-center);
    float y =  direction.dotProduct(direction);
    float z =  (origin-center).dotProduct(origin-center);
    float delta = x*x-y*(z-radius*radius);

    if(delta < 0 || t <= 0)
        return returnValue;

    returnValue.isIntersect = true;
    //float intersectionPoint1 = t + sqrt(delta);
    float intersectionPoint2 = t - sqrt(delta);
	//float intersectionPoint = intersectionPoint1 < intersectionPoint2 ? intersectionPoint1 : intersectionPoint2;
    returnValue.intersectionPoint =ray.getPoint(intersectionPoint2);
	returnValue.t = intersectionPoint2;
    returnValue.hitNormal = (ray.getPoint(intersectionPoint2) -center )/(this->radius);


//    cout <<ray.getPoint(intersectionPoint1)<<" " << ray.getPoint(intersectionPoint2)<<endl ;
    return returnValue;
}

BoundingBox* Sphere::getBounds()
{
	if(bounds == nullptr)
	{
		bounds = new BoundingBox();
		Vector3f radiusVec = { radius,radius,radius };
		bounds->min = center - radiusVec;
		bounds->max = center + radiusVec;
	}
	return bounds;
}
BoundingBox* Triangle::getBounds()
{
	if (bounds == nullptr)
	{
		bounds = new BoundingBox();
		bounds->min = BoundingBox::getMin(point1, BoundingBox::getMin(point2, point3));
		bounds->max = BoundingBox::getMax(point1, BoundingBox::getMax(point2, point3));
	}
	return bounds;
}
BoundingBox* Mesh::getBounds()
{
	if (bounds == nullptr)
	{
		bounds = new BoundingBox();
		for (Triangle triangle : faces)
		{
			BoundingBox* triangleBounds = triangle.getBounds();
			bounds->min = BoundingBox::getMin(bounds->min, triangleBounds->min);
			bounds->max = BoundingBox::getMax(bounds->max, triangleBounds->max);
		}
	}
	return bounds;
}

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices, ShapeType type)
    : Shape(id, matIndex,type)
{
    point1 = pVertices[0][p1Index-1];
    point2 = pVertices[0][p2Index-1];
    point3 = pVertices[0][p3Index-1];
}

/* Triangle-ray intersection routine. You will implement this.
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc.
You should to declare the variables  in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
    ReturnVal returnValue ={};
    returnValue.isIntersect = false;
    Vector3f rayDirection = ray.direction;
    Vector3f rayOrigin = ray.origin;
    float AMatrix[3][3] = {

            {point1.x-point2.x, point1.x-point3.x, rayDirection.x},
            {point1.y-point2.y, point1.y-point3.y, rayDirection.y},
            {point1.z-point2.z, point1.z-point3.z, rayDirection.z}
    };

    float betaMatrix[3][3] = {
            {point1.x-rayOrigin.x, point1.x-point3.x, rayDirection.x},
            {point1.y-rayOrigin.y, point1.y-point3.y, rayDirection.y},
            {point1.z-rayOrigin.z, point1.z-point3.z, rayDirection.z}

    };

    float gammaMatrix[3][3] = {
            {point1.x-point2.x, point1.x-rayOrigin.x, rayDirection.x},
            {point1.y-point2.y, point1.y-rayOrigin.y, rayDirection.y},
            {point1.z-point2.z, point1.z-rayOrigin.z, rayDirection.z}

    };

    float tMatrix[3][3] = {
            {point1.x-point2.x, point1.x-point3.x, point1.x-rayOrigin.x},
            {point1.y-point2.y, point1.y-point3.y, point1.y-rayOrigin.y},
            {point1.z-point2.z, point1.z-point3.z, point1.z-rayOrigin.z}

    };
    float determinantA = determinant(AMatrix);
    if(determinantA == 0)
    {
        returnValue.isIntersect = false; // maybe wrong;
        return returnValue;
    }

    float beta = determinant(betaMatrix)/determinantA;
    float gamma = determinant(gammaMatrix)/determinantA;
    float t = determinant(tMatrix)/determinantA;
    if(beta+gamma<=1&&beta>=0&&gamma>=0&&t>0)
    {
        returnValue.isIntersect = true;
        returnValue.t = t;
        returnValue.intersectionPoint = ray.getPoint(t);
    }
    Vector3f crossProduct =(point2-point1)*(point3-point1);
    returnValue.hitNormal =  crossProduct.normalizeVector();

    return returnValue;
}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices, ShapeType type)
    : Shape(id, matIndex,type),faces(faces)
{

}

/* Mesh-ray intersection routine. You will implement this.
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc.
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
    int size = faces.size();

	ReturnVal returnVal = {};
	returnVal.isIntersect = false;
	Vector3f cameraPosition = ray.getPoint(0);
	float cameraDistance = INT_MAX;
    for(int i = 0 ; i < size;i++)
    {
        ReturnVal tempReturnVal= faces[i].intersect(ray);

		if (tempReturnVal.isIntersect)
		{
			// Select closest of faces intersecting
			Vector3f cameraDistanceVector = tempReturnVal.intersectionPoint - cameraPosition;
			if(cameraDistanceVector.length() < cameraDistance){
				cameraDistance = cameraDistanceVector.length();
				returnVal = tempReturnVal;
			}
		}
    }

    return returnVal;
}
