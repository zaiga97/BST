#include <iostream>
#include "bst.h"

int main(){

	std::pair<int, int> x(1,2);
	BST<int, int> t{};
	BST<int, int>::node* l;
	BST<int, int>::node a{x}, b{x, l};
	a = b;


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