#pragma once
#ifndef NODE_H_
#define NODE_H_
#include<vector>
using namespace std;

class Node
{
	friend class BTree;
private:
	Node* parent_;
	vector<int> data_;
	vector<Node*> child_;
public:
	Node() { parent_ = NULL; }
	Node(int data, Node* left = NULL, Node* right = NULL, Node* parent = NULL) {
		data_.emplace_back(data);
		parent_ = parent;
		if (left) left->parent_ = this;
		child_.emplace_back(left);
		if (right) right->parent_ = this;
		child_.emplace_back(right);
	}

	Node(vector<int> data, vector<Node*> child, Node* parent = NULL) :
		data_(data), child_(child), parent_(parent) {};
};

#endif // !NODE_H_
