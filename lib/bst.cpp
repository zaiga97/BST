#include "../include/bst.h"

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <math.h>
#include <stdexcept>


//					### build balance queue ###
template<typename Tkey, typename Tvalue, typename Tcompare>
void BST<Tkey, Tvalue, Tcompare>::build_balance_queue(int i, int f, std::queue<int>& q){
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

		build_balance_queue(i, m-1, q);
		build_balance_queue(m+1, f, q);
	}
	return;
}

//					### balance ###
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
	build_balance_queue(0, t_content.size()-1, balance_queue);

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


//					### _insert ###
template<typename Tkey, typename Tvalue, typename Tcompare>
template <typename O>
std::pair<_iterator<std::pair<Tkey, Tvalue>>, bool> BST<Tkey, Tvalue, Tcompare>::_insert(O&& x){
	Tkey search_key{std::forward<O>(x).first};	
	// Base case for bst tree with no root
	if (!root){
		root.reset(new node{std::forward<O>(x)});
		return (std::pair<iterator, bool> (iterator{root.get()}, true));
	}

	// Tmp node used during the search
	node* tmp_node {root.get()};
	Tkey tmp_key {tmp_node->pair_type.first};
	// Loop for descending the tree
	while (tmp_key != search_key){
		// Case search_key is higher than current key
		if (!lt(search_key, tmp_key)){ 
			if (tmp_node->right_child){
				tmp_node = tmp_node->right_child.get();
				tmp_key = tmp_node->pair_type.first;
			}
			else { // node doesn't exist
				tmp_node->right_child.reset(new node{std::forward<O>(x)});
				tmp_node->right_child->parent = tmp_node;
				return (std::pair<iterator, bool> (iterator{root.get()}, true));
			}
		}

		// Case search_key is higher than current key
		else{ 
			if (tmp_node->left_child){
				tmp_node = tmp_node->left_child.get();
				tmp_key = tmp_node->pair_type.first;
			}
			else { // node doesn't exist
				tmp_node->left_child.reset(new node{std::forward<O>(x)});
				tmp_node->left_child->parent = tmp_node;
				return (std::pair<iterator, bool> (iterator{root.get()}, true));
			}
		}
	}

	// The key is already in the tree
	return (std::pair<iterator, bool> (iterator{nullptr}, false));
}

//					### _begin ###
template<typename Tkey, typename Tvalue, typename Tcompare>
typename BST<Tkey, Tvalue, Tcompare>::node* BST<Tkey, Tvalue, Tcompare>::_begin() const noexcept {
	if (!root){return nullptr;}
	node* tmp_node = root.get();
	while (tmp_node->left_child){
		tmp_node = tmp_node->left_child.get();
	}
	return tmp_node;
}

//					### _find ###
template<typename Tkey, typename Tvalue, typename Tcompare>
typename BST<Tkey, Tvalue, Tcompare>::node* BST<Tkey, Tvalue, Tcompare>::_find(const Tkey& search_key){
	// Base case for bst tree with no root
	if (! root) return nullptr;

	// Tmp node used during the search
	node* tmp_node {root.get()};
	Tkey tmp_key {tmp_node->pair_type.first};

	// Loop for descending the tree
	while (tmp_key != search_key){
	
		// Case search_key is higher than current key
		if (!lt(search_key, tmp_key)){
			if (tmp_node->right_child){
				tmp_node = tmp_node->right_child.get();
				tmp_key = tmp_node->pair_type.first;
			}
			else return nullptr; // Node doesn't exist
		}

		// Case search_key is higher than current key
		else{
			if (tmp_node->left_child){
				tmp_node = tmp_node->left_child.get();
				tmp_key = tmp_node->pair_type.first;
			}
			else return nullptr; // Node doesn't exist
		}
	}
	// If the search is successful return the iterator to that position
	return tmp_node;
}

//					### find ###
template<typename Tkey, typename Tvalue, typename Tcompare>
BST<Tkey, Tvalue, Tcompare>::_iterator<std::pair<Tkey, Tvalue>> BST<Tkey, Tvalue, Tcompare>::find(const Tkey& search_key){
		return iterator{_find(search_key)};
}

//					### find ###
template<typename Tkey, typename Tvalue, typename Tcompare>
BST<Tkey, Tvalue, Tcompare>::_iterator<const std::pair<Tkey, Tvalue>> BST<Tkey, Tvalue, Tcompare>::find(const Tkey& search_key) const{
		return const_iterator{_find(search_key)};
}

