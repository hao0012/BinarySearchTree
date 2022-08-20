#pragma once
#ifndef NODE_H_
#define NODE_H_
#include<iostream>
using namespace std;

#define getHeight(node) ((node) ? (node)->height : 0)
#define black 1
#define red 0

class Node
{
	friend class BinaryTree;
	friend class BST;
	friend class AVL;
	friend class Splay;
	friend class RBTree;
private:
	int data;
	Node* parent;
	Node* left, * right;
	int height;
	bool color; // 0-red 1-black
//private:
public:
	Node(int d = 0, Node* p = NULL, Node* l = NULL, Node* r = NULL, int h = 0, int c = red)
		: data(d), parent(p), left(l), right(r), height(h), color(c) {};

	Node* insertAsLeft(int data);
	Node* insertAsRight(int data);
	Node* getSuccessor() const;

	Node* getTallerChild();

	bool isRight() const;
	bool isLeft() const;
};

#endif // !NODE_H_