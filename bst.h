#pragma once
#include <iostream>
#include <vector>
#include <memory>


template<typename Tkey, typename Tvalue, typename Tcompare = std::less<Tkey>>
class BST{
public:

	struct node; 

	// pointer to the root node
	std::unique_ptr<node> root;

	// default ctor
	BST() = default;

	// default dtor
	~BST() = default;

	class iterator;

	//					###insert();###

	//std::pair<iterator, bool> insert(std::pair<Tkey, Tvalue>&& x);
	std::pair<iterator, bool> insert(const std::pair<Tkey, Tvalue>& x){
		// Base case for bst tree with no root
		node new_node {x};
		if (! *this.root){
			*this.root = &new_node;			
			return (std::make_pair<iterator, bool> (iterator{*this.root}, true));
		}

		// Tmp node used during the search
		node* tmp_node {*this.root.get()};
		Tkey tmp_key {tmp_node->pair_type.first};
		// Loop for descending the tree
		while (tmp_key != search_key){
			// Case search_key is higher than current key
			if (search_key > tmp_key){ // Tcompare ??
				if (tmp_node.right_child){
					tmp_node = tmp_node->right_child.get();
					tmp_key = tmp_node->pair_type.first;
				}
				else { // node doesn't exist
					tmp_node.right_child = &new_node;
					return (std::make_pair<iterator, bool> (iterator{&new_node}, true));
				}
			}

			// Case search_key is higher than current key
			else{ // Tcompare ??
				if (tmp_node.left_child){
					tmp_node = tmp_node->left_child.get();
					tmp_key = tmp_node->pair_type.first;
				}
				else { // node doesn't exist
					tmp_node.right_child = &new_node;
					return (std::make_pair<iterator, bool> (iterator{&new_node}, true));
				}
			}
		}

		// The key is already in the tree
		return (std::make_pair<iterator, bool> (iterator{}, false));
	}

	//					###emplace();###

	//template< class... Types >
	//std::pair<iterator,bool> emplace(Types&&... args);

	//					###clear();###

	//void clear();

	//					###iterator###

	// return an iterator pointing to the leftmost node
	iterator begin(){

		if (! *this.root){
			iterator iter {};
			return iter;
		}

		node* tmp_node = *this.root.get();
		while (tmp_node->left_child){
			tmp_node == tmp_node->left_child.get();
		}

		iterator iter{tmp_node};
		return iter;
	}
	//const_iterator begin() const;
	//const_iterator cbegin() const;

	// return an iterator pointing to the last+1 node

	iterator end(){

		if (! *this.root){
			iterator iter {};
			return iter;
		}

		node* tmp_node = *this.root.get();
		while (tmp_node->right_child){
			tmp_node == tmp_node->right_child.get();
		}

		iterator iter{tmp_node};
		return iter;
	}
	//const_iterator end() const;
	//const_iterator cend() const;

	//					###find();###

	iterator find(const Tkey& search_key){
		// Base case for bst tree with no root
		if (! *this.root){
			iterator iter {};
			return iter;
		}

		// Tmp node used during the search
		node* tmp_node {*this.root.get()};
		Tkey tmp_key {tmp_node->pair_type.first};

		// Loop for descending the tree
		while (tmp_key != search_key){
	
			// Case search_key is higher than current key
			if (search_key > tmp_key){ // Tcompare ??
				if (tmp_node.right_child){
					tmp_node = tmp_node->right_child.get();
					tmp_key = tmp_node->pair_type.first;
				}
				else { // node doesn't exist
					return end();
				}
			}

			// Case search_key is higher than current key
			else{ // Tcompare ??
				if (tmp_node.left_child){
					tmp_node = tmp_node->left_child.get();
					tmp_key = tmp_node->pair_type.first;
				}
				else { // node doesn't exist
					return end();
				}
			}
		}

		// If the search is succesfull return the iterator to that position
		iterator iter{tmp_node};
		return iter;

	}
	//const_iterator find(const Tkey& x) const;

	//					###balance();###

	//void balance();

	//					###subscripting###

	//value_type& operator[](const Tkey& x);
	//value_type& operator[](Tkey&& x);

	//					###put_to<<###

	friend std::ostream& operator<<(std::ostream& os, const BST& t){
		for(auto x: t){
			os << x.current->pair_type.second << " ";
		}
		os << std::endl;
		return os;
	}

	//					###copy and move###

	//					###erase###

	//void erase(const Tkey& x);
};


template<typename Tkey, typename Tvalue, typename Tcompare>
struct BST<Tkey, Tvalue, Tcompare>::node{

	std::pair<Tkey, Tvalue> pair_type;
	std::unique_ptr<node> left_child;
	std::unique_ptr<node> right_child;
	node* parent;

	// default ctor
	node() = default;

	// custom ctor: passing std::pair
	node(const std::pair<Tkey, Tvalue>& Npair): 
	pair_type{Npair}, left_child{nullptr}, right_child{nullptr}, parent{nullptr} {}

	// custom ctor: passing std::pair and parent
	node(const std::pair<Tkey, Tvalue>& Npair, node* Nparent): 
	pair_type{Npair}, left_child{nullptr}, right_child{nullptr}, parent{Nparent -> parent} {}

	// default dtor
	~node() = default;

	node& operator=(node& x){
		pair_type = std::move(x.pair_type);
		left_child = std::move(x.left_child);
		right_child = std::move(x.right_child);
		parent = std::move(x.parent);

		x.pair_type.first = 0;
		x.pair_type.second = 0;
		x.left_child.reset(nullptr);
		x.right_child.reset(nullptr);
		x.parent = nullptr;

		return *this;
	}
};

