#include "bst.h"

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <math.h>
#include <stdexcept>

/** This pre-increment operator is the core of the _iterator class 
	and it's responsable for the traversing of the tree. All it does
	is changing the node the iterator is pointing to the one in 
	the tree with the next larger key. If such a node it's not present,
	(it's pointing to the node with biggest key in the tree), retuns
	an iterator pointing to a nullptr.
*/
template<typename Tkey, typename Tvalue, typename Tcompare>
template <typename Tpair_i>
BST<Tkey, Tvalue, Tcompare>::_iterator<Tpair_i>& BST<Tkey, Tvalue, Tcompare>::_iterator<Tpair_i> ::operator++(){
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
		// Get to the leftmost node on this new branc
		while (current->left_child){ current = current->left_child.get(); }
		return *this;
	}		
}

/** Post-increment operator. See pre-increment for more details*/
template<typename Tkey, typename Tvalue, typename Tcompare>
template <typename Tpair_i>
inline BST<Tkey, Tvalue, Tcompare>::_iterator<Tpair_i> BST<Tkey, Tvalue, Tcompare>::_iterator<Tpair_i> ::operator++(int){
	// copy in temp
	auto tmp{*this};
	// pre increment
	++(*this);
	// return temp
	return tmp;
}

/** Jump in the tree of n nodes following the key order*/
template<typename Tkey, typename Tvalue, typename Tcompare>
template <typename Tpair_i>
inline BST<Tkey, Tvalue, Tcompare>::_iterator<Tpair_i> BST<Tkey, Tvalue, Tcompare>::_iterator<Tpair_i> ::operator+(int n){
	for (unsigned int i = 0; i < n; i++){
		*this++;
	}
	return *this;
}