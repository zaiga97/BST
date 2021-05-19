#include <iostream>
#include "bst.h"

int main(){

	std::pair<int, int> x(0,0), y(1,1), z(2,2), w(3,3), ww(4,4);
	BST<int, int> t{};
	t.insert(x);
	t.insert(y);
	t.insert(z);
	t.insert(w);
	t.insert(ww);

	std::cout << (t.root->pair_type.first) << std::endl;
	std::cout << (t.root->right_child.get()->pair_type.first) << std::endl;

	std::cout << t;

	t.balance();
	std::cout << std::endl;
	t.balance();
	std::cout << std::endl;


	std::cout << (t.root->pair_type.first) << std::endl;
	std::cout << (t.root->right_child.get()->pair_type.first) << std::endl;

	std::cout << t;

	auto abcd = t.find(22);
	std::cout << &abcd << std::endl;

	auto test = t[22];
	std::cout << test << std::endl;

	/*int al = 2;
	int* ptr = &al;
	std::cout << al << std::endl;
	al++;
	std::cout << ptr << " " << &al << " " << *ptr << std::endl;*/

	// al = value of al
	// &al = address of al
	// *ptr = value of al
	// ptr = address of al
	// &ptr = address of ptr


	return 0;

}