#include "Transformation.h"


#define PI 3.14159265
vec4 Transformation::translation(int id, glm::vec3 position)
{
	vec4 translated =multiplyMatrixWithVec4(*translationMatrices[id],vec4(position,1));
	return translated;
}
vec4 Transformation::inverseTranslation(int id, glm::vec3 position)
{
	vec4 translated = multiplyMatrixWithVec4(*inverseTranslationMatrices[id], vec4(position, 1));
	return translated;
}
vec4 Transformation::scaling(int id, vec3 scale)
{
	vec4 scaled = multiplyMatrixWithVec4(*scalingMatrices[id], vec4(scale, 1));
	return scaled;
}
vec4 Transformation::inverseScaling(int id, vec3 scale)
{
	vec4 scaled = multiplyMatrixWithVec4(*inverseScalingMatrices[id], vec4(scale, 1));
	return scaled;
}
vec4 Transformation::rotation(int id, vec4 _rotation)
{

	vec4 rotated =  multiplyMatrixWithVec4(*rotationMatrices[id],_rotation);
	
	return rotated;
}
vec3 Transformation::inverseRotation(int id, vec4 _rotation)
{
	
	vec3 rotated = multiplyMatrixWithVec4(*inverseRotationMatrices[id], _rotation);
	
	return rotated;
}

void Transformation::initMatrices()
{
	for (int i = 0 ; i < translationList.size();i++)
	{
		mat4x4 *translationMatrix = new mat4x4;

		(*translationMatrix)[0][3] = translationList[i].x;
		(*translationMatrix)[1][3] = translationList[i].y;
		(*translationMatrix)[2][3] = translationList[i].z;
		translationMatrices.push_back(translationMatrix);

		mat4x4* invTrans = new mat4x4(inverse(*translationMatrix));
		(*invTrans)[0][3] = translationList[i].x*-1;
		(*invTrans)[1][3] = translationList[i].y*-1;
		(*invTrans)[2][3] = translationList[i].z*-1;
		
		inverseTranslationMatrices.push_back(invTrans);
	}
	for (int i = 0; i < scalingList.size(); i++)
	{
		mat4x4 *scalingMatrix = new mat4x4;

		(*scalingMatrix)[0][0] = scalingList[i].x;
		(*scalingMatrix)[1][1] = scalingList[i].y;
		(*scalingMatrix)[2][2] = scalingList[i].z;
		scalingMatrices.push_back(scalingMatrix);

		
		mat4x4* invScaling = new mat4x4(0);
		(*invScaling)[0][0] = 1/scalingList[i].x;
		(*invScaling)[1][1] = 1/scalingList[i].y;
		(*invScaling)[2][2] = 1/scalingList[i].z;
		
		inverseScalingMatrices.push_back(invScaling);
	}
	for (int i = 0; i < rotationList.size(); i++)
	{
		 double cost = cos(rotationList[i][0] * PI / 180.0 );
		 double sint = sin(rotationList[i][0] * PI / 180.0 );
		vec4 u = vec4(rotationList[i][1], rotationList[i][2], rotationList[i][3], -1);
		u = normalize(u);

		mat4x4 *rotationMatrix = new mat4x4(0);

		(*rotationMatrix)[0][0] = cost + u.x * u.x * (1 - cost);
		(*rotationMatrix)[0][1] = u.x * u.y * (1 - cost) - u.z * sint;
		(*rotationMatrix)[0][2] = u.x * u.z * (1 - cost) + u.y * sint;
		
		(*rotationMatrix)[1][0] = u.y * u.x * (1 - cost) + u.z * sint;
		(*rotationMatrix)[1][1] = cost + u.y * u.y * (1 - cost);
		(*rotationMatrix)[1][2] = u.y * u.z * (1 - cost) - u.x * sint;
		
		(*rotationMatrix)[2][0] = u.z * u.x * (1 - cost) - u.y * sint;
		(*rotationMatrix)[2][1] = u.z * u.y * (1 - cost) + u.x * sint;
		(*rotationMatrix)[2][2] = cost + u.z * u.z * (1 - cost);
		
		rotationMatrices.push_back(rotationMatrix);
	

		 cost = cos((-1 * rotationList[i][0]) * PI / 180.0);
		 sint = sin((-1 * rotationList[i][0]) * PI / 180.0);
	

		mat4x4* invrotationMatrix = new mat4x4(0);

		(*invrotationMatrix)[0][0] = cost + u.x * u.x * (1 - cost);
		(*invrotationMatrix)[0][1] = u.x * u.y * (1 - cost) - u.z * sint;
		(*invrotationMatrix)[0][2] = u.x * u.z * (1 - cost) + u.y * sint;
		 
		(*invrotationMatrix)[1][0] = u.y * u.x * (1 - cost) + u.z * sint;
		(*invrotationMatrix)[1][1] = cost + u.y * u.y * (1 - cost);
		(*invrotationMatrix)[1][2] = u.y * u.z * (1 - cost) - u.x * sint;
		 
		(*invrotationMatrix)[2][0] = u.z * u.x * (1 - cost) - u.y * sint;
		(*invrotationMatrix)[2][1] = u.z * u.y * (1 - cost) + u.x * sint;
		(*invrotationMatrix)[2][2] = cost + u.z * u.z * (1 - cost);
		inverseRotationMatrices.push_back(invrotationMatrix);
	}
	
	
}
