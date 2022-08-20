#include"btree.h"

void BTree::overflow(Node* node)
{
	if (node) {
		vector<int>& store = node->data_;
		vector<Node*>& child = node->child_;
		int size = store.size();
		if (size == order_) {
			// �ҵ��м�λ�ü�����:
			int mi = size / 2;
			int mid = node->data_[mi];
			// ����һ���µķ�֧:
			// 1. �����Ұ������ݵ�һ��������
			vector<int> rightData = vector<int>(store.begin() + mi + 1, store.end());
			store.erase(store.begin() + mi, store.end());
			// 2. �����Ұ����ӽڵ㵽һ��������
			vector<Node*> rightChild = vector<Node*>(child.begin() + mi + 1, child.end());
			child.erase(child.begin() + mi + 1, child.end());
			// 3. ��1,2�����ݺ��ӽڵ㴴���½ڵ�
			Node* right = new Node(rightData, rightChild, node->parent_);
			// 4. �������Ҷ�ڵ㣬���Ӳ�Ϊ�� �򽫺��ӵĸ�����Ϊright
			if (rightChild[0]) { 
				for (auto& child : rightChild) 
					child->parent_ = right;
			}
			// ��mid���븸�ڵ�:
			// (1) ���nodeû�и��ڵ�:
			if (!node->parent_) {
				// ��ô�������ڵ㣬mid��Ϊ���ڵ�����
				Node* root = new Node(mid, node, right); 
				node->parent_ = root;
				right->parent_ = root;
				root_ = root; // ��root
			}
			// (2) ���node�и��ڵ�
			else { 
				vector<int>& parentData = node->parent_->data_;
				// 1. �ڸ��ڵ����ҵ�����mid��λ��
				int i = find(parentData, mid);
				// 2. ���ڵ����mid
				parentData.insert(parentData.begin() + i, mid);
				// 3. ���ڵ��child�м�¼right�ڵ�
				node->parent_->child_.insert(node->parent_->child_.begin() + i + 1, right);
			}
			// ������ڵ����ݸ������꣬��ô�Ը��ڵ��ٽ���ͬ������
			overflow(node->parent_);
		}
	}
}

// ����store�в�С��data����Сֵ�±�
template<typename T>
int BTree::find(vector<T>& store, T data)
{
	int size = store.size();
	int i = 0;
	for (; i < size; ++i)
		if (store[i] >= data) break;
	return i;
}
// ������v��Ϊ[0, point)��point��(point, end]������
template<typename T> 
vector<T>& BTree::split(vector<T>& v, int point, bool reservePoint)
{
	vector<T>* rightPart = new vector<T>(v.begin() + point + 1, v.end());
	if (reservePoint) v.erase(v.begin() + point + 1, v.end());
	else v.erase(v.begin() + point, v.end());
	return *rightPart;
}

Node* BTree::searchIn(Node* root, int data)
{
	if (!root) return NULL;
	vector<int> store = root->data_;
	int i = find(store, data);
	if (i < store.size() && store[i] == data) return root;
	hot_ = root;
	return searchIn(root->child_[i], data);
}

Node* BTree::search(int data)
{
	hot_ = NULL;
	return searchIn(root_, data);
}

