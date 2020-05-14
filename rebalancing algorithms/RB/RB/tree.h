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
	int mateInt;
	int mateCounter;
	Node<T>* rotate(Node<T>* x, bool dir);
	Node<T>* ostRotate(Node<T>* x, bool dir);   //Precondition: bf must contain size of subtree
	void insertBST(Node<T>* z);
	void RBinsertFixup(Node<T>* z);
	Node<T>* balanceR(Node<T>* h);				//Precondition: bf must contain size of subtree
	Node<T>* balanceMod(Node<T>* h);			//Precondition: bf must contain size of subtree
	Node<T>* qBalance(int l, int r);
	Node<T>* qBalanceComplete(int, int);
	Node<T>* partR(Node<T>* h, int k);			//Precondition: bf must contain size of subtree
	void treeToList(Node<T>* x);
	void updateColors(Node<T>*, int);
	int updateSizes(Node<T>* h);
	Node<T>* head;
	void Tree<T>::destroyTree(Node<T>* node);

public:
	Tree();
	~Tree();
	Node<T>* getRoot() { return root; }
	int getSize() { return size; }
	Node<T>* select(Node<T>* node, int i);
	void insert(Node<T>* z);
	Node<T>* search(T k);
	void balanceMod();				// creates complete tree
	void qBalanceComplete();			// creates complete tree
};

template<typename T>
Tree<T>::Tree()
{
	root = nullptr;
	size = 0;
	head = nullptr;
}

template<typename T>
void Tree<T>::destroyTree(Node<T>* node) {
	if (node != nullptr) {
		destroyTree(node->child[1]);
		destroyTree(node->child[0]);
		delete node;
	}
}

template<typename T>
Tree<T>::~Tree()
{
	destroyTree(root);
}

//Precondition: bf must contain size of subtree
template<typename T>
int Tree<T>::N(Node<T>* n)
{
	if (nullptr == n) return 0;
	return n->bf;
}

template<typename T>
Node<T>* Tree<T>::rotate(Node<T>* x, bool dir)
{
	Node<T>* y = x->child[!dir];
	x->child[!dir] = y->child[dir];
	if (nullptr != y->child[dir])  y->child[dir]->p = x;
	y->p = x->p;
	if (x->p == nullptr)  root = y;
	else x->p->child[x == x->p->child[1]] = y;
	y->child[dir] = x;
	x->p = y;
	return y;
}

//Precondition: bf must contain size of subtree
template<typename T>
Node<T>* Tree<T>::ostRotate(Node<T>* x, bool dir)
{
	Node<T>* y = x->child[!dir];
	x->child[!dir] = y->child[dir];
	if (nullptr != y->child[dir])  y->child[dir]->p = x;
	y->p = x->p;
	if (x->p == nullptr)  root = y;
	else x->p->child[x == x->p->child[1]] = y;
	y->child[dir] = x;
	x->p = y;

	x->bf = N(x->child[0]) + N(x->child[1]) + 1;
	y->bf = N(y->child[0]) + N(y->child[1]) + 1;

	return y;
}

template<typename T>
void Tree<T>::insertBST(Node<T>* z)
{
	size++;
	Node<T>* x(root);
	Node<T>* y(nullptr);
	while (nullptr != x)
	{
		y = x;
		x = x->child[z->key >= x->key];
	}
	z->p = y;
	if (y == nullptr)
		root = z;
	else
		y->child[z->key >= y->key] = z;
}

template<typename T>
void Tree<T>::RBinsertFixup(Node<T>* z)
{
	while (z != root && z->p->bf == 0)
	{
		bool side(z->p->key >= z->p->p->key);
		Node<T>* y = z->p->p->child[!side];
		if (nullptr != y && y->bf == 0)
		{
			z->p->bf = 1;
			y->bf = 1;
			z->p->p->bf = 0;
			z = z->p->p;
			continue;
		}
		if (z == z->p->child[!side])
		{
			z = z->p;
			rotate(z, side);
		}
		z->p->bf = 1;
		z->p->p->bf = 0;
		rotate(z->p->p, !side);
	}
	root->bf = 1;
}

template<typename T>
Node<T>* Tree<T>::search(T k)
{
	Node<T>* x = root;
	while (nullptr != x && k != x->key)
		x = x->child[k >= x->key];
	return x;
}

//Precondition: bf must contain color
template<typename T>
void Tree<T>::insert(Node<T>* z)
{
	insertBST(z);
	RBinsertFixup(z);
}

template<typename T>
int Tree<T>::updateSizes(Node<T>* h)
{
	if (h == nullptr) return 0;

	int lN = updateSizes(h->child[0]);
	int rN = updateSizes(h->child[1]);
	h->bf = 1 + lN + rN;

	return h->bf;
}

