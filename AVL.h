#pragma once
#ifndef AVL_H_
#define AVL_H_
#include"binary_search_tree.h"

#define balancefactor(node) (getHeight(node->left) - getHeight(node->right))
#define isBlanced(node) (balancefactor(node) < 2 && balancefactor(node) > -2)

class AVL: public BST 
{
protected:
	void zig(Node*& node);
	void zag(Node*& node);

	Node* rotateAt(Node* node);
	Node* connect34(Node* a, Node* b, Node* c, 
				   Node* T0, Node* T1, Node* T2, Node* T3);
public:
	Node*& insert(int data);
	bool remove(int data);
};


#endif // !AVL_H_
