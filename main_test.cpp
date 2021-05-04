#include <iostream>
#include "bst.h"

int main(){

	std::pair<int, int> x(1,2), y(0,1), z(2,4);
	BST<int, int> t{};
	t.insert(x);
	t.insert(y);
	t.insert(z);

	//std::cout << (t.root->left_child->left_child) << std::endl;
	
	BST<int,int>::iterator it = t.begin();
	std::cout << it.current->pair_type.first << std::endl;
	it++;
	std::cout << it.current->pair_type.first << std::endl;
	it++;
	std::cout << it.current->pair_type.first << std::endl;
	it++;
	std::cout << it.current  << std::endl;

	std::cout << t;

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