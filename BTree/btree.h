#pragma once
#ifndef BTREE_H_
#define BTREE_H_
#include"node.h"
#include<iostream>
#include<queue>

#define UPPER_LIMIT 4
#define LOWER_LIMIT 2
class BTree
{
private:
	int size_;
	int order_;
	int lower_limit;
	Node* root_;
	Node* hot_;
	template<typename T> int find(vector<T>& store, T data);
	template<typename T> vector<T>& split(vector<T>& v, int point, bool reservePoint);
	Node* searchIn(Node* root, int data);
	void overflow(Node* node);
	void underflow(Node* node);
public:
	BTree() : size_(0), order_(UPPER_LIMIT), root_(NULL), hot_(NULL) { 
		lower_limit = (order_ + 1) / 2 - 1; 
	};
	Node* search(int data);
	bool insert(int data);
	bool remove(int data);
	ostream& operator<<(ostream& out);
};

#endif // !BTREE_H_
