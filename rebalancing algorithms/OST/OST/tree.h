#pragma once
#include<iostream>
#include"Node.h"
using namespace std;

template<typename T>
class Tree
{
	Node<T>* root;
	int size;
	int N(Node<T>* n);
	Node<T>* ostRotate(Node<T>* x, bool dir);
	void insertOST(Node<T>* z);
	Node<T>* balanceR(Node<T>* h);
	int mateInt;
	int mateCounter;
	Node<T>* qBalance(Node<T>*& node, int m);
	Node<T>* partR(Node<T>* h, int k);
	int tree_to_vine(Node<T>* r);
	void Tree<T>::destroyTree(Node<T>* node);
public:
	Tree();
	~Tree();
	Node<T>* getRoot() { return root; }
	int getSize() { return size; }
	Node<T>* select(Node<T>* node, int i);
	void insert(Node<T>* z);
	Node<T>* search(T k);
	void balance();
	void qBalance();
};

template<typename T>
Tree<T>::Tree()
{
	root = nullptr;
	size = 0;
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
		//If there is no left child, the iteration goes to the right-hand child of the current node
		if (remainder->child[0] == nullptr)
		{
			vineTail = remainder;
			remainder = remainder->child[1];
			number++;
		}
		// შეამცირე მარცხენა ქვეხე მარჯვნივ მობრუნებით
		//Reduce the left-hand side using the right-rotation
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
Tree<T>::~Tree()
{
	destroyTree(root);
}

template<typename T>
int Tree<T>::N(Node<T>* n)
{
	if (nullptr == n) return 0;
	return n->bf;
}

template<typename T>
Node<T>* Tree<T>::ostRotate(Node<T>* x, bool dir)
{
	Node<T>* y = x->child[!dir];
	x->child[!dir] = y->child[dir];
	if (x == root)  root = y;
	y->child[dir] = x;
	x->bf = N(x->child[0]) + N(x->child[1]) + 1;
	y->bf = N(y->child[0]) + N(y->child[1]) + 1;
	return y;
}

template<typename T>
void Tree<T>::insertOST(Node<T>* z)
{
	size++;
	Node<T>* x(root);
	Node<T>* y(nullptr);
	while (nullptr != x)
	{
		++x->bf;
		y = x;
		x = x->child[z->key >= x->key];
	}
	if (y == nullptr)
		root = z;
	else
		y->child[z->key >= y->key] = z;
}

template<typename T>
void Tree<T>::insert(Node<T>* z)
{
	insertOST(z);
}

template<typename T>
Node<T>* Tree<T>::search(T k)
{
	Node<T>* x = root;
	while (nullptr != x && k != x->key)
		x = x->child[k >= x->key];
	return x;
}

template<typename T>
Node<T>* Tree<T>::select(Node<T>* node, int i)
{
	if (i<0 || i > node->bf)
	{
		cout << "Error!" << endl;
		return nullptr;
	}
	int r = 1 + N(node->child[0]);
	if (i == r) return node;
	if (i < r) return select(node->child[0], i);
	return select(node->child[1], i - r);
}

//----------------------------Sedgewick's algorithm------------------------
template<typename T>
Node<T>* Tree<T>::partR(Node<T>* h, int k)
{
	int t = N(h->child[0]);

	if (t != k)
	{
		int dir = (t < k) ? 1 : 0;
		h->child[dir] = partR(h->child[dir], k - dir*(t + 1));
		h = ostRotate(h, !dir);
	}
	return h;
}

template<typename T>
Node<T>* Tree<T>::balanceR(Node<T>* h)
{
	if (h == nullptr || h->bf < 2) return h;

	h = partR(h, h->bf / 2);
	h->child[0] = balanceR(h->child[0]);
	h->child[1] = balanceR(h->child[1]);
	return h;
}

template<typename T>
void Tree<T>::balance() {
	root = balanceR(root);
}

//---------------------------------- qBalance, looks like qSort
template<typename T>
Node<T>* Tree<T>::qBalance(Node<T>*& node, int m)
{
	if (m == 2)
	{
		Node<T>* tmp = node->child[1];
		tmp->child[0] = node;
		node->child[0] = node->child[1] = nullptr;
		node->bf = 1;
		node = tmp->child[1];
		tmp->child[1] = nullptr;
		tmp->bf = 2;
		return tmp;
	}
	if (m == 1)
	{
		Node<T>* tmp = node;
		node = node->child[1];
		tmp->child[0] = tmp->child[1] = nullptr;
		tmp->bf = 1;
		return tmp;
	}
	int q = m / 2;
	Node<T>* a = qBalance(node, q);
	Node<T>* b = node;
	b->child[0] = a;
	node = node->child[1];
	a = qBalance(node, m - q - 1);
	b->child[1] = a;
	b->bf = N(b->child[0]) + N(b->child[1]) + 1;
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