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

// ������search
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
	Node* succ; // ������
	if (!target->left) succ = target = target->right;
	else if (!target->right) succ = target = target->left;
	else {
		// ȡ�����������̽ڵ�
		Node* successor = target->getSuccessor();
		// ����̵�ֵ������ǰҪɾ���ڵ�
		target->data = successor->data;
		// Ҫɾ���Ľڵ��Ϊ��̽ڵ�
		willDelete = successor;
		// ���ֻ ������������ �� ��ΪҶ�ӽڵ㣬��˸ú�̵ĸ��׽ڵ�ֻ�轫����ָ��ú�̵�����
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