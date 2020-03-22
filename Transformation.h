#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_

#include "defs.h"
#include "RayTracing/glm/ext.hpp"
#include "RayTracing/glm/glm.hpp"
#include <vector>

using namespace glm;
using namespace std;

// Class for point lights
class Transformation
{
	public:
		vector<vec3> translationList;
		vector<vec3> scalingList;
		vector<vec4> rotationList;
		vec4 translation(int id,vec3 position);
		vec4 scaling(int id,vec3 scale);
		vec4 rotation(int id,vec4 rotationList);
		vec4 multiplyMatrixWithVec4(mat4x4 m, vec4 v)
		{
			double values[4];
			double total;

			for (int i = 0; i < 4; i++)
			{
				total = 0;
				for (int j = 0; j < 4; j++)
				{
					total += m[i][j] * v[j];
				}
				values[i] = total;
			}

			return vec4(values[0], values[1], values[2], values[3]);
		}
};


#endif
