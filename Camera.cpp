#include "Camera.h"
#include <iostream>
#include <cmath>
using namespace std;
Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
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
    this->gaze = gaze.normalizeVector();
    this->up = up.normalizeVector();
    this->imgPlane = imgPlane;
    this->u = up*gaze;
	u = u.normalizeVector();
}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel.
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
    Vector3f pixelPosition = pixelPositionOnImagePlane(row,col);

    Ray *ray = new Ray(pos,normalizeVector(pixelPosition-pos));

    return *ray;
}

Vector3f Camera::pixelPositionOnImagePlane(int row ,int column ) const
{
      float s_u =imgPlane.right-(column+0.5)*((imgPlane.right-imgPlane.left)/imgPlane.nx);
      float s_v =imgPlane.top-  (row+0.5)*((imgPlane.top-imgPlane.bottom)/imgPlane.ny);


      Vector3f pixelPosition = up*s_v + u*s_u+pos+gaze*imgPlane.distance ;
	 
      return pixelPosition;

}

Vector3f Camera::normalizeVector(Vector3f v)const
{

    float magnitude = sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    if(magnitude == 0)
    {
        Vector3f resultVector = {0,0,0}; // ne olacak bilmiyorum böyle gelirse
         return resultVector;
    }
    else
    {
        Vector3f resultVector = {v.x/magnitude , v.y / magnitude, v.z/magnitude};

        return resultVector;
    }

}
