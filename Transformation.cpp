#include "Transformation.h"


#define PI 3.14159265
vec4 Transformation::translation(int id, glm::vec4 position)
{
	vec4 translated =multiplyMatrixWithVec4(*translationMatrices[id],position);
	return translated;
}

vec4 Transformation::translationNormal(int id, glm::vec4 position)
{
	vec4 translated = multiplyMatrixWithVec4(*NormalTranslationMatrices[id], position);
	return translated;
}
vec4 Transformation::inverseTranslation(int id, glm::vec4 position)
{
	vec4 translated = multiplyMatrixWithVec4(*inverseTranslationMatrices[id], position);
	return translated;
}
vec4 Transformation::scaling(int id, vec4 scale)
{
	vec4 scaled = multiplyMatrixWithVec4(*scalingMatrices[id], scale);
	return scaled;
}
vec4 Transformation::scalingNormal(int id, vec4 scale)
{
	vec4 scaled = multiplyMatrixWithVec4(*NormalScalingMatrices[id], scale);
	return scaled;
}
vec4 Transformation::inverseScaling(int id, vec4 scale)
{
	vec4 scaled = multiplyMatrixWithVec4(*inverseScalingMatrices[id], scale);
	return scaled;
}
vec4 Transformation::rotation(int id, vec4 _rotation)
{

	vec4 rotated =  multiplyMatrixWithVec4(*rotationMatrices[id],_rotation);
	
	return rotated;
}
vec4 Transformation::rotationNormal(int id, vec4 _rotation)
{

	vec4 rotated = multiplyMatrixWithVec4(*NormalRotationMatrices[id], _rotation);

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
		mat4x4 *translationMatrix = new mat4x4(0);

		(*translationMatrix)[0][3] = translationList[i].x;
		(*translationMatrix)[1][3] = translationList[i].y;
		(*translationMatrix)[2][3] = translationList[i].z;
		(*translationMatrix)[3][3] = 1;
		(*translationMatrix)[0][0] = 1;
		(*translationMatrix)[1][1] = 1;
		(*translationMatrix)[2][2] = 1;
		translationMatrices.push_back(translationMatrix);

		mat4x4* invTrans = new mat4x4(*translationMatrix);
		(*invTrans)[0][3] = translationList[i].x*-1;
		(*invTrans)[1][3] = translationList[i].y*-1;
		(*invTrans)[2][3] = translationList[i].z*-1;
		(*invTrans)[3][3] = 1;
		(*invTrans)[0][0] = 1;
		(*invTrans)[1][1] = 1;
		(*invTrans)[2][2] = 1;
		inverseTranslationMatrices.push_back(invTrans);

		mat4x4* normalMatrix = new mat4x4(transpose(*invTrans));
		NormalTranslationMatrices.push_back(normalMatrix);
	}
	for (int i = 0; i < scalingList.size(); i++)
	{
		mat4x4 *scalingMatrix = new mat4x4(0);

		(*scalingMatrix)[0][0] = scalingList[i].x;
		(*scalingMatrix)[1][1] = scalingList[i].y;
		(*scalingMatrix)[2][2] = scalingList[i].z;
		(*scalingMatrix)[3][3] = 1;
		scalingMatrices.push_back(scalingMatrix);

		
		mat4x4* invScaling = new mat4x4(0);
		(*invScaling)[0][0] = 1.0/scalingList[i].x;
		(*invScaling)[1][1] = 1.0/scalingList[i].y;
		(*invScaling)[2][2] = 1.0/scalingList[i].z;
		(*invScaling)[3][3] = 1;
		
		inverseScalingMatrices.push_back(invScaling);

		mat4x4* normalMatrix = new mat4x4(transpose(*invScaling));
		NormalScalingMatrices.push_back(normalMatrix);
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
		(*rotationMatrix)[3][3] = 1;
		rotationMatrices.push_back(rotationMatrix);
		 mat4x4* invrotationMatrix;
		invrotationMatrix = new mat4x4(transpose(*rotationMatrix));
		inverseRotationMatrices.push_back(invrotationMatrix);
		mat4x4* normalMatrix = new mat4x4(transpose(*invrotationMatrix));
		NormalRotationMatrices.push_back(normalMatrix);
	}
	
	
}
