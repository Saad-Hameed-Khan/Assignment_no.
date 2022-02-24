/*****************************************************************************
 *                                                                           *
 * C++ implementation of binary search trees                                 *
 *                                                                           *
 * This software is (c) 2022 by                                              *
 * Jens Schneider,                                                           *
 * Division of Information and Computing Technology,                         *
 * College of Science and Engineering,                                       *
 * Hamad Bin Khalifa University,                                             *
 * Qatar Foundation, Doha Qatar                                              *
 *                                                                           *
 * It is distributed under the CC BY-NC-ND 4.0 licence:                      *
 * https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode               *
 * For use in commercial projects, contact mailto:jeschneider@hbku.edu.qa.   *
 *                                                                           *
 * If you use this software for academic projects,                           *
 * an acknowledgement will be appreciated.                                   *
 *                                                                           *
 *                                                  last update: 24/Jan/2022 *
 *****************************************************************************/
#pragma once
#include<string>
#include<sstream>
#include<exception>
#include<sstream>
#include<vector>

 // implementation of a binary search tree
template<class T = int>
class tree {
public:

	// This here is a dependent class definition.
	// It does not exist outside of tree<T>.
	// As a result, we have to tell C++ that whenever we want to use
	// tree<T>::node as a return type with a "typename" (see below).
	class node {
	public:
		node(void);
		node(const node& other);
		~node(void);
		bool is_leaf(void) const;
		// typename example to get rid of the "dependent name is not a type" compiler warning.
		typename const tree<T>::node& operator=(const tree<T>::node& other);
		void clear(void);
		int children(void) const;
		node* left, * right;
		T  key;
	};

	tree(void);
	tree(const tree& other);
	~tree(void);
	const tree& operator=(const tree& other);
	bool empty(void) const;
	typename tree<T>::node* find(const T& key) const;
	bool insert(const T& key);
	bool erase(const T& key);
	void clear(void);

	enum class order {
		inorder,
		preorder,
		postorder
	};
	std::vector<T> traverse(order o = order::inorder) const;
	std::string debuginfo(void) const;
	void swap(tree& other);
protected:
	node* root;
	std::vector<T> traversal_backend(const node* cur, order o) const;
	std::string debuginfo_backend(const node* cur) const;
	typename tree<T>::node* get_successor(node* del_node);
};

using iTree = tree<int>;

// =================== IMPLEMENTATION OF tree::node ==================

template<class T>
tree<T>::node::node(void) :
	left(nullptr),
	right(nullptr),
	key(T())
{}

template<class T>
tree<T>::node::node(const tree<T>::node& other) :
	left(nullptr),
	right(nullptr),
	key(T())
{
	*this = other;
}

template<class T>
tree<T>::node::~node(void) {
	clear();
}

template<class T>
bool tree<T>::node::is_leaf(void) const {
	return left == nullptr && right == nullptr;
}

template<class T>
typename const tree<T>::node& tree<T>::node::operator=(const tree<T>::node& other) {
	clear();
	key = other.key;
	// copy left subtree recursively
	if (other.left != nullptr) {
		left = new node;
		*left = *(other.left);
	}
	// copy right subtree recursively
	if (other.right != nullptr) {
		right = new node;
		*right = *(other.right);
	}
	return *this;
}

template<class T>
void tree<T>::node::clear(void) {
	// recursively delete left subtree
	if (left != nullptr) delete left;
	left = nullptr;
	// recursively delete right subtree
	if (right != nullptr) delete right;
	right = nullptr;
}

template<class T>
int tree<T>::node::children(void) const {
	// if a bool is converted to an int, that int will be
	// 1 for a true and 0 for a false
	return int(left != nullptr) + int(right != nullptr);
}

// =================== IMPLEMENTATION OF tree ==================

template<class T>
tree<T>::tree(void) :
	root(nullptr)
{}

template<class T>
tree<T>::tree(const tree<T>& other) {
	*this = other;
}

template<class T>
tree<T>::~tree(void) {
	clear();
}

template<class T>
void tree<T>::clear(void) {
	// trigger recursive deletion
	if (root != nullptr) delete root;
	root = nullptr;
}

template<class T>
const tree<T>& tree<T>::operator=(const tree<T>& other) {
	clear();
	if (other.root == nullptr) return *this;
	root = new node;
	// trigger recursive deep copy
	*root = *(other.root);
	return *this;
}

template<class T>
bool tree<T>::empty(void) const {
	return root == nullptr;
}

template<class T>
typename tree<T>::node* tree<T>::find(const T& key) const {
	node* cur = root;
	while (cur != nullptr && cur->key != key) {
		if (key < cur->key) cur = cur->left;	// smaller, go left
		else cur = cur->right;					// larger, go right
	}
	return cur;
}

