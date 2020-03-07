#ifndef _NODE_H_
#define _NODE_H_


#include "BoundingBox.h"

enum NodeType
{
	Root,
	Parent,
	Leaf
};
class Node
{
	
public:
	BoundingBox* boundingBox;
	Node* left;
	Node* right;
	NodeType nodeType;
	vector<int> ObjectIDs;
	Node()
	{
		boundingBox = new BoundingBox();
		nodeType = Leaf;
		left = nullptr;
		right = nullptr;
	}
	Node(Node* left,Node *right)
	{
		this->left = left;
		this->right = right;
	}

};
#endif

