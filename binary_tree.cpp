#include"binary_tree.h"
//--------------------------------- protected ---------------------------------//
int BinaryTree::updateHeight(Node* node)
{
	int leftHeight = getHeight(node->left);
	int rightHeight = getHeight(node->right);
	node->height = 1 + max(leftHeight, rightHeight);
	return node->height;
}
void BinaryTree::updateHeightAbove(Node* node)
{
	while (node) {
		updateHeight(node);
		node = node->parent;
	}
}
//--------------------------------- protected ---------------------------------//

ostream& BinaryTree::operator<<(ostream& out) const
{
	if (root_) {
		queue<Node*> q;
		q.emplace(root_);
		Node* node = NULL;
		while (!q.empty()) {
			for (size_t i = q.size(); i > 0; --i) {
				node = q.front();
				q.pop();
				if (node->left) q.emplace(node->left);
				if (node->right) q.emplace(node->right);
				out << node->data << " ";
			}
			out << "\n";
		}
	}
	return out;
}

BinaryTree::~BinaryTree()
{
	if (root_) {
		std::queue<Node*> q;
		q.emplace(root_);
		Node* node = NULL;
		while (!q.empty()) {
			node = q.front();
			q.pop();
			if (node->left) q.emplace(node->left);
			if (node->right) q.emplace(node->right);
			delete node;
		}
	}
}

Node* BinaryTree::insertAsRoot(int data)
{
	if (!root_) {
		size_ = 1;
		root_ = new Node(data);
	}
	return root_;
}
Node* BinaryTree::insertAsLeft(Node* subroot, int data)
{
	if (!subroot) return NULL;
	++size_;
	subroot->insertAsLeft(data);
	updateHeightAbove(subroot);
	return subroot->left;
}
Node* BinaryTree::insertAsRight(Node* subroot, int data)
{
	if (!subroot) return NULL;
	++size_;
	subroot->insertAsRight(data);
	updateHeightAbove(subroot);
	return subroot->right;
}