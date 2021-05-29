#include <iostream>
#include "../include/bst.h"


int main(){

	std::pair<int, int> x(0,0), y(1,1), z(2,2), w(3,3), ww(4,4);
	BST<int, int> t{}, u{};
	t.insert(x);
	t.insert(y);
	t.insert(z);
	t.insert(w);
	t.insert(ww);

	u = t;

	std::cout << u << std::endl;

	std::cout << (t.root->pair_type.first) << std::endl;
	std::cout << (t.root->right_child.get()->pair_type.first) << std::endl;

	std::cout << t;

	t.balance();
	std::cout << std::endl;
	t.balance();
	std::cout << std::endl;
	t.erase(2397);


	std::cout << (t.root->pair_type.first) << std::endl;
	std::cout << (t.root->right_child.get()->pair_type.first) << std::endl;

	std::cout << t;

	auto abcd = t.find(22);
	std::cout << &abcd << std::endl;

	auto test = t[22];
	std::cout << test << std::endl;

	auto t2 = t;
	std::cout << t2 << std::endl;



	return 0;

}