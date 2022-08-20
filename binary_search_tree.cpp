#include"binary_search_tree.h"

istream& BST::operator>>(istream& in)
{
	cout << "please input some data:" << endl;
	int data;
	while (in >> data) {
		insert(data);
	}
	return in;
}

Node*& BST::searchIn(Node*& root, int e) {
	if (!root || root->data == e) return root;
	hot_ = root;
	return searchIn((root->data > e ? root->left : root->right), e);
}

Node*& BST::search(int data)
{
	hot_ = NULL;
	return searchIn(root_, data);
}

// 迭代版search
//Node*& BST::search(int data)
//{
//	Node* node = root_;
//	while (node && node->data != data) {
//		node = (node->data < data) ? node->left : node->right;
//	}
//	hot_ = node->parent;
//	return node;
//}

Node* & BST::insert(int data)
{
	Node*& node = search(data);
	if (node) return node;
	++size_;
	node = new Node(data, hot_);
	
	updateHeightAbove(node);
	return node;
}

Node* BST::removeAt(Node*& target)
{
	Node* willDelete = target;
	Node* succ; // 接替者
	if (!target->left) succ = target = target->right;
	else if (!target->right) succ = target = target->left;
	else {
		// 取得中序遍历后继节点
		Node* successor = target->getSuccessor();
		// 将后继的值赋给当前要删除节点
		target->data = successor->data;
		// 要删除的节点改为后继节点
		willDelete = successor;
		// 后继只 可能有右子树 或 仅为叶子节点，因此该后继的父亲节点只需将左子指向该后继的右子
		((successor == target->right) ? successor->parent->right : successor->parent->left)
			= succ = successor->right;
	}
	hot_ = willDelete->parent;
	if (succ) succ->parent = hot_;
	delete willDelete;
	return succ;
}

bool BST::remove(int data)
{
	Node*& target = search(data);
	if (target == NULL) return false;
	removeAt(target);
	--size_;
	updateHeightAbove(hot_);
	return true;
}