//Precondition: bf must contain size of subtree
template<typename T>
Node<T>* Tree<T>::select(Node<T>* node, int i)
{
	if (i<0 || i > node->bf)
	{
		cout << "Correct order!" << endl;
		return nullptr;
	}
	int r = 1 + N(node->child[0]);
	if (i == r) return node;
	if (i < r) return select(node->child[0], i);
	return select(node->child[1], i - r);
}

//----------------------------Sedgewick's algorithm------------------------
template<typename T>
void Tree<T>::updateColors(Node<T>* node, int maxHeight) {
	static int level = -1;
	if (node != nullptr) {
		level++;
		node->bf = level < maxHeight ? 1 : 0;
		updateColors(node->child[0], maxHeight);
		updateColors(node->child[1], maxHeight);
		level--;
	}
}

//Precondition: bf must contain size of subtree
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
	if (nullptr != h->child[0])   h->child[0]->p = h;
	h->child[1] = balanceR(h->child[1]);
	if (nullptr != h->child[1])   h->child[1]->p = h;
	return h;
}

//------------------------------Modified Sedgewick's algorithm--------------------------------
template<typename T>
Node<T>* Tree<T>::balanceMod(Node<T>* h)
{
	if (h == nullptr || h->bf < 2) return h;

	int height = (int)log2(h->bf);
	if (h->bf <= 3 * (int)pow(2, height - 1) - 1) {
		h = partR(h, h->bf - (int)pow(2, height - 1));
		h->child[1] = balanceR(h->child[1]);
		if (nullptr != h->child[0])   h->child[0]->p = h;
		h->child[0] = balanceMod(h->child[0]);
		if (nullptr != h->child[1])   h->child[1]->p = h;
	}
	else {
		h = partR(h, (int)pow(2, height) - 1);
		h->child[1] = balanceMod(h->child[1]);
		if (nullptr != h->child[0])   h->child[0]->p = h;
		h->child[0] = balanceR(h->child[0]);
		if (nullptr != h->child[1])   h->child[1]->p = h;
	}
	return h;
}

template<typename T>
void Tree<T>::balanceMod()
{
	updateSizes(root);
	root = balanceMod(root);
	int maxHeight = (int)log2(size);
	updateColors(root, maxHeight);
}

//---------------------------------- qBalance, looks like qSort
template<typename T>
Node<T>* Tree<T>::qBalance(int st, int fin)
{
	if (fin < st)  return nullptr;
	if (fin == st)
	{
		Node<T>* tmp = head;
		head = head->child[1];
		tmp->child[0] = tmp->child[1] = nullptr;
		return tmp;
	}
	int q = (st + fin) / 2;
	Node<T>* a = qBalance(st, q - 1);
	Node<T>* b = head;
	b->child[0] = a;
	if (nullptr != a)  a->p = b;
	head = head->child[1];
	a = qBalance(q + 1, fin);
	b->child[1] = a;
	if (nullptr != a)  a->p = b;
	return b;
}

template<typename T>
Node<T>* Tree<T>::qBalanceComplete(int st, int fin)
{
	if (fin < st)  return nullptr;
	if (fin == st)
	{
		Node<T>* tmp = head;
		head = head->child[1];
		tmp->child[0] = tmp->child[1] = nullptr;
		return tmp;
	}
	int n = fin - st + 1;
	int height = (int)log2(n);
	int q;
	Node<T>* a, *b;

	if (n <= 3 * (int)pow(2, height - 1) - 1)
	{
		q = st + n - (int)pow(2, height - 1);
		a = qBalanceComplete(st, q - 1);
		b = head;
		b->child[0] = a;
		if (nullptr != a)  a->p = b;
		head = head->child[1];
		a = qBalance(q + 1, fin);
		b->child[1] = a;
		if (nullptr != a)  a->p = b;
		return b;
	}
	q = st + (int)pow(2, height) - 1;
	a = qBalance(st, q - 1);
	b = head;
	b->child[0] = a;
	if (nullptr != a)  a->p = b;
	head = head->child[1];
	a = qBalanceComplete(q + 1, fin);
	b->child[1] = a;
	if (nullptr != a)  a->p = b;

	return b;
}

template<typename T>
void Tree<T>::treeToList(Node<T>* x)
{
	if (nullptr != x)
	{
		treeToList(x->child[1]);
		{
			x->child[1] = head;
			head = x;

			if ((size - mateCounter) / 2 < (size - mateInt) && (size - mateCounter) % 2 == 1)
				head->bf = 0;
			else
				head->bf = 1;
			++mateCounter;
		}
		treeToList(x->child[0]);
	}
}

template<typename T>
void Tree<T>::qBalanceComplete()
{
	int height = (int)log2(size);
	mateInt = pow(2, height) - 1;
	mateCounter = 0;
	treeToList(root);
	root = qBalanceComplete(0, size - 1);
}
