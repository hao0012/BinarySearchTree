#include"node.h"

// ���leftΪ�վͲ���
// �������������ڵ�
Node* Node::insertAsLeft(int data)
{
	if (!left) left = new Node(data, this);
	return left;
}


// ���rightΪ�վͲ���
// �������������ڵ�
Node* Node::insertAsRight(int data)
{
	if (!right) right = new Node(data, this);
	return right;
}

Node* Node::getSuccessor() const
{
	Node* successor;
	if (right) {
		successor = right;
		while ((successor->left)) successor = successor->left;
	}
	else {
		successor = this->parent;
		while (successor && successor->isRight()) successor = successor->parent;
		if(successor) successor = successor->parent;
	}
	return successor;
}
Node* Node::getTallerChild()
{
	return (getHeight(left) >= getHeight(right)) ? left : right;
}


bool Node::isRight() const
{
	// ��Ϊ������
	if (!parent) return false;
	return parent->right == this;
}
bool Node::isLeft() const
{
	// ��Ϊ������
	if (!parent) return false;
	return parent->left == this;
}