#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Ray.h"
#include "defs.h"
#include "RayTracing/glm/vec3.hpp"

// Structure for holding variables related to the image plane
typedef struct ImagePlane
{
	float left;     // "u" coordinate of the left edge
	float right;    // "u" coordinate of the right edge
	float bottom;   // "v" coordinate of the bottom edge
	float top;      // "v" coordinate of the top edge
    float distance; // distance to the camera (always positive)
    int nx;         // number of pixel columns
    int ny;         // number of pixel rows
} ImagePlane;

class Camera
{
public:
  char imageName[32];
  int id;
  ImagePlane imgPlane;     // Image plane

	Camera(int id,                      // Id of the camera
           const char* imageName,       // Name of the output PPM file
           const glm::vec3& pos,         // Camera position
           const glm::vec3& gaze,        // Camera gaze direction
           const glm::vec3& up,          // Camera up direction
           const ImagePlane& imgPlane); // Image plane parameters

    // Computes the primary ray through pixel (row, col)
	Ray getPrimaryRay(int row, int col) const;

private:
   glm::vec3 pos;         // Camera position
   glm::vec3 gaze;        // Camera gaze direction
   glm::vec3 up;
   glm::vec3 u;
   glm::vec3 DotProduct(const glm::vec3& vector1,const glm::vec3& vector2);
   glm::vec3 pixelPositionOnImagePlane(int column , int row) const;
   glm::vec3 normalizeVector(glm::vec3 v)const;
};

#endif
