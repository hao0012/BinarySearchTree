#include"node.h"

// 如果left为空就插入
// 返回左子树根节点
Node* Node::insertAsLeft(int data)
{
	if (!left) left = new Node(data, this);
	return left;
}


// 如果right为空就插入
// 返回右子树根节点
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
	// 若为根则不是
	if (!parent) return false;
	return parent->right == this;
}
bool Node::isLeft() const
{
	// 若为根则不是
	if (!parent) return false;
	return parent->left == this;
}