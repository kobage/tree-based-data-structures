#pragma once
#include<iostream>
#include"Node.h"
#include <thread>
#include <future>
using namespace std;

template<typename T>
class Tree
{
	Node<T>* root;
	int size;
	void Tree<T>::destroyTree(Node<T>* node);
	Node<T>* rotate(Node<T>* x, bool dir);
	void insertBST(Node<T>* z);
	void RBinsertFixup(Node<T>* z);
	Node<T>* qBalance(Node<T>*& node, int m, const bool direction);
	Node<T>* qBalanceComplete(Node<T>*& node, int m, const bool direction);
	void treeToList(Node<T>* x, Node<T>*& head, int mateInt, int &mateCounter, const bool direction);
	Node<T>* extremum(Node<T>* x, bool direction);
	Node<T>* next(Node<T>* x, bool direction);
	void treeToList_Colored(Node<T>*& head, int numberOfNodes, const bool direction, const int height);
	void treeToList_Colored_B(Node<T>*& head, int numberOfNodes, const bool direction);
public:
	Tree();
	~Tree();
	Node<T>*& getRoot() { return root; }
	int getSize() { return size; }
	void insert(Node<T>* z);
	Node<T>* search(T k);
	void qBalancePublic(const bool direction);			// creates complete tree
	void qBalanceAsync();								// 2-thread implementation
};

template<typename T>
Tree<T>::Tree()
{
	root = nullptr;
	size = 0;
}

