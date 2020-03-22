#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Ray.h"
#include "defs.h"
#include "Image.h"
#include "Shading.h"
#include "RayIntersection.h"
#include "Reflection.h"
#include "BoundingVolume.h"
#include "happly.h"
#include "Transformation.h"
class Shading;
class RayIntersection;
class Reflection;
class Refraction;
// Forward declarations to avoid cyclic references
class Camera;
class PointLight;
class Material;
class Shape;
class BoundingVolume;
using namespace std;

// Class to hold everything related to a scene.
class Scene
{
public:
	static int maxRecursionDepth;			// Maximum recursion depth
	float intTestEps;				// IntersectionTestEpsilon. You will need this one while implementing intersect routines in Shape class
	float shadowRayEps;				// ShadowRayEpsilon. You will need this one while generating shadow rays.
	Vector3f backgroundColor;		// Background color
	Vector3f ambientLight;			// Ambient light radiance

	vector<Camera *> cameras;		// Vector holding all cameras
	vector<PointLight *> lights;	// Vector holding all point lights
	vector<Material *> materials;	// Vector holding all materials
	vector<Vector3f> vertices;		// Vector holding all vertices (vertex data)
	vector<Shape *> objects;		// Vector holding all shapes

	Shading* shading;
	RayIntersection* rayIntersection;
	Reflection* reflection;
	Refraction* refraction;
	BoundingVolume* boundingVolume;
	Transformation* transformation;
	void initObjects();
	Scene(const char *xmlPath);		// Constructor. Parses XML file and initializes vectors above. Implemented for you.

	void setScene();
	void readPly(const char* fileName);
	void threading(Camera* camera, Image* image);
	
	void renderScene(void);			// Method to render scene, an image is created for each camera in the scene. You will implement this.
	void renderImage(int part, Camera* camera, Image* image);
	void convertPPMToPNG(string ppmFileName, int osType);
	void readPly(const char* str, const vector<Triangle>& faces, vector<int>* is);
	void readXML(const char* xmlPath);


private:

	void renderImage(Camera* camera, Image* image);
	void renderImagePart(float start, float end, Camera* camera, Image* image);


	int cameraCount;
	int lightCount;
	int objectCount;
	Color* ambientLightList;
};

#endif
