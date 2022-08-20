#include"AVL.h"
// ----------------------------- protected ------------------------------- //

// ����
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

// ����
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
	if (!hot_) root_ = node; // ��ʱ����Ľڵ�Ϊ���ڵ�
	// ��ƽ��
	for (Node* p = node->parent; p; p = p->parent) {
		if (!isBlanced(p)) { // �������ʧ��
			// �ȼ�¼Ҫ����ת�������ĸ���
			Node* temp = p->parent;
			// ��ת�����ر���ת������������
			Node* newp = rotateAt((p->getTallerChild())->getTallerChild());
			// �����ص�������ԭ������˫������
			// ���������ĸ���
			newp->parent = temp;
			// �����������丸�׵��ӽڵ��е�λ��
			if (temp) {
				if (p == temp->left) temp->left = newp;
				else temp->right = newp;
			}
			// �����ת������Ϊ�����ĸ�����ô����root_
			if (p == root_) root_ = newp;
			// ��ת�������߶���δ����ǰ��ͬ����˲��ø��¸߶�
			break;
		}
		else {
			// ���¸߶�
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
	// ��ƽ��
	for (Node* p = target->parent; p; p = p->parent) {
		if (!isBlanced(p)) { // �������ʧ��
			// �ȼ�¼Ҫ����ת�������ĸ���
			Node* temp = p->parent;
			// ��ת�����ظ�������������
			Node* newp = rotateAt((p->getTallerChild())->getTallerChild());
			// ����2�������ص�������ԭ������˫������
			// 1.���������ĸ���
			newp->parent = temp;
			// 2.�����������丸�׵��ӽڵ��е�λ��
			if (temp) {
				if (p == temp->left) temp->left = newp;
				else temp->right = newp;
			}
			// �����ת������Ϊ�����ĸ�����ô����root_
			if (p == root_) root_ = newp;
			updateHeight(p); // �߶ȸ���
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