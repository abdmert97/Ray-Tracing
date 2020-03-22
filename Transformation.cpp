#include "Transformation.h"



vec4 Transformation::translation(int id, glm::vec3 position)
{
	mat4x4 translationMatrix;

	translationMatrix[0][3] = translationList[id].x;
	translationMatrix[1][3] = translationList[id].y;
	translationMatrix[2][3] = translationList[id].z;

	vec4 translated =multiplyMatrixWithVec4(translationMatrix,vec4(position,1));
	return translated;
}

vec4 Transformation::scaling(int id, vec3 scale)
{
	mat4x4 scalingMatrix;

	scalingMatrix[0][0] = scalingList[id].x;
	scalingMatrix[1][1] = scalingList[id].y;
	scalingMatrix[2][2] = scalingList[id].z;

	vec4 scaled = multiplyMatrixWithVec4(scalingMatrix, vec4(scale, 1));
	return scaled;
}

vec4 Transformation::rotation(int id, vec4 _rotation)
{
	const double cost = cos(rotationList[id][0]);
	const double sint = sin(rotationList[id][0]);
	vec4 u  = vec4(rotationList[id][1], rotationList[id][2], rotationList[id][3], -1);
	u = normalize(u);
	
	mat4x4 rotationMatrix;

	rotationMatrix[0][0] = cost + u.x * u.x * (1 - cost);
	rotationMatrix[0][1] = u.x * u.y * (1 - cost) - u.z * sint;
	rotationMatrix[0][2] = u.x * u.z * (1 - cost) + u.y * sint;

	rotationMatrix[1][0] = u.y * u.x * (1 - cost) + u.z * sint;
	rotationMatrix[1][1] = cost + u.y * u.y * (1 - cost);
	rotationMatrix[1][2] = u.y * u.z * (1 - cost) - u.x * sint;

	rotationMatrix[2][0] = u.z * u.x * (1 - cost) - u.y * sint;
	rotationMatrix[2][1] = u.z * u.y * (1 - cost) + u.x * sint;
	rotationMatrix[2][2] = cost + u.z * u.z * (1 - cost);

	vec4 rotated =  multiplyMatrixWithVec4(rotationMatrix,_rotation);
	return rotated;
}
