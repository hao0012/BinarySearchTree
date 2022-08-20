#pragma once
#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_
#include"node.h"
#include<queue>

class BinaryTree
{
protected:
	int size_;
	Node* root_;
	int updateHeight(Node* node);
	void updateHeightAbove(Node* node);
public:
	BinaryTree() : size_(0), root_(NULL) {};
	~BinaryTree();
	int size() const { return size_; };
	bool empty() { return root_ == NULL; };
	Node* insertAsRoot(int data);
	Node* insertAsLeft(Node* subroot, int data);
	Node* insertAsRight(Node* subroot, int data);
	ostream& operator<<(ostream& out) const;
};

#endif // !BINARY_SEARCH_TREE_H_
