#pragma once
#ifndef SPLAY_H_
#define SPLAY_H_

#include"binary_search_tree.h"

class Splay : public BST
{
protected:
	void attachAsLeft(Node* p, Node* v);
	void attachAsRight(Node* p, Node* v);

	Node* splay(Node* v);
public:
	Node*& search(int data);
	Node*& insert(int data);
	bool remove(int data);
};

#endif // !SPLAY_H_

