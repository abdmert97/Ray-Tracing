#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cstdio>
#include <cstdlib>

#include "defs.h"

//
// Access the color using either individual component names
// or the channel array.
//
typedef union Color
{
    struct
    {
        unsigned char red;
        unsigned char grn;
        unsigned char blu;
    };
	Color operator+(Color c)
	{
		Color retVal = { red + c.red,
						grn + c.grn,
						blu + c.blu };
		return retVal;
	}
	Color operator+(Vector3f colorVector)
	{
		Color color = { red,grn,blu };
		if ((int)color.red + colorVector.x > 255)
			color.red = (char)255;
		else
			color.red += (char)colorVector.x;

		if ((int)color.grn + colorVector.y > 255)
			color.grn = (char)255;
		else
			color.grn += (char)colorVector.y;

		if ((int)color.blu + colorVector.z > 255)
			color.blu = (char)255;
		else
			color.blu += (char)colorVector.z;
		return color;
	}
	Color operator*(float c)
	{

		return Color{ (unsigned char)(red * c),(unsigned char)(grn * c),(unsigned char)(blu * c) };
	}
	ostream& operator<<(ostream& os)
	{
		return os << "red: " << red << " green: " << grn << " blue: " << blu << endl;

	}
    unsigned char channel[3];
} Color;

/* This class is provided to you for defining an image as a variable, manipulate it easily, and save it as a ppm file. */
class Image
{
public:
    Color** data;                   // Image data
	int width;						// Image width
	int height;						// Image height

	Image(int width, int height);	// Constructor
	void setPixelValue(int col, int row, const Color& color); // Sets the value of the pixel at the given column and row
	void saveImage(const char *imageName) const;	          // Takes the image name as a file and saves it as a ppm file. 
};

#endif
