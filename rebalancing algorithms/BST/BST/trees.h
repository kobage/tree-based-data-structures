#pragma once
#include<iostream>
#include"node.h"
#include"math.h"
using namespace std;

template<typename T>
class Tree
{
	Node<T>* root;
	int size;
	int fullSize(int size);
	void compression(Node<T>* root, int count);
	void vine_to_tree(Node<T>* root, int size);
	void destroyTree(Node<T>* node);
	Node<T>* qBalance(Node<T>*& node, int m);
	int tree_to_vine(Node<T>* r);
public:
	Tree();
	~Tree();
	int getSize() { return size; }
	Node<T>* getRoot() { return root; }
	void insert(Node<T>* z);
	Node<T>* search(T key);
	void balanceDSW();
	void qBalance();
};

template<typename T>
Tree<T>::Tree()
{
	root = nullptr;
	size = 0;
}

template<typename T>
Tree<T>::~Tree()
{
	destroyTree(root);
}

template<typename T>
void Tree<T>::insert(Node<T>* z)
{
	size++;
	Node<T>* x(root);
	Node<T>* y(nullptr);
	while (nullptr != x)
	{
		y = x;
		x = x->child[z->key >= x->key];
	}
	if (y == nullptr)
		root = z;
	else
		y->child[z->key >= y->key] = z;
}

template<typename T>
void Tree<T>::destroyTree(Node<T>* node)
{
	Node<T>* pseudo_root = new Node<T>();
	pseudo_root->child[1] = node;
	tree_to_vine(pseudo_root);
	node = pseudo_root->child[1];

	while (node != nullptr)
	{
		delete pseudo_root;
		pseudo_root = node;
		node = node->child[1];
	}
}
template<typename T>
Node<T>* Tree<T>::search(T key)
{
	Node<T>* x = root;
	while (nullptr != x  && x->key != key)
		x = x->child[key >= x->key];
	return x;
}

template<typename T>
int Tree<T>::tree_to_vine(Node<T>* r)
{
	Node<T>* vineTail = r;
	Node<T>* remainder = vineTail->child[1];

	int number = 0;
	Node<T>* tempPtr;
	while (remainder != nullptr)
	{
		// თუ მარცხენა შვილი აღარ ჰყავს, იტერაცია გადადის მიმდინარე კვანძის მარჯვენა შვილზე
		//If there is no left child, iteration goes to the right-hand son of the current node
		if (remainder->child[0] == nullptr)
		{
			vineTail = remainder;
			remainder = remainder->child[1];
			number++;
		}
		// შეამცირე მარცხენა ქვეხე მარჯვნივ მობრუნებით
		//Reduce the left-hand side using right-rotation
		else
		{
			tempPtr = remainder->child[0];
			remainder->child[0] = tempPtr->child[1];
			tempPtr->child[1] = remainder;
			remainder = tempPtr;
			vineTail->child[1] = tempPtr;
		}
	}
	return number;
}

template<typename T>
int Tree<T>::fullSize(int size)
{
	int n = 1;
	while (n <= size) {
		n = n + n + 1;
	}
	return n / 2;
}

template<typename T>
void Tree<T>::compression(Node<T>* root, int count)
{
	Node<T>* scanner = root;
	// მობრუნება მარცხნივ
	for (int i = 0; i < count; i++)
	{
		Node<T>* child = scanner->child[1];
		scanner->child[1] = child->child[1];
		scanner = scanner->child[1];
		child->child[1] = scanner->child[0];
		scanner->child[0] = child;
	}
}

template<typename T>
void Tree<T>::vine_to_tree(Node<T>* root, int size)
{
	int fullCount = fullSize(size);
	compression(root, size - fullCount);
	for (size = fullCount; size > 1; size /= 2) {
		compression(root, size / 2);
	}
}

template<typename T>
void Tree<T>::balanceDSW()
{
	Node<T>* pseudo_root = new Node<T>();
	pseudo_root->child[1] = root;

	int n = tree_to_vine(pseudo_root);
	vine_to_tree(pseudo_root, n);
	root = pseudo_root->child[1];
}

template<typename T>
Node<T>* Tree<T>::qBalance(Node<T>*& node, int m)
{
	if (m == 2)
	{
		Node<T>* tmp = node->child[1];
		tmp->child[0] = node;
		node->child[0] = node->child[1] = nullptr;
		node = tmp->child[1];
		tmp->child[1] = nullptr;
		return tmp;
	}
	if (m == 1)
	{
		Node<T>* tmp = node;
		node = node->child[1];
		tmp->child[0] = tmp->child[1] = nullptr;
		return tmp;
	}
	int q = m / 2;
	Node<T>* a = qBalance(node, q);
	Node<T>* b = node;
	b->child[0] = a;
	node = node->child[1];
	a = qBalance(node, m - q - 1);
	b->child[1] = a;

	return b;
}

template<typename T>
void Tree<T>::qBalance()
{
	Node<T>* pseudo_root = new Node<T>();
	pseudo_root->child[1] = root;
	tree_to_vine(pseudo_root);

	root = qBalance(pseudo_root->child[1], size);
	delete pseudo_root;
}