bool BTree::insert(int data)
{
	Node* node = search(data);
	if (node) return false; // ������������ʧ��
	if (!hot_) root_ = new Node(data);
	else {
		vector<int>& store = hot_->data_;
		int i = find(store, data);
		store.insert(store.begin() + i, data);
		hot_->child_.insert(hot_->child_.begin() + i + 1, NULL);
	}
	++size_;
	overflow(hot_);
	return true;
}
void BTree::underflow(Node* node)
{
	if (!node) return;
	if (node == root_) { // ����ǰΪ��
		if (node->data_.size() == 0) { // ������������
			if (node->child_[0]) { // ��������������
				root_ = node->child_[0]; // �ͽ�����Ϊ�¸�
				root_->parent_ = NULL;
				node->child_[0] = NULL;
			}
			else root_ = NULL; // �����Ҳû�����ݣ� ��ô������Ϊ��
			delete node;
		}
		return;
	}
	vector<int>& store = node->data_;
	int size = store.size();
	if (size == (order_ + 1) / 2 - 2) {
		// node���ֵ�
		vector<Node*>& brothers = node->parent_->child_;
		int bsize = brothers.size();
		// �ҵ�node��brothers�е�λ���±�
		int i = 0;
		for (; i < bsize; ++i) if (brothers[i] == node) break;
		// 1. ��������ֵ������ֵ�Ԫ���㹻
		if (i > 0) { 
			int brotherDataSize = brothers[i - 1]->data_.size();// ���ֵ���Ԫ�ظ���
			int brotherChildSize = brothers[i - 1]->child_.size(); // ���ֵ��к��Ӹ���
			if (brotherDataSize >= (order_ + 1) / 2) { // ���ֵ�Ԫ���㹻 ��ô�����ֵܽ�һ��
				// ��������node��Ԫ�صĿ�ͷ
				node->data_.insert(node->data_.begin(), node->parent_->data_[i - 1]); 
				// ����Ϊ�������Ǹ�Ԫ��
				node->parent_->data_[i - 1] = brothers[i - 1]->data_[brotherDataSize - 1];
				// ���ֵ���ɾ�������ߵ��Ǹ�Ԫ��
				brothers[i - 1]->data_.pop_back();
				// ���ֵܵ����һ�����Ӹ�node����һ������
				node->child_.insert(node->child_.begin(), brothers[i - 1]->child_[brotherChildSize - 1]);
				// ɾ�����ֵܵ����һ������
				brothers[i - 1]->child_.pop_back();
				return;
			}
		}
		// 2. ��������ֵ���Ԫ���㹻
		if (i < brothers.size() - 1) { 
			int brotherDataSize = brothers[i + 1]->data_.size(); 
			if (brotherDataSize >= (order_ + 1) / 2) { 
				node->data_.insert(node->data_.end(), node->parent_->data_[i]);
				node->parent_->data_[i] = brothers[i + 1]->data_[0];
				brothers[i + 1]->data_.erase(brothers[i + 1]->data_.begin());
				node->child_.push_back(brothers[i + 1]->child_[0]);
				brothers[i + 1]->child_.erase(brothers[i + 1]->child_.begin());
				return;
			}
		}
		// 3. ��������ֵܣ��ϲ�
		if (i > 0) { 
			Node* left = brothers[i - 1];
			vector<int>& data = left->data_;
			vector<Node*>& child = left->child_;
			// 1. ���ݺϲ�
			data.insert(data.end(), node->parent_->data_[i - 1]);
			data.insert(data.end(), store.begin(), store.end());
			// 2. ���Ӻϲ�
			child.insert(child.end(), node->child_.begin(), node->child_.end());
			// 3. ɾ���ϲ������Ľڵ�
			node->parent_->data_.erase(node->parent_->data_.begin() + i - 1);
			brothers.erase(brothers.begin() + i);
			delete node;
			// ��������
			underflow(left->parent_);
			return;
		}
		// 4. ��������ֵܣ��ϲ�
		if (i < brothers.size() - 1) { 
			Node* right = brothers[i + 1];
			vector<int>& data = right->data_;
			vector<Node*>& child = right->child_;
			node->data_.insert(node->data_.end(), node->parent_->data_[i]);
			node->data_.insert(node->data_.end(), data.begin(), data.end());
			node->child_.insert(node->child_.end(), child.begin(), child.end());
			node->parent_->data_.erase(node->parent_->data_.begin() + i);
			brothers.erase(brothers.begin() + i + 1);
			delete right;
			underflow(node->parent_);
			return;
		}
	}
}

bool BTree::remove(int data)
{
	Node* node = search(data);
	if (!node) return false; // ���û�ҵ���ɾ��ʧ��
	vector<int>& store = node->data_;
	int i = find(store, data);
	if (node->child_[0]) { // ������ݲ���Ҷ�ڵ㣬��ô
		// �ҵ����Ԫ��
		Node* successor = node->child_[i + 1];
		while (successor->child_[0]) successor = successor->child_[0];
		// ���̽�������
		store[i] = successor->data_[0];
		// ��ɾ�������Ϊ���successor->data_[0]
		i = 0;
		node = successor;
	}
	// ɾ��
	node->data_.erase(node->data_.begin() + i);
	node->child_.erase(node->child_.begin() + i);
	--size_;
	underflow(node);
	return true;
}

ostream& BTree::operator<<(ostream& out)
{
	if (!root_) return out;
	queue<Node*> q;
	q.emplace(root_);
	while (!q.empty()) {
		int size = q.size();
		for (int i = 0; i < size; ++i) {
			Node* f = q.front();
			q.pop();
			for (auto& child : f->child_) 
				if(child) q.emplace(child);
			for (auto& data : f->data_) 
				out << data << " ";
			out << " ";
		}
		out << '\n';
	}
	return out;
}