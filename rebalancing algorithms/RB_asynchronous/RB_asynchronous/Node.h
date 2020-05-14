#pragma once
template<typename T>
struct Node
{
	T key;
	Node* p;
	Node* child[2];
	char color;
	Node();
	Node(T keyValue);
};
template<typename T>
Node<T>::Node(T keyValue)
{
	key = keyValue;
	p = nullptr;
	child[0] = nullptr;
	child[1] = nullptr;
	color = 0;
}
template<typename T>
Node<T>::Node()
{
	p = nullptr;
	child[0] = nullptr;
	child[1] = nullptr;
	color = 0;
}
