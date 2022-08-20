#include"red_black_Tree.h"

Node* RBTree::rotateAt(Node* v) {
	Node *p = v->parent, *g = p->parent;
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

Node* RBTree::connect34(Node* a, Node* b, Node* c,
	Node* T0, Node* T1, Node* T2, Node* T3) {
	a->left = T0; if(T0) T0->parent = a;
	a->right = T1; if(T1) T1->parent = a;

	c->left = T2; if(T2) T2->parent = c;
	c->right = T3; if(T3) T3->parent = c;

	b->left = a; a->parent = b;
	b->right = c; c->parent = b;
	return b;
}

// 高度为黑高度
int RBTree::updateHeight(Node* node)
{
	// 空节点默认为黑节点，因此高度0
	if (!node) return 0;
	node->height = max(getHeight(node->left), getHeight(node->right));
	if (isBlack(node)) ++node->height;
	return node->height;
}

void RBTree::doubleRed(Node* node) {
	Node* parent = node->parent;
	// 如果node父为黑，则不用调整
	if (isBlack(parent)) return;
	Node* grand = parent->parent;
	// 获得node的叔叔节点
	Node* uncle = (parent->isLeft()) ? grand->right : grand->left;
	// 如果uncle为黑, 需旋转和染色
	if (isBlack(uncle)) { // 这种情况下操作后 黑高度与未插入数据前相等
		// 提前记录
		Node* gp = grand->parent;
		// 旋转
		Node* p = rotateAt(node);
		// 统一染色，根为黑，左右都为红
		
		p->left->color = red;
		updateHeight(p->left);
		p->right->color = red;
		updateHeight(p->right);
		p->color = black;
		updateHeight(p);
		
		// 若grand有父节点，即grand不为根
		if (gp) {
			// 则在gp中更新
			if (gp->left == grand) gp->left = p;
			else gp->right = p;
		}
		// 若grand为根，则更新根
		else root_ = p;
	}
	// uncle为红, 需3次染色和向上继续检查
	else {
		uncle->color = black;
		// 黑高度+1
		++uncle->height;
		parent->color = black; 
		// 黑高度+1
		++parent->height;
		// 若grand为根， 则根的黑高度+1
		if (root_ == grand) ++grand->height;
		// 若grand不为根，则染为红色
		else grand->color = red;
		// 上一层可能还会发生双红缺陷，继续检查
		doubleRed(grand);
	}
}

Node*& RBTree::insert(int data) {
	Node*& node = search(data);
	if (node) return node;
	// 若hot_为NULL, 即插入的为树根，颜色为黑，黑高度1
	if (!hot_) node = new Node(data, hot_, NULL, NULL, 1, black);
	// 否则，插入的为树叶，默认颜色为红，黑高度0
	else {
		node = new Node(data, hot_, NULL, NULL, 0, red);
		// 检查是否有双红缺陷
		doubleRed(node);
	}
	++size_;
	return node;
}

bool RBTree::remove(int data) {
	Node *&node = search(data);
	// 没找到要删除的节点，删除失败
	if (!node) return false;
	// 删除, result是接替的那个节点
	Node *result = removeAt(node);
	--size_;
	// 情况1. 删除后为空树
	if (size_ == 0) return true;
	// 情况2. 若实际删除的是根
	if (!hot_) {
		// 新根必须为黑色
		result->color = black;
		updateHeight(result);
		return true;
	}
	// 情况3. 被删除节点红, 父亲必为黑, 该种情况无需调整
	if ( isBlack(hot_) // 父亲为黑 
			 && 
		 getHeight(hot_) == getHeight(result) + 1) // 通过黑高度判断被删除节点颜色为红
		return true;
	// 情况4. 被删除节点黑, 有红子
	if (isRed(result)) {
		result->color = black;
		++result->height;
		return true;
	}
	// 情况5. 被删除节点黑且无红子
	else doubleBlack(result);
	return true;
}

void RBTree::doubleBlack(Node* node) {
	Node* p = (node) ? node->parent : hot_;
	if (!p) return;
	Node* sibling = (p->left == node) ? p->right : p->left;
	Node* nephew = (isRed(sibling->right)) ? sibling->right : sibling->left;
	// 1. 若兄弟为黑
	if (isBlack(sibling)) {
		// 1.1 若有侄子为红
		if (isRed(nephew)) {
			Node* grand = p->parent;
			bool c = p->color;
			// 对侄子旋转 temp为新树的根
			Node* temp = rotateAt(nephew);
			// 重染色
			if (temp->left) temp->left->color = black;
			updateHeight(temp->left);
			if (temp->right) temp->right->color = black;
			updateHeight(temp->right);
			temp->color = c;
			updateHeight(temp);
			// 重建与整树的连接
			if (grand) {
				if (grand->left == p) grand->left = temp;
				else grand->right = temp;
			}
			else root_ = temp;
			return;
		}
		// 1.2 若侄子都为黑色
		// 则兄弟转红
		sibling->color = red;
		--sibling->height;
		// 1.2.1 若parent为红色 BB-2-R
		if (isRed(p))  p->color = black;
		// 1.2.2 若parent为黑色 BB-2-B
		else {
			--p->height;
			doubleBlack(p);
		}
	}
	else {
		// sibling 为红色的情况 BB-3
		// 修改颜色
		p->color = red;
		sibling->color = black;
		// 获得与sibling同侧的t
		Node* t = (sibling->isLeft()) ? sibling->left : sibling->right;
		// 临时记录祖父节点，用于一会儿的连接
		Node* grand = p->parent;
		// 记录当前的父节点
		hot_ = p;
		// 旋转
		Node* temp = rotateAt(t);
		// 连接
		if (grand) {
			if (grand->left == p) grand->left = temp;
			else grand->right = temp;
		}
		else root_ = temp;
		// 转到其他情况重新处理
		doubleBlack(node);
	}
}