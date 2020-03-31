#include "Camera.h"
#include <iostream>
#include <cmath>
#include "RayTracing/glm/detail/func_geometric.inl"
using namespace std;
Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file
               const glm::vec3& pos,         // Camera position
               const glm::vec3& gaze,        // Camera gaze direction
               const glm::vec3& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
{
    this->id  = id;
    int i = 0;
    while(imageName[i])
    {
        this->imageName[i] = imageName[i];
        i++;
    }
    this->imageName[i] = 0 ;
    this->pos = pos;
    this->gaze = normalizeVector(gaze);
    this->up = normalizeVector(up);
    this->imgPlane = imgPlane;
    this->u = cross(up,gaze);
	u = normalizeVector(u);
}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel.
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
    glm::vec3 pixelPosition = pixelPositionOnImagePlane(row,col);

    Ray ray = Ray(pos,glm::normalize(pixelPosition-pos));
	
    return ray;
}

glm::vec3 Camera::pixelPositionOnImagePlane(int row ,int column ) const
{
      float s_u =imgPlane.right-(column+0.5)*((imgPlane.right-imgPlane.left)/imgPlane.nx);
      float s_v =imgPlane.top-  (row+0.5)*((imgPlane.top-imgPlane.bottom)/imgPlane.ny);
	
	
      glm::vec3 pixelPosition = up*s_v + u*s_u+pos+gaze*imgPlane.distance ;
	  
      return pixelPosition;

}

glm::vec3 Camera::normalizeVector(glm::vec3 v)const
{

    float magnitude = sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    if(magnitude == 0)
    {
        glm::vec3 resultVector = {0,0,0}; // ne olacak bilmiyorum böyle gelirse
         return resultVector;
    }
    else
    {
        glm::vec3 resultVector = {v.x/magnitude , v.y / magnitude, v.z/magnitude};

        return resultVector;
    }

}
