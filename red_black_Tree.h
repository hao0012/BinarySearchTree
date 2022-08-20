#pragma once
#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_
#include"binary_search_tree.h"


#define isBlack(node) ((!node)||((node)->color == 1))
#define isRed(node) (!isBlack(node))


class RBTree : public BST
{
protected:
	void doubleRed(Node* node);
	void doubleBlack(Node* node);
	int updateHeight(Node* node);
	Node* rotateAt(Node* v);
	Node* connect34(Node* a, Node* b, Node* c,
			   Node* T0, Node* T1, Node* T2, Node* T3);
public:
	Node*& insert(int data);
	bool remove(int data);
};


#endif // !RED_BLACK_TREE_H_
