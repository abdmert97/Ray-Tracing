#ifndef _BOUNDINGVOLUME_H_
#define _BOUNDINGVOLUME_H_

#include <utility>
#include "Node.h"
#include <vector>
#include <algorithm>
#include "Shape.h"
#include <ctime>

class BoundingVolume
{
private:
	vector<Shape*> *objects;
	vector<Triangle> *triangles;

public:
	Node* root;
	int level=0;
	BoundingVolume(vector<Shape*> *_objects)
	{
		root = new Node();
		root->nodeType = Root;
	    objects = std::move(_objects);
		for (int i = 0; i < objects->size();i++)
			root->ObjectIDs.push_back(i);
		build(root);

	}
	BoundingVolume(vector<Triangle> triangles)
	{
		root = new Node();
		root->nodeType = Root;
		this->triangles = &triangles;
		for (int i = 0; i < triangles.size(); i++)
			root->ObjectIDs.push_back(i);
		buildMeshVolume(root);

	}
	bool isInLeft(const glm::vec3& midPoint, BoundingBox* bounding_box);
	bool sortVector(const std::pair<int, glm::vec3>& v1, const std::pair<int, glm::vec3>& v2);



	void build(Node* node);
	void buildMeshVolume(Node* node);

};


#endif