template<typename T>
void Tree<T>::destroyTree(Node<T>* node)
{
	if (node != nullptr)
	{
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
	while (z != root && z->p->color == 0)
	{
		bool side(z->p->key >= z->p->p->key);
		Node<T>* y = z->p->p->child[!side];
		if (nullptr != y && y->color == 0)
		{
			z->p->color = 1;
			y->color = 1;
			z->p->p->color = 0;
			z = z->p->p;
			continue;
		}
		if (z == z->p->child[!side])
		{
			z = z->p;
			rotate(z, side);
		}
		z->p->color = 1;
		z->p->p->color = 0;
		rotate(z->p->p, !side);
	}
	root->color = 1;
}

template<typename T>
void Tree<T>::insert(Node<T>* z)
{
	insertBST(z);
	RBinsertFixup(z);
}

template<typename T>
Node<T>* Tree<T>::search(T k)
{
	Node<T>* x = root;
	while (nullptr != x && k != x->key)
		x = x->child[k >= x->key];
	return x;
}

//---------------------------------- qBalance -------------------------------------
template<typename T>
Node<T>* Tree<T>::qBalance(Node<T>*& node, int m, const bool direction)
{
	bool antiDirection = !direction;
	Node<T>* a, *b, *tmp;

	if (m == 3)
	{
		a = node;
		tmp = node->child[antiDirection];
		b = tmp->child[antiDirection];
		tmp->child[antiDirection] = a;
		tmp->child[direction] = b;
		node = b->child[antiDirection];
		a->p = b->p = tmp;
		a->child[!direction] = a->child[direction] = b->child[!direction] = b->child[direction] = nullptr;
		return tmp;
	}
	if (m == 2)
	{
		a = node;
		tmp = node->child[antiDirection];
		node = tmp->child[antiDirection];
		tmp->child[antiDirection] = a;
		a->p = tmp;
		a->child[!direction] = a->child[direction] = nullptr;
		tmp->child[direction] = nullptr;
		return tmp;
	}
	if (m == 1)
	{
		tmp = node;
		node = node->child[antiDirection];
		tmp->child[antiDirection] = tmp->child[direction] = nullptr;
		return tmp;
	}
	int q = m / 2;
	a = qBalance(node, q, direction);
	tmp = node;
	node = tmp->child[antiDirection];
	b = qBalance(node, m - q - 1, direction);
	if (nullptr != a)  a->p = tmp;
	if (nullptr != b)  b->p = tmp;
	tmp->child[direction] = b;
	tmp->child[antiDirection] = a;
	return tmp;
}

template<typename T>
Node<T>* Tree<T>::qBalanceComplete(Node<T>*& node, int m, const bool direction)
{
	bool antiDirection = !direction;
	Node<T>* a, *b, *tmp;

	if (m == 3)
	{
		a = node;
		tmp = node->child[antiDirection];
		b = tmp->child[antiDirection];
		tmp->child[antiDirection] = a;
		tmp->child[direction] = b;
		node = b->child[antiDirection];
		a->p = b->p = tmp;
		a->child[!direction] = a->child[direction] = b->child[!direction] = b->child[direction] = nullptr;
		return tmp;
	}
	if (m == 2)
	{
		a = node;
		tmp = node->child[antiDirection];
		node = tmp->child[antiDirection];
		tmp->child[antiDirection] = a;
		a->p = tmp;
		a->child[!direction] = a->child[direction] = nullptr;
		tmp->child[direction] = nullptr;
		return tmp;
	}
	if (m == 1)
	{
		tmp = node;
		node = node->child[antiDirection];
		tmp->child[antiDirection] = tmp->child[direction] = nullptr;
		return tmp;
	}
	int height = (int)log2(m);
	int q;

	if (m <= 3 * (int)pow(2, height - 1) - 1)
	{
		q = (int)pow(2, height - 1) - 1;
		a = qBalanceComplete(node, m - q - 1, direction);
		tmp = node;
		node = tmp->child[antiDirection];
		b = qBalance(node, q, direction);
		if (nullptr != a)  a->p = tmp;
		if (nullptr != b)  b->p = tmp;
		tmp->child[direction] = b;
		tmp->child[antiDirection] = a;
		return tmp;
	}
	q = (int)pow(2, height) - 1;
	a = qBalance(node, q, direction);
	tmp = node;
	node = tmp->child[antiDirection];
	b = qBalanceComplete(node, m - q - 1, direction);
	if (nullptr != a)  a->p = tmp;
	if (nullptr != b)  b->p = tmp;
	tmp->child[direction] = b;
	tmp->child[antiDirection] = a;
	return tmp;
}

//head - the head of the list, ->child[direction] is used to link the next node
template<typename T>
void Tree<T>::treeToList
(
	Node<T>* x,
	Node<T>*& head,
	int mateInt,
	int &mateCounter,
	const bool direction
)
{
	if (nullptr != x)
	{
		treeToList(x->child[direction], head, mateInt, mateCounter, direction);
		{
			x->child[direction] = head;
			head = x;

			if ((size - mateCounter) / 2 < (size - mateInt) && (size - mateCounter) % 2 == 1)
				head->color = 0;
			else
				head->color = 1;
			++mateCounter;
		}
		treeToList(x->child[!direction], head, mateInt, mateCounter, direction);
	}
}

template<typename T>
void Tree<T>::qBalancePublic(const bool direction)
{
	Node<T>* head = nullptr;
	int height = (int)log2(size);
	treeToList_Colored(head, size, direction, height);
	root = qBalanceComplete(head, size, direction);
}

template<typename T>
Node<T>* Tree<T>::extremum(Node<T>* x, bool direction)
{
	while (x->child[direction] != nullptr)
		x = x->child[direction];
	return x;
}

template<typename T>
Node<T>* Tree<T>::next(Node<T>* x, bool direction)
{
	if (x->child[direction] != nullptr)
		return extremum(x->child[direction], !direction);
	while (x->p != nullptr  &&  x == x->p->child[direction])
		x = x->p;
	return x->p;
}

template<typename T>
void Tree<T>::treeToList_Colored
(
	Node<T>*& head,
	int NUmberOfNodes,
	const bool direction,
	const int height
)
{
	//int height = (int)log2(NUmberOfNodes);
	int mateInt = pow(2, height) - 1;
	int mateCounter = 0;
	head = extremum(root, !direction);
	Node<T>* tail = head;
	for (int i = 0; i < NUmberOfNodes; ++i)
	{
		if (mateCounter / 2 < (NUmberOfNodes - mateInt) && mateCounter % 2 == 0)
			tail->color = 0;
		else
			tail->color = 1;
		++mateCounter;
		tail = tail->child[!direction] = next(tail, direction);
	}
}

template<typename T>
void Tree<T>::treeToList_Colored_B
(
	Node<T>*& head,
	int NUmberOfNodes,
	const bool direction
)
{
	head = extremum(root, !direction);
	Node<T>* tail = head;
	for (int i = 0; i < NUmberOfNodes; ++i)
	{
		tail->color = 1;
		tail = tail->child[!direction] = next(tail, direction);
	}
}

template<typename T>
void Tree<T>::qBalanceAsync()
{
	Node<T>* headL = nullptr;
	Node<T>* headR = nullptr;
	int sizeL, sizeR;

	if (size % 2 == 1)
	{
		sizeL = size / 2;
		sizeR = size / 2;
	}
	else
	{
		sizeL = size / 2;
		sizeR = size / 2 - 1;
	}
	int heightL = (int)log2(sizeL);
	int heightR = (int)log2(sizeR);

	if (heightL == heightR)
	{
		std::thread t(&Tree<T>::treeToList_Colored, this, ref(headL), sizeL, true, heightL);
		treeToList_Colored(headR, sizeR, false, heightR);
		t.join();
	}
	else
	{
		std::thread t(&Tree<T>::treeToList_Colored, this, ref(headL), sizeL, true, heightL);
		treeToList_Colored_B(headR, sizeR, false);
		t.join();
	}

	std::future<Node<T>*> fut = std::async(&Tree<T>::qBalanceComplete, this, ref(headL), sizeL, true);

	Node<T>* b = qBalanceComplete(headR, sizeR, false);
	Node<T>* a = fut.get();

	headL->child[0] = a;
	root = headL;
	root->child[1] = b;
	root->child[0]->p = root->child[1]->p = root;
	root->color = 1;
	if (nullptr != a)  a->p = root;
	if (nullptr != b)  b->p = root;
}