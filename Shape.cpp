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

Shape::Shape(int id, int matIndex,Material * material, ShapeType type)
    : id(id), matIndex(matIndex) ,shapeType(type),material(material)
{
	bounds = nullptr;
}

Sphere::Sphere(void)
{}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, Material* material, int cIndex, float R, vector<Vector3f> *pVertices, ShapeType type)
    : Shape(id, matIndex,material,type)
{
this->radius = R;
this->center = (*pVertices)[cIndex-1];
getBounds();
}

/* Sphere-ray intersection routine. You will implement this.
Note that IntersectionInfo structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc.
You should to declare the variables in IntersectionInfo structure you think you will need. It is in defs.h file. */
IntersectionInfo Sphere::intersect(const Ray & ray) const
{
    IntersectionInfo returnValue = {};
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

    if(delta < 0 || t + sqrt(delta) <= 0)
        return returnValue;

    
    float intersectionPoint1 = t + sqrt(delta);
    float intersectionPoint2 = t - sqrt(delta);
	float intersectionPoint = intersectionPoint2 > 0 ? intersectionPoint2 : intersectionPoint1;
	//float intersectionPoint = intersectionPoint1 < intersectionPoint2 ? intersectionPoint1 : intersectionPoint2;
	returnValue.isIntersect = true;
    returnValue.intersectionPoint =ray.getPoint(intersectionPoint);
	returnValue.t = intersectionPoint;
	returnValue.objectID = id ;
    returnValue.hitNormal = (ray.getPoint(intersectionPoint) - center )/(this->radius);
	

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
		boundingVolume = new BoundingVolume(faces);
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
Triangle::Triangle(int id, int matIndex, Material* material, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices, ShapeType type)
    : Shape(id, matIndex,material,type)
{
    point1 = pVertices[0][p1Index-1];
    point2 = pVertices[0][p2Index-1];
    point3 = pVertices[0][p3Index-1];
	getBounds();
}

/* Triangle-ray intersection routine. You will implement this.
Note that IntersectionInfo structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc.
You should to declare the variables  in IntersectionInfo structure you think you will need. It is in defs.h file. */
IntersectionInfo Triangle::intersect(const Ray & ray) const
{
    IntersectionInfo returnValue ={};
    returnValue.isIntersect = false;
	returnValue.objectID = -1;
    Vector3f rayDirection = ray.direction;
    Vector3f rayOrigin = ray.origin;
    float AMatrix[3][3] = {

            {point1.x-point2.x, point1.x-point3.x, rayDirection.x},
            {point1.y-point2.y, point1.y-point3.y, rayDirection.y},
            {point1.z-point2.z, point1.z-point3.z, rayDirection.z}
    };
	float determinantA = determinant(AMatrix);
	if (determinantA == 0)
	{
		returnValue.isIntersect = false; // maybe wrong;
		return returnValue;
	}
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
 

    float beta = determinant(betaMatrix)/determinantA;
    float gamma = determinant(gammaMatrix)/determinantA;
    float t = determinant(tMatrix)/determinantA;
    if(beta+gamma<=1&&beta>=0&&gamma>=0&&t>0.001)
    {
		returnValue.objectID = id;
        returnValue.isIntersect = true;
        returnValue.t = t;
        returnValue.intersectionPoint = ray.getPoint(t);
		Vector3f crossProduct = (point2 - point1) * (point3 - point1);
		returnValue.hitNormal = crossProduct.normalizeVector();
    }


    return returnValue;
}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, Material* material, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices, ShapeType type)
    : Shape(id, matIndex,material,type),faces(faces)
{
	for (Triangle triangle : faces)
		triangle.getBounds();
	getBounds();
}

void Mesh::MeshVolumeIntersection(const Ray& ray, Node* node, IntersectionInfo* intersecion_info) const
{
	float t_min = intersecion_info->t;
	short t_intersection = node->boundingBox->isIntersect(ray);

	if (node->boundingBox->isIntersect(ray) != -1)
	{
		if (node->left != nullptr)
		{
			MeshVolumeIntersection(ray, node->left, intersecion_info);
		}
		if (node->right != nullptr)
		{
			MeshVolumeIntersection(ray, node->right, intersecion_info);
		}

		if (node->left == nullptr || node->right == nullptr)
		{
			const Triangle* shape = &faces[node->ObjectIDs[0]];

			if (shape->bounds->isIntersect(ray) != -1)
			{
				IntersectionInfo intesectionInfo = shape->intersect(ray);
				if (intesectionInfo.isIntersect == true)
				{
					if (intesectionInfo.t <= t_min)
					{
						t_min = intesectionInfo.t;
						intesectionInfo.objectID = node->ObjectIDs[0];
						*intersecion_info = intesectionInfo;
					}

				}
			}
		}
	}

}

/* Mesh-ray intersection routine. You will implement this.
Note that IntersectionInfo structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc.
You should to declare the variables in IntersectionInfo structure you think you will need. It is in defs.h file. */
IntersectionInfo Mesh::intersect(const Ray & ray) const
{

	IntersectionInfo returnValue{};

	float t = 9999;
	returnValue.t = t;
	Node* node = boundingVolume->root;
	
	MeshVolumeIntersection(ray, node, &returnValue);



	return  returnValue;

	

}
