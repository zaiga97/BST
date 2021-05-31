#include <iostream>
#include "../include/bst.h"

using namespace std;


int main(){

	// Default ctor
	BST<int, int> bst;
	cout << "bst:\n" << bst << endl;

	// Insert (l-value)
	std::pair<int, int> a{1, 1}, b{2, 2}, c{3, 3};
	bst.insert(a);
	bst.insert(b);
	bst.insert(c);
	cout << "Inserted 3 nodes:\n" << bst << endl;

	// Insert (r-value)
	bst.insert(std::pair<int, int> (4, 4));
	cout << "Inserted 1 node:\n" << bst << endl;

	// Emplace
	bst.emplace(0, 0);
	cout << "Emplaced 1 node:\n" << bst << endl;

	// Balance
	bst.balance();
	cout << "Balanced tree:\n" << bst << endl;

	// Subscripting (l-value)
	int i{1};
	cout << "Node with key = 1:\n" << bst[i] << endl;

	// Subscripting (r-value)
	bst[std::move(i)];
	cout << "Node with key = 1:\n" << bst[i] << endl;

	// Erase
	bst.erase(1);
	cout << "Erased node with key = 1:\n" << bst << endl;

	// Copy ctor
	BST<int, int> bst_copy{bst};
	cout << "bst_copy:\n" << bst_copy << endl;

	// Move ctor
	BST<int, int> bst_move{std::move(bst)};
	cout << "bst_move:\n" << bst_copy << endl;

	// Copy assignement
	BST<int, int> bst_copy_a{};
	bst_copy_a = bst_move;
	cout << "bst_copy_a:\n" << bst_copy_a << endl;

	// Move assignement
	BST<int, int> bst_move_a{};
	bst_move_a = std::move(bst_move);
	cout << "bst_move_a:\n" << bst_move_a << endl;

	return 0;
}