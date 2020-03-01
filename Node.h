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
	int start;
	int end;
	Node()
	{
		boundingBox = new BoundingBox();
	}
	Node(Node* left,Node *right)
	{
		this->left = left;
		this->right = right;
	}
	BoundingBox* getBoundingBox(Node* left, Node* right);
};
#endif