template<class T>
bool tree<T>::insert(const T& key) {
	node* new_node = new node;
	new_node->key = key;
	if (root == nullptr) {
		root = new_node;
		return true;		// success inserting the node
	}
	node* cur = root;
	node* parent = root;
	// find the first key that is smaller than our key
	// the parent of that node then will become the parent of the
	// key we're trying to add
	while (true) {
		parent = cur;
		if (key == cur->key) return false;		// already in tree, can't insert
		if (key < cur->key) {					// smaller, go left
			cur = cur->left;
			if (cur == nullptr) {
				parent->left = new_node;
				return true;
			}
		}
		else {
			cur = cur->right;					// larger, go right
			if (cur == nullptr) {
				parent->right = new_node;
				return true;
			}
		}
	}
}

template<class T>
bool tree<T>::erase(const T& key) {
	if (root == nullptr) return false;
	node* cur = root;
	node* parent = root;

	// search for parent node
	while (cur->key != key) {
		parent = cur;
		if (key < cur->key) cur = cur->left; // go left
		else cur = cur->right; // go right
	}
	if (cur == nullptr) return false;
	bool isLeftChild = key < parent->key;

	// Case 1 - deleting a leaf node / zero children
	if (cur->is_leaf()) {
		if (cur == root) { // tree has only one node
			delete root;
			root = nullptr;
		}
		else {
			// disconnect from parent
			if (isLeftChild) parent->left = nullptr;
			else parent->right = nullptr;
			delete cur;
			cur = nullptr;
		}
		return true;
	}

	// Case 2 - internal node with one child
	if (cur->children() == 1) {
		if (cur->right == nullptr) {
			// no right child:
			if (cur == root) root = cur->left;
			else if (isLeftChild) parent->left = cur->left;
			else parent->right = cur->left;
			cur->left = nullptr; // disconnect child to prevent deep delete
			delete cur;
		}
		else {
			// no left child:
			if (cur == root) root = cur->right;
			else if (isLeftChild) parent->left = cur->right;
			else parent->right = cur->right;
			cur->right = nullptr; // disconnect child to prevent deep delete
			delete cur;
		}
		return true;
	}

	// Case 2 - internal node with two children
	node* successor = get_successor(cur);
	if (cur == root) root = successor;
	else if (isLeftChild) parent->left = successor;
	else parent->right = successor;
	successor->left = cur->left;
	cur->left = cur->right = nullptr; // disconnect children to prevent deep delete
	delete cur;
	return true;
}

template<class T>
typename tree<T>::node* tree<T>::get_successor(tree<T>::node* del_node) {
	node* parent = del_node;
	node* successor = del_node;
	node* cur = del_node->right;
	while (cur != nullptr) {
		parent = successor;
		successor = cur;
		cur = cur->left;
	}
	if (successor != del_node->right) {
		// add new connections
		parent->left = successor->right;
		successor->right = del_node->right;
	}
	return successor;
}

template<class T>
std::vector<T> tree<T>::traverse(order o) const {
	return traversal_backend(root, o);
}

template<class T>
std::vector<T> tree<T>::traversal_backend(const tree<T>::node* cur, order o) const {
	std::vector<T> result;
	if (cur == nullptr) return result;
	std::vector<T> ltree, rtree;
	if (cur->left != nullptr) ltree = traversal_backend(cur->left, o);
	if (cur->right != nullptr) rtree = traversal_backend(cur->right, o);
	switch (o) {
	case order::preorder:
		result.push_back(cur->key);
		result.insert(result.end(), ltree.begin(), ltree.end());
		result.insert(result.end(), rtree.begin(), rtree.end());
		return result;
	case order::inorder:
		result = ltree;
		result.push_back(cur->key);
		result.insert(result.end(), rtree.begin(), rtree.end());
		return result;
	case order::postorder:
		result = ltree;
		result.insert(result.end(), ltree.begin(), ltree.end());
		result.insert(result.end(), rtree.begin(), rtree.end());
		result.push_back(cur->key);
	default:
		throw std::runtime_error("order not supported");
	}
	return result;
}

template<class T>
std::string tree<T>::debuginfo(void) const {
	return debuginfo_backend(root);
}

template<class T>
std::string tree<T>::debuginfo_backend(const tree<T>::node* cur) const {
	if (cur == nullptr) return std::string();
	std::stringstream stream;
	stream << "@" << cur << ": key=" << cur->key << " ";
	if (cur->left == nullptr) stream << "left->null ";
	else stream << "left->" << cur->left->key << " @" << cur->left << " ";
	if (cur->right == nullptr) stream << "right->null ";
	else stream << "right->" << cur->right->key << " @" << cur->right << " ";
	stream << std::endl;
	return stream.str() + debuginfo_backend(cur->left) + debuginfo_backend(cur->right);
}

template<class T>
void tree<T>::swap(tree& other) {
	std::swap(root, other.root);
}

namespace std {
	template<class T>
	void swap(tree<T>& lhs, tree<T>& rhs) {
		lhs.swap(rhs);
	}
};