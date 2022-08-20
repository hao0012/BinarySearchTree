#include"red_black_Tree.h"
#include<iostream>
using namespace std;

int main() {
	RBTree rb;
	Node* n1 = new Node(25);
	Node* n2 = new Node(30);
	Node* left = new Node(5);
	Node* right = new Node(20);
	Node* root = new Node(10, NULL);
	root->left = left;
	left->parent = root;
	root->right = right;
	right->parent = root;
	right->left = n1;
	n1->parent = right;
	right->right = n2;
	n2->parent = right;
	rb.root_ = root;
	n1->color = black;
	n2->color = black;
	left->color = black;
	right->color = red;
	root->color = black;
	n1->height = 1;
	n2->height = 1;
	left->height = 1;
	right->height = 1;
	root->height = 2;
	rb.remove(5);
	rb << cout;
}