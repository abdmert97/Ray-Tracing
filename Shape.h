#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <vector>
#include "Ray.h"
#include "defs.h"
#include "BoundingBox.h"
#include "Material.h"
using namespace std;
enum ShapeType
{
	TriangleShape,
	SphereType,
	MeshType
	
};
class BoundingVolume;
class Node;
class BoundingBox;
// Base class for any shape object
class Shape
{
public:
	int id;	        // Id of the shape
	int matIndex;	// Material index of the shape
	ShapeType shapeType;
	BoundingBox* bounds;
	Material* material;
	vector<std::pair<char,int>> transformations;
	Ray applyTransform(Ray rayTransformed) const;
	glm::vec3 transformVector(glm::vec3& vect);
	virtual IntersectionInfo intersect(const Ray & ray) const = 0; // Pure virtual method for intersection test. You must implement this for sphere, triangle, and mesh.
	virtual BoundingBox* getBounds()= 0;
    Shape(void);
	Shape(int id, int matIndex, Material* material, ShapeType type, vector<std::pair<char, int>> transformations); // Constructor

private:

};

// Class for sphere
class Sphere: public Shape
{
public:
	
	Sphere(void);	// Constructor
	Sphere(int id, int matIndex, Material* material, int cIndex, float R, vector<glm::vec3> *vertices,ShapeType type, vector<std::pair<char, int>> transformations);	// Constructor
	
	IntersectionInfo intersect(const Ray & ray) const;	// Will take a ray and return a structure related to the intersection information. You will implement this.
	BoundingBox* getBounds();
private:
	glm::vec3 center;
	float radius;

};

// Class for triangle
class Triangle: public Shape
{
public:
	Triangle(void);	// Constructor
	Triangle(int id, int matIndex, Material* material, int p1Index, int p2Index, int p3Index, vector<glm::vec3> *vertices, ShapeType type, vector<std::pair<char, int>> transformations);	// Constructor
	IntersectionInfo intersect(const Ray & ray) const; // Will take a ray and return a structure related to the intersection information. You will implement this.
	BoundingBox* getBounds();
private:
    glm::vec3 point1;
    glm::vec3 point2;
    glm::vec3 point3;
};

// Class for mesh
class Mesh: public Shape
{
public:
	BoundingVolume* boundingVolume;
	Mesh(void);	// Constructor
	Mesh(int id, int matIndex, Material* material, const vector<Triangle>& faces, vector<int> *pIndices, vector<glm::vec3> *vertices, ShapeType type, vector<std::pair<char, int>> transformations);	// Constructor
	void MeshVolumeIntersection(const Ray& ray, Node* node, IntersectionInfo* intersecion_info) const;
	IntersectionInfo intersect(const Ray & ray) const; // Will take a ray and return a structure related to the intersection information. You will implement this.
	BoundingBox* getBounds() ;
private:
	 const vector<Triangle> faces;

};

#endif
