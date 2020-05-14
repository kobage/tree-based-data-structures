#include<iostream>
#include"Node.h"
#include"tree.h"
#include<vector>
#include<random>
#include"chrono"

using namespace std;

int calculateHeight(Node<int>* node)
{
	if (node == nullptr) return -1;
	int a = calculateHeight(node->child[0]);
	int b = calculateHeight(node->child[1]);
	return (1 + ((a < b) ? b : a));
}

void balancing(int tree_size, int method)
{
	switch (method)
	{
	case 1: cout << "Test started: balanceMod..." << endl << endl; break;
	case 2: cout << "Test started: qBalance..." << endl << endl; break;
	default: cout << "Enter correct choice. "; return;
	}
	//შემთხვევითი სიდიდეების და რიცხვების შექმნა, დროსთან დაკავშირებული ცვლადები
	//Creating random values and numbers, variables related to time
	auto st = chrono::high_resolution_clock::now();
	auto diff = chrono::high_resolution_clock::now() - st;
	auto time = chrono::duration_cast<chrono::milliseconds>(diff);

	default_random_engine dre;
	uniform_int_distribution<int> di;

	//ხის აგება და გადაბალანსება
	//Building and balancing the tree
	vector<_int64> times;
	vector<_int64> BalancingTimes;
	const int REPETITIONS = 2;
	int height;

	for (int k = 0; k < REPETITIONS; ++k)
	{
		Tree<int> tree;
		st = chrono::high_resolution_clock::now();
		for (int i = 0; i < tree_size; i++)
		{
			Node<int>* node = new Node<int>(di(dre));		  //Random data
			//	Node<int>* node = new Node<int>(i + 1);			//Ordered data
			tree.insert(node);
		}

		diff = chrono::high_resolution_clock::now() - st;
		time = chrono::duration_cast<chrono::milliseconds>(diff);
		times.push_back(time.count());

		st = chrono::high_resolution_clock::now();
		switch (method)
		{
		case 1:  tree.qBalancePublic(true); break;
		case 2:  tree.qBalanceAsync(); break;

		}
		diff = chrono::high_resolution_clock::now() - st;
		time = chrono::duration_cast<chrono::milliseconds>(diff);
		BalancingTimes.push_back(time.count());
		if (k == (REPETITIONS - 1))
			height = calculateHeight(tree.getRoot());
	}

	cout << endl << "Tree has been built in : " << times[REPETITIONS / 2] << " milliseconds. " << endl;
	cout << "Tree has been balanced in : " << BalancingTimes[REPETITIONS / 2] << " milliseconds." << endl;
	cout << "tree size: " << tree_size;
	cout << "; tree height: " << height << endl << endl;
	cout << "Test completed successfully!" << endl << endl;
}

int main()
{
	int n;
	cout << "Enter size of tree" << endl;
	cin >> n;
	cout << "To balance with modified Sedgewick enter 1; to balance with qBalance enter 2" << endl;
	int choice;
	cin >> choice;
	balancing(n, choice);
}