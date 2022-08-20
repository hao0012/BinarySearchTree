#include"btree.h"

void BTree::overflow(Node* node)
{
	if (node) {
		vector<int>& store = node->data_;
		vector<Node*>& child = node->child_;
		int size = store.size();
		if (size == order_) {
			// 找到中间位置及数据:
			int mi = size / 2;
			int mid = node->data_[mi];
			// 创建一个新的分支:
			// 1. 剪切右半侧的数据到一个新数组
			vector<int> rightData = vector<int>(store.begin() + mi + 1, store.end());
			store.erase(store.begin() + mi, store.end());
			// 2. 剪切右半侧的子节点到一个新数组
			vector<Node*> rightChild = vector<Node*>(child.begin() + mi + 1, child.end());
			child.erase(child.begin() + mi + 1, child.end());
			// 3. 用1,2的数据和子节点创建新节点
			Node* right = new Node(rightData, rightChild, node->parent_);
			// 4. 如果不在叶节点，孩子不为空 则将孩子的父亲设为right
			if (rightChild[0]) { 
				for (auto& child : rightChild) 
					child->parent_ = right;
			}
			// 将mid插入父节点:
			// (1) 如果node没有父节点:
			if (!node->parent_) {
				// 那么创建父节点，mid成为父节点数据
				Node* root = new Node(mid, node, right); 
				node->parent_ = root;
				right->parent_ = root;
				root_ = root; // 新root
			}
			// (2) 如果node有父节点
			else { 
				vector<int>& parentData = node->parent_->data_;
				// 1. 在父节点中找到插入mid的位置
				int i = find(parentData, mid);
				// 2. 父节点插入mid
				parentData.insert(parentData.begin() + i, mid);
				// 3. 父节点的child中记录right节点
				node->parent_->child_.insert(node->parent_->child_.begin() + i + 1, right);
			}
			// 如果父节点数据个数超标，那么对父节点再进行同样操作
			overflow(node->parent_);
		}
	}
}

// 返回store中不小于data的最小值下标
template<typename T>
int BTree::find(vector<T>& store, T data)
{
	int size = store.size();
	int i = 0;
	for (; i < size; ++i)
		if (store[i] >= data) break;
	return i;
}
// 将向量v分为[0, point)、point、(point, end]三部分
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
	if (node) return false; // 如果已有则插入失败
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
	if (node == root_) { // 若当前为根
		if (node->data_.size() == 0) { // 若根中无数据
			if (node->child_[0]) { // 若根的子有数据
				root_ = node->child_[0]; // 就将子置为新根
				root_->parent_ = NULL;
				node->child_[0] = NULL;
			}
			else root_ = NULL; // 如果子也没有数据， 那么整棵树为空
			delete node;
		}
		return;
	}
	vector<int>& store = node->data_;
	int size = store.size();
	if (size == (order_ + 1) / 2 - 2) {
		// node的兄弟
		vector<Node*>& brothers = node->parent_->child_;
		int bsize = brothers.size();
		// 找到node在brothers中的位置下标
		int i = 0;
		for (; i < bsize; ++i) if (brothers[i] == node) break;
		// 1. 如果有左兄弟且左兄弟元素足够
		if (i > 0) { 
			int brotherDataSize = brothers[i - 1]->data_.size();// 左兄弟中元素个数
			int brotherChildSize = brothers[i - 1]->child_.size(); // 左兄弟中孩子个数
			if (brotherDataSize >= (order_ + 1) / 2) { // 左兄弟元素足够 那么从左兄弟借一个
				// 将父放在node的元素的开头
				node->data_.insert(node->data_.begin(), node->parent_->data_[i - 1]); 
				// 父变为借来的那个元素
				node->parent_->data_[i - 1] = brothers[i - 1]->data_[brotherDataSize - 1];
				// 左兄弟中删除被借走的那个元素
				brothers[i - 1]->data_.pop_back();
				// 左兄弟的最后一个孩子给node作第一个孩子
				node->child_.insert(node->child_.begin(), brothers[i - 1]->child_[brotherChildSize - 1]);
				// 删除左兄弟的最后一个孩子
				brothers[i - 1]->child_.pop_back();
				return;
			}
		}
		// 2. 如果有右兄弟且元素足够
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
		// 3. 如果有左兄弟，合并
		if (i > 0) { 
			Node* left = brothers[i - 1];
			vector<int>& data = left->data_;
			vector<Node*>& child = left->child_;
			// 1. 数据合并
			data.insert(data.end(), node->parent_->data_[i - 1]);
			data.insert(data.end(), store.begin(), store.end());
			// 2. 孩子合并
			child.insert(child.end(), node->child_.begin(), node->child_.end());
			// 3. 删除合并后多余的节点
			node->parent_->data_.erase(node->parent_->data_.begin() + i - 1);
			brothers.erase(brothers.begin() + i);
			delete node;
			// 处理下溢
			underflow(left->parent_);
			return;
		}
		// 4. 如果有右兄弟，合并
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
	if (!node) return false; // 如果没找到则删除失败
	vector<int>& store = node->data_;
	int i = find(store, data);
	if (node->child_[0]) { // 如果数据不在叶节点，那么
		// 找到后继元素
		Node* successor = node->child_[i + 1];
		while (successor->child_[0]) successor = successor->child_[0];
		// 与后继交换数据
		store[i] = successor->data_[0];
		// 待删除对象变为后继successor->data_[0]
		i = 0;
		node = successor;
	}
	// 删除
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