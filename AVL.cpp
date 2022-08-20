#include"AVL.h"
// ----------------------------- protected ------------------------------- //

// 右旋
void AVL::zig(Node*& node)
{
	Node*& left = node->left;
	Node*& r = left->right;
	left->right = node;
	left->parent = node->parent;
	node->parent = left;
	node->left = r;
	r->parent = node;
}

// 左旋
void AVL::zag(Node*& node)
{
	Node* temp = node;
	node = node->right;

	Node* l = node->left;
	node->left = temp;
	temp->parent = node;

	temp->right = l;
	if(l) l->parent = temp;
}

//void AVL::rotateAt(Node** node)
//{
//	Node** grand = node;
//	Node** parent = &((*node)->getTallerChild());
//	Node** child = parent->getTallerChild();
//	if (parent->isRight()) {
//		if (child->isLeft()) zig(parent);
//		zag(grand);
//	}
//	else {
//		if (child->isRight()) zag(parent);
//		zig(grand);
//	}
//	if (!grand->parent) root_ = grand;
//}

Node*& AVL::insert(int data)
{
	Node*& node = search(data);
	if (node) return node;
	++size_;
	node = new Node(data, hot_);
	if (!hot_) root_ = node; // 此时插入的节点为根节点
	// 重平衡
	for (Node* p = node->parent; p; p = p->parent) {
		if (!isBlanced(p)) { // 如果发现失衡
			// 先记录要被旋转的子树的父亲
			Node* temp = p->parent;
			// 旋转并返回被旋转子树的新树根
			Node* newp = rotateAt((p->getTallerChild())->getTallerChild());
			// 将返回的子树与原树进行双向链接
			// 更新子树的父亲
			newp->parent = temp;
			// 更新子树在其父亲的子节点中的位置
			if (temp) {
				if (p == temp->left) temp->left = newp;
				else temp->right = newp;
			}
			// 如果旋转子树根为整树的根，那么更新root_
			if (p == root_) root_ = newp;
			// 旋转后子树高度与未插入前相同，因此不用更新高度
			break;
		}
		else {
			// 更新高度
			updateHeight(p);
		}
	}
	return node;
}

bool AVL::remove(int data)
{
	Node*& target = search(data);
	if (!target) return false;
	removeAt(target);
	--size_;
	// 重平衡
	for (Node* p = target->parent; p; p = p->parent) {
		if (!isBlanced(p)) { // 如果发现失衡
			// 先记录要被旋转的子树的父亲
			Node* temp = p->parent;
			// 旋转并返回该子树的新树根
			Node* newp = rotateAt((p->getTallerChild())->getTallerChild());
			// 下面2步将返回的子树与原树进行双向链接
			// 1.更新子树的父亲
			newp->parent = temp;
			// 2.更新子树在其父亲的子节点中的位置
			if (temp) {
				if (p == temp->left) temp->left = newp;
				else temp->right = newp;
			}
			// 如果旋转子树根为整树的根，那么更新root_
			if (p == root_) root_ = newp;
			updateHeight(p); // 高度更新
		}
	}
	return true;
}

Node* AVL::rotateAt(Node* v)
{
	Node* p = v->parent, * g = p->parent;
	if (p->isLeft()) {
		if (v->isLeft()) {
			p->parent = g->parent;
			return connect34(v, p, g, v->left, v->right, p->right, g->right);
		}
		else {
			v->parent = g->parent;
			return connect34(p, v, g, p->left, v->left, v->right, g->right);
		}
	}
	else {
		if (v->isRight()) {
			p->parent = g->parent;
			return connect34(g, p, v, g->left, p->left, v->left, v->right);
		}
		else {
			v->parent = g->parent;
			return connect34(g, v, p, g->left, v->left, v->right, p->right);
		}
	}
}

Node* AVL::connect34(Node* a, Node* b, Node* c,
					 Node* T0, Node* T1, Node* T2, Node* T3)
{
	a->left = T0; if (T0) T0->parent = a;
	a->right = T1; if (T1) T1->parent = a;
	updateHeight(a);

	c->left = T2; if (T2) T2->parent = c;
	c->right = T3; if (T3) T3->parent = c;
	updateHeight(c);

	b->left = a; a->parent = b;
	b->right = c; c->parent = b;
	updateHeight(b);

	return b;
}