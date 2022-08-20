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

// �߶�Ϊ�ڸ߶�
int RBTree::updateHeight(Node* node)
{
	// �սڵ�Ĭ��Ϊ�ڽڵ㣬��˸߶�0
	if (!node) return 0;
	node->height = max(getHeight(node->left), getHeight(node->right));
	if (isBlack(node)) ++node->height;
	return node->height;
}

void RBTree::doubleRed(Node* node) {
	Node* parent = node->parent;
	// ���node��Ϊ�ڣ����õ���
	if (isBlack(parent)) return;
	Node* grand = parent->parent;
	// ���node������ڵ�
	Node* uncle = (parent->isLeft()) ? grand->right : grand->left;
	// ���uncleΪ��, ����ת��Ⱦɫ
	if (isBlack(uncle)) { // ��������²����� �ڸ߶���δ��������ǰ���
		// ��ǰ��¼
		Node* gp = grand->parent;
		// ��ת
		Node* p = rotateAt(node);
		// ͳһȾɫ����Ϊ�ڣ����Ҷ�Ϊ��
		
		p->left->color = red;
		updateHeight(p->left);
		p->right->color = red;
		updateHeight(p->right);
		p->color = black;
		updateHeight(p);
		
		// ��grand�и��ڵ㣬��grand��Ϊ��
		if (gp) {
			// ����gp�и���
			if (gp->left == grand) gp->left = p;
			else gp->right = p;
		}
		// ��grandΪ��������¸�
		else root_ = p;
	}
	// uncleΪ��, ��3��Ⱦɫ�����ϼ������
	else {
		uncle->color = black;
		// �ڸ߶�+1
		++uncle->height;
		parent->color = black; 
		// �ڸ߶�+1
		++parent->height;
		// ��grandΪ���� ����ĺڸ߶�+1
		if (root_ == grand) ++grand->height;
		// ��grand��Ϊ������ȾΪ��ɫ
		else grand->color = red;
		// ��һ����ܻ��ᷢ��˫��ȱ�ݣ��������
		doubleRed(grand);
	}
}

Node*& RBTree::insert(int data) {
	Node*& node = search(data);
	if (node) return node;
	// ��hot_ΪNULL, �������Ϊ��������ɫΪ�ڣ��ڸ߶�1
	if (!hot_) node = new Node(data, hot_, NULL, NULL, 1, black);
	// ���򣬲����Ϊ��Ҷ��Ĭ����ɫΪ�죬�ڸ߶�0
	else {
		node = new Node(data, hot_, NULL, NULL, 0, red);
		// ����Ƿ���˫��ȱ��
		doubleRed(node);
	}
	++size_;
	return node;
}

bool RBTree::remove(int data) {
	Node *&node = search(data);
	// û�ҵ�Ҫɾ���Ľڵ㣬ɾ��ʧ��
	if (!node) return false;
	// ɾ��, result�ǽ�����Ǹ��ڵ�
	Node *result = removeAt(node);
	--size_;
	// ���1. ɾ����Ϊ����
	if (size_ == 0) return true;
	// ���2. ��ʵ��ɾ�����Ǹ�
	if (!hot_) {
		// �¸�����Ϊ��ɫ
		result->color = black;
		updateHeight(result);
		return true;
	}
	// ���3. ��ɾ���ڵ��, ���ױ�Ϊ��, ��������������
	if ( isBlack(hot_) // ����Ϊ�� 
			 && 
		 getHeight(hot_) == getHeight(result) + 1) // ͨ���ڸ߶��жϱ�ɾ���ڵ���ɫΪ��
		return true;
	// ���4. ��ɾ���ڵ��, �к���
	if (isRed(result)) {
		result->color = black;
		++result->height;
		return true;
	}
	// ���5. ��ɾ���ڵ�����޺���
	else doubleBlack(result);
	return true;
}

void RBTree::doubleBlack(Node* node) {
	Node* p = (node) ? node->parent : hot_;
	if (!p) return;
	Node* sibling = (p->left == node) ? p->right : p->left;
	Node* nephew = (isRed(sibling->right)) ? sibling->right : sibling->left;
	// 1. ���ֵ�Ϊ��
	if (isBlack(sibling)) {
		// 1.1 ����ֶ��Ϊ��
		if (isRed(nephew)) {
			Node* grand = p->parent;
			bool c = p->color;
			// ��ֶ����ת tempΪ�����ĸ�
			Node* temp = rotateAt(nephew);
			// ��Ⱦɫ
			if (temp->left) temp->left->color = black;
			updateHeight(temp->left);
			if (temp->right) temp->right->color = black;
			updateHeight(temp->right);
			temp->color = c;
			updateHeight(temp);
			// �ؽ�������������
			if (grand) {
				if (grand->left == p) grand->left = temp;
				else grand->right = temp;
			}
			else root_ = temp;
			return;
		}
		// 1.2 ��ֶ�Ӷ�Ϊ��ɫ
		// ���ֵ�ת��
		sibling->color = red;
		--sibling->height;
		// 1.2.1 ��parentΪ��ɫ BB-2-R
		if (isRed(p))  p->color = black;
		// 1.2.2 ��parentΪ��ɫ BB-2-B
		else {
			--p->height;
			doubleBlack(p);
		}
	}
	else {
		// sibling Ϊ��ɫ����� BB-3
		// �޸���ɫ
		p->color = red;
		sibling->color = black;
		// �����siblingͬ���t
		Node* t = (sibling->isLeft()) ? sibling->left : sibling->right;
		// ��ʱ��¼�游�ڵ㣬����һ���������
		Node* grand = p->parent;
		// ��¼��ǰ�ĸ��ڵ�
		hot_ = p;
		// ��ת
		Node* temp = rotateAt(t);
		// ����
		if (grand) {
			if (grand->left == p) grand->left = temp;
			else grand->right = temp;
		}
		else root_ = temp;
		// ת������������´���
		doubleBlack(node);
	}
}