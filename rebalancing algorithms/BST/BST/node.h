#pragma once

template<typename T>
struct Node
{
	T key;
	Node* child[2];
	Node();
	Node(T keyValue);
};

template<typename T>
Node<T>::Node(T keyValue)
{
	key = keyValue;
	child[0] = nullptr;
	child[1] = nullptr;
}

template<typename T>
Node<T>::Node()
{
	child[0] = nullptr;
	child[1] = nullptr;
}
