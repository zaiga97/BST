#include <iostream>
#include "../include/bst.h"


int main(){

	std::pair<int, int> x(0,0), y(1,1), z(2,2), w(3,3), ww(4,4);
	BST<int, int> t{};
	//t.emplace(std::pair<int, int>(1,1));
	t.insert(y);
	t.insert(std::pair<int,int> (0,0));

	std::cout << t[1] << std::endl;
	std::cout << t[0] << std::endl;

	//auto abcd = t.find(0);
	//std::cout << &abcd << std::endl;
	return 0;

}