#include"Splay.h"

void Splay::attachAsLeft(Node* p, Node* v)
{
	p->left = v;
	if (v) v->parent = p;
}
void Splay::attachAsRight(Node* p, Node* v)
{
	p->right = v;
	if(v) v->parent = p;
}

Node* Splay::splay(Node* v) {
	if (!v) return NULL;
	Node* p, * g;
	while ((p = v->parent) && (g = p->parent)) {
		Node* gp = g->parent;
		if (p->isLeft()) {
			if (v->isLeft()) {
				attachAsLeft(g, p->right);
				attachAsRight(p, g);

				attachAsLeft(p, v->right);
				attachAsRight(v, p);
			}
			else {
				attachAsRight(p, v->left);
				attachAsLeft(v, p);

				attachAsLeft(g, v->right);
				attachAsRight(v, g);
			}
		}
		else {
			if (v->isRight()) {
				attachAsRight(g, p->left);
				attachAsLeft(p, g);

				attachAsRight(p, v->left);
				attachAsLeft(v, p);
			}
			else {
				attachAsRight(g, v->left);
				attachAsLeft(v, g);

				attachAsLeft(p, v->right);
				attachAsRight(v, p);
			}
		}
		if (!gp) v->parent = NULL;
		else (g == gp->left) ? attachAsLeft(gp, v) : attachAsRight(gp, v);
		updateHeight(p);
		updateHeight(g);
		updateHeight(v);
	}
	p = v->parent;
	if (p) {
		if (v->isLeft()) {
			attachAsLeft(p, v->right);
			attachAsRight(v, p);
		}
		else {
			attachAsRight(p, v->left);
			attachAsLeft(v, p);
		}
		updateHeight(p);
		updateHeight(v);
	}
	v->parent = NULL;
	return v;
}

Node*& Splay::search(int data)
{	
	hot_ = NULL;
	Node*& node = searchIn(root_, data);
	root_ = splay(node ? node : hot_);
	return root_;
}
Node*& Splay::insert(int data)
{
	Node*& node = search(data);
	if (!node) return root_ = new Node(data); // 当前为空树
	else if (node->data == data) return node; // 当前已有要插入的数据
	Node* newRoot = new Node(data);
	if (root_) {
		if (root_->data < data) {
			attachAsLeft(newRoot, root_);
			attachAsRight(newRoot, root_->right);
			root_->right = NULL;
		}
		else {
			attachAsRight(newRoot, root_);
			attachAsLeft(newRoot, root_->left);
			root_->left = NULL;
		}
	}
	root_ = newRoot;
	if(root_->left) updateHeight(root_->left);
	if(root_->right) updateHeight(root_->right);
	updateHeight(root_);
	return root_;
}
bool Splay::remove(int data)
{
	if (!root_ || data != search(data)->data) return false;
	Node* temp = root_;
	if (!root_->left) {
		root_ = root_->right;
		if(root_->right) root_->right->parent = NULL;
	}
	else if (!root_->right) {
		root_ = root_->left;
		root_->left->parent = NULL;
	}
	else {
		Node* leftTree = root_->left;
		leftTree->parent = NULL;
		root_->left = NULL;
		root_ = root_->right;
		root_->parent = NULL;
		search(temp->data);
		attachAsLeft(root_, leftTree);
	}
	delete temp;
	--size_;
	if(root_) updateHeight(root_);
	return true;
}