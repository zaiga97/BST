#include "bst.h"

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <math.h>
#include <stdexcept>

/** A fuction to search a key in the tree. Return an iterator to a node with the
	key == search_key found in the tree. If the key is not present then it returns an 
	iterator pointing to one past the last element, a.k.a end()
*/
template<typename Tkey, typename Tvalue, typename Tcompare>
BST<Tkey, Tvalue, Tcompare>::_iterator<std::pair<Tkey, Tvalue>> BST<Tkey, Tvalue, Tcompare>::find(const Tkey& search_key){
		// Base case for bst tree with no root
		if (! root) return iterator{};

		// Tmp node used during the search
		node* tmp_node {root.get()};
		Tkey tmp_key {tmp_node->pair_type.first};

		// Loop for descending the tree
		while (tmp_key != search_key){
	
			// Case search_key is higher than current key
			if (search_key > tmp_key){
				if (tmp_node->right_child){
					tmp_node = tmp_node->right_child.get();
					tmp_key = tmp_node->pair_type.first;
				}
				else return end(); // Node doesn't exist
			}

			// Case search_key is higher than current key
			else{
				if (tmp_node->left_child){
					tmp_node = tmp_node->left_child.get();
					tmp_key = tmp_node->pair_type.first;
				}
				else return end(); // Node doesn't exist
			}
		}
		// If the search is successful return the iterator to that position
		return iterator{tmp_node};
	}