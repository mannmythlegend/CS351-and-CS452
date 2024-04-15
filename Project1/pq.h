template <typename T>
class node {
	public:
		T data;
		node * left;
		node * right;
		node (T d) {
			data = d;
			left = NULL;
			right = NULL;
		}
};

template <typename T> 
class pq {
	private:
		node<T> * root;
		
	public:
		pq() { root = NULL; }
		bool empty() const;
		void enqueue (T);
		T dequeue();
		void print(node<T>*) const;
		void print() const { print(root); }
};

template <typename T> 
bool pq<T>::empty() const {
	return (root == NULL);
}

template <typename T> 
void pq<T>::enqueue(T input) {
	node<T> * inputnode = new node<T>(input);
	if (empty()) {
		root = inputnode;
		return;
	}

	node<T> * currNode = root;
	node<T> * parent = NULL;
	while (currNode != NULL) {
		parent = currNode;
		if (currNode->data < inputnode->data)
			currNode = currNode->right;
		else
			currNode = currNode->left;
	}
	
	// parent now has the location where the insertion needs to take place
	if (parent->data < inputnode->data)
		parent->right = inputnode;
	else
		parent->left = inputnode;
	
	return;
} // end enqueue

template <typename T> 
T pq<T>::dequeue() {
	if (empty())
		return T();

	node<T> * parent = NULL;
	node<T> * currNode = root;
	T answer;
	
	// find max data in the tree
	while (currNode->right != NULL) {
		parent = currNode;
		currNode = currNode->right;
	}

	// the correct answer is the currNode's data
	answer = currNode->data;

	// currNode is the root
	if (currNode == root) {
		if (currNode->left == NULL) { // root is the last node left
			root = NULL;
			delete currNode;
			return answer;
		}
		else { // currNode only has left children
			root = currNode->left;
			delete currNode;
			return answer;
		}
	}
	
	// currNode is a leaf
	if (currNode->left == NULL) {
		delete currNode;
		parent->right = NULL;
		return answer;
	}
	else { // currNode has a left tree
		// sets parent's value correctly
		parent->right = currNode->left;
		delete currNode;
		return answer;
	}
} // end dequeue

template <typename T> 
void pq<T>::print(node<T> * p) const {
	if (p != NULL) {
		p->data.print();
		print(p->left);
		print(p->right);
	}
	return;
}