#include "bst.h"

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <math.h>
#include <stdexcept>

/* 	A recursive function that build a queue of integer positions in such a way that if we insert a sorted
	array in this order into an empty tree we end up with a balaced tree.
*/
template<typename Tkey, typename Tvalue, typename Tcompare>
void BST<Tkey, Tvalue, Tcompare>::build_balace_queue(int i, int f, std::queue<int>& q){
	// Base cases:
	if (f == i){
		q.push(i);
	}
	else if (f-i+1 == 2){
		q.push(i);
		q.push(f);
	}

	// Recursive step
	else{
		int m = i + ceil((f-i)/2.);
		q.push(m);

		build_balace_queue(i, m-1, q);
		build_balace_queue(m+1, f, q);
	}
	return;
}

/** This function can be used for balancing the tree. It copies the content
	of the tree in a std::vector using an iterator. A new tree is generated
	and the content of the vector is inserted in such a way to keep the 
	new tree as balanced as possible. In the end a move between the new 
	tree and the old one is performed.
*/
template<typename Tkey, typename Tvalue, typename Tcompare>
void BST<Tkey, Tvalue, Tcompare>::balance(){
	// Save all tree content in an array
	// Note that it's sorted because of how we traverse the tree
	std::vector<std::pair<Tkey, Tvalue>> t_content{};
	for (auto x:*this){
		t_content.push_back(x);
	}

	// Build an queque of integer positions for later insertion
	std::queue<int> balance_queue{};
	build_balace_queue(0, t_content.size()-1, balance_queue);

	// Build an empty tree
	BST<Tkey, Tvalue> tmp_t{};

	while (!balance_queue.empty())
	{
		// queue.front() is the integer position of the next pair to be inserted
		// t_content[queue.front()] is the pair <key, value> to be inserted 
	   	tmp_t.insert(t_content[balance_queue.front()]);
	   	balance_queue.pop();
	}

	// Swap balanced tree and this
	*this = std::move(tmp_t);
}
