#pragma once
template<typename T>
struct Node
{
	T key;
	Node* child[2];
	int bf; //the number of nodes in the subtree rooted at this node
	Node();
	Node(T);
};
template<typename T>
Node<T>::Node(T keyValue)
{
	key = keyValue;
	child[0] = nullptr;
	child[1] = nullptr;
	bf = 1;
}
template<typename T>
Node<T>::Node()
{
	child[0] = nullptr;
	child[1] = nullptr;
	bf = 1;
}

