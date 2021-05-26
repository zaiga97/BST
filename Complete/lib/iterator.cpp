#include "../include/bst.h"

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <math.h>
#include <stdexcept>



//					### pre-increment operator ###
template <typename Tkey_i, typename Tvalue_i>
_iterator<Tkey_i, Tvalue_i>& _iterator<Tkey_i, Tvalue_i>::operator++(){
	// If current has no right children then i have to search a new branch to move down
	if (!current->right_child){
		// Find the first node for who current is a left descendant
		// Also need to check if I reached the root and in case return nullptr
		while (current->parent && current->parent->right_child.get() == current){ current = current->parent; }
		current = current->parent;
		return *this;
	}
	// Now if my node has a right child then the next node wiil be the leftmost on the right sub-tree:
	else{
		// Move on the right branch
		current = current->right_child.get();
		// Get to the leftmost node on this new branch
		while (current->left_child){ current = current->left_child.get(); }
		return *this;
	}		
}

//					### post-increment operator ###
template <typename Tkey_i, typename Tvalue_i>
_iterator<Tkey_i, Tvalue_i> _iterator<Tkey_i, Tvalue_i>::operator++(int){
	// copy in temp
	auto tmp{*this};
	// pre increment
	++(*this);
	// return temp
	return tmp;
}

//					### n-increment operator ###
template <typename Tkey_i, typename Tvalue_i>
_iterator<Tkey_i, Tvalue_i> _iterator<Tkey_i, Tvalue_i>::operator+(int n){
	for (unsigned int i = 0; i < n; i++){
		*this++;
	}
	return *this;
}