//					### subscripting operator ###
template<typename Tkey, typename Tvalue, typename Tcompare>
Tvalue& BST<Tkey, Tvalue, Tcompare>::operator[](const Tkey& x) noexcept{
	iterator iter{};
	iter = find(x);
	// If a node with that key exists
	if (iter.current != nullptr){
		return iter.current->pair_type.second;
	}
	else{
		// Create new node with random value
		std::pair<Tkey, Tvalue> new_node(x, Tvalue{});
		insert(new_node);
		return operator[](x);
	}	
}

//					### subscripting operator ###
template<typename Tkey, typename Tvalue, typename Tcompare>
Tvalue& BST<Tkey, Tvalue, Tcompare>::operator[](Tkey&& x) noexcept{
	iterator iter{};
	iter = find(x);
	// If a node with that key exists
	if (iter.current != nullptr){
		return iter.current->pair_type.second;
	}
	else{
		// Create new node with random value
		std::pair<Tkey, Tvalue> new_node(std::move(x), Tvalue{});
		insert(new_node);
		return operator[](x);
	}
}



//					### erase ###
template<typename Tkey, typename Tvalue, typename Tcompare>
void erase(const Tkey& x){

	BST<Tkey, Tvalue, Tcompare>::_iterator<std::pair<Tkey, Tvalue>> iter {find(x)};
	BST<Tkey, Tvalue, Tcompare>::node* current{iter.current};

	/*if (!iter.current){
		tree is empty, handle return
	}*/

	/*if (iter == nullptr){
		node doesn't exist, handle return
	}*/

	// First case: current is a leaf

	if (!current->right_child.get() && !current->left_child.get()){
		// add root case
		// current is a left child
		if (current->parent->left_child.get() == current){
			current->parent->left_child.reset(nullptr);
			return;
		}
		// current is a right child
		else {
			current->parent->right_child.reset(nullptr);
			return;
		}
	}
	// Second case: current has just one child
	
	// current has a left child
	if (!current->right_child.get() && current->left_child.get()){
		if (current->parent == nullptr){
			root.release();
			root.reset(current->left_child.get());
			current->left_child->parent = nullptr;
			current->erase_node();
			return;
		}
		// current is a left child
		else if (current->parent->left_child.get() == current){
			/* move_erase takes two args that translate to:
			* - first arg: 0 if current is a left child, 1 if current is a right child
			* - second arg: 0 if current has a left child, 1 if current has a right child
			*/
			current->move_erase(0, 0);
			return;
		}
		// current is a right child
		else if (current->parent->right_child.get() == current){
			current->move_erase(1, 0);
			return;
		}
	}
		// current has a right child
	if (current->right_child.get() && !current->left_child.get()){
		// current is root
		if (current->parent == nullptr){
			root.release();
			root.reset(current->right_child.get());
			current->right_child->parent = nullptr;
			current->erase_node();
			return;
		}

		// current is a left child
		else if (current->parent->left_child.get() == current){
			current->move_erase(0, 1);
			return;
		}
		// current is a right child
		else if (current->parent->right_child.get() == current){
			current->move_erase(1, 1);
			return;
			}
		}

		
	// Third case: current has two children
	if (current->right_child.get() && current->left_child.get()){
		// swap with the leftmost node in the right subtree marked by current
		bool found {false};
		node* child {current->right_child.get()};
		while (!found){
			if (child->left_child){
				child = child->left_child.get();
			}
			else { // left doesn't exist
				found = true;
			}
		}
		// if child is the right child of current, then its right child will point to null
		if (current->right_child.get() == child){
			child->right_child.release();
			child->right_child.reset(nullptr);
			}
		else{
			child->right_child.release();
			child->right_child.reset(current->right_child.get());
			child->right_child->parent = child;
			// setting the old child's parent to point to nullptr
			child->parent->left_child.release();
			child->parent->left_child.reset(nullptr);
		}
		child->parent = current->parent;
		child->left_child.release();
		child->left_child.reset(current->left_child.get());
		// current is root
		if (current->parent == nullptr){
			root.release();
			root.reset(child);
			current->left_child->parent = child;
			current->erase_node();
			return;
		}
		// current is a left node
		else if (current->parent->left_child.get() == current){
			current->parent->left_child.release();
			current->parent->left_child.reset(child);
			current->left_child->parent = child;
			current->erase_node();
			return;
		}
		// current is a right node
		else if(current->parent->right_child.get() == current){
			current->parent->right_child.release();
			current->parent->right_child.reset(child);
			current->left_child->parent = child;
			current->erase_node();
			return;
		}
	}
}