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
	vector<Shape*> objects;
public:
	Node* root;
	int level=0;
	BoundingVolume(vector<Shape*> _objects)
	{
		root = new Node();
		root->nodeType = Root;
	    objects = std::move(_objects);

		build(root, 0, objects.size()-1);

	}

	bool isInLeft(const Vector3f& midPoint, BoundingBox* bounding_box);
	bool sortVector(const Vector3f& v1, const Vector3f& v2);


	void build(Node* node, int left, int right);
};


#endif

