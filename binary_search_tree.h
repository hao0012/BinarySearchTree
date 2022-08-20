#pragma once
#ifndef BINARY_SEARCH_TREE_H_
#define BINARY_SEARCH_TREE_H_
#include"binary_tree.h"

class BST: public BinaryTree
{
protected:
	Node* hot_;
	virtual Node*& searchIn(Node*& root, int data);
	virtual Node* removeAt(Node*& target);
public:
	virtual Node* & search(int data);
	virtual Node* & insert(int data);
	virtual bool remove(int data);

	istream& operator>>(istream& in);
};

#endif // !BINARY_SEARCH_TREE_H_
