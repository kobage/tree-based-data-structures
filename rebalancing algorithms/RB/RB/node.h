#pragma once
template<typename T>
struct Node
{
	T key;
	Node* p;
	Node* child[2];
	int bf; //balance factor   
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
	bf = 0;    //red 
}
template<typename T>
Node<T>::Node()
{
	p = nullptr;
	child[0] = nullptr;
	child[1] = nullptr;
	bf = 0;    //red 
}