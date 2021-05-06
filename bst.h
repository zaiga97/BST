#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <math.h>

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
		Tkey search_key{x.first};	
		// Base case for bst tree with no root
		if (!root){
			root.reset(new node{x});
			return (std::pair<iterator, bool> (iterator{root.get()}, true));
		}

		// Tmp node used during the search
		node* tmp_node {root.get()};
		Tkey tmp_key {tmp_node->pair_type.first};
		// Loop for descending the tree
		while (tmp_key != search_key){
			// Case search_key is higher than current key
			if (search_key > tmp_key){ // Tcompare ??
				if (tmp_node->right_child){
					tmp_node = tmp_node->right_child.get();
					tmp_key = tmp_node->pair_type.first;
				}
				else { // node doesn't exist
					tmp_node->right_child.reset(new node{x});
					tmp_node->right_child->parent = tmp_node;
					return (std::pair<iterator, bool> (iterator{root.get()}, true));
				}
			}

			// Case search_key is higher than current key
			else{ // Tcompare ??
				if (tmp_node->left_child){
					tmp_node = tmp_node->left_child.get();
					tmp_key = tmp_node->pair_type.first;
				}
				else { // node doesn't exist
					tmp_node->left_child.reset(new node{x});
					tmp_node->left_child->parent = tmp_node;
					return (std::pair<iterator, bool> (iterator{root.get()}, true));
				}
			}
		}
		//size ++;

		// The key is already in the tree
		return (std::pair<iterator, bool> (iterator{}, false));
	}

	//					###emplace();###

	template<class... Types >
	std::pair<iterator,bool> emplace(Types&&... args){
		// std::move unconditionally transforms the passed argument in an rvalue
		// std::forward forwards r(l)values if r(l)values are passed
		return insert(std::pair<Tkey, Tvalue> (std::forward<Types>(args)...));
	}

	//					###clear();###

	void clear(){
		root.reset();
	}

	//					###iterator###

	// return an iterator pointing to the leftmost node
	iterator begin(){

		if (!root){
			return iterator{};
		}

		node* tmp_node = root.get();
		while (tmp_node->left_child){
			tmp_node = tmp_node->left_child.get();
		}

		return iterator{tmp_node};
	}
	//const_iterator begin() const;
	//const_iterator cbegin() const;

	// return an iterator pointing to the last+1 node

	iterator end(){
		return iterator{nullptr};
	}
	//const_iterator end() const;
	//const_iterator cend() const;

	//					###find();###

	iterator find(const Tkey& search_key){
		// Base case for bst tree with no root
		if (! root){
			iterator iter {};
			return iter;
		}

		// Tmp node used during the search
		node* tmp_node {root.get()};
		Tkey tmp_key {tmp_node->pair_type.first};

		// Loop for descending the tree
		while (tmp_key != search_key){
	
			// Case search_key is higher than current key
			if (search_key > tmp_key){ // Tcompare ??
				if (tmp_node->right_child){
					tmp_node = tmp_node->right_child.get();
					tmp_key = tmp_node->pair_type.first;
				}
				else { // node doesn't exist
					return end();
				}
			}

			// Case search_key is higher than current key
			else{ // Tcompare ??
				if (tmp_node->left_child){
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
	std::queue<int> build_balace_queue(int i, int f){
		static std::queue<int> q{};
		if (f == i){
			q.push(i);
		}
		else if (f-i+1 <= 2){
			q.push(i);
			q.push(f);
		}
		else{
			int m = i + ceil((float)(f-i)/2);
			q.push(m);

			build_balace_queue(i, m-1);
			build_balace_queue(m+1, f);
		}
		return q;
	}

	void balance(){
		// save all tree content in an array
		std::vector<std::pair<Tkey, Tvalue>> t_content{};
		for (auto x:*this){
			t_content.push_back(x);
		}
		//std::cout << n << std::endl;
		//std::cout << t_content[1].second << std::endl;

		// build a queque for insertion
		auto queue = build_balace_queue(0, t_content.size()-1);

		// build a balaced tree
		BST<Tkey, Tvalue> tmp_t{};

		while (!queue.empty())
		{
		   	tmp_t.insert(t_content[queue.front()]);
		   	queue.pop();
		}

		// swap balanced tree and this

		*this = std::move(tmp_t);
	}

	//					###subscripting###

	Tvalue& operator[](const Tkey& x){
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
	//Tvalue& operator[](Tkey&& x);

	//					###put_to<<###

	friend std::ostream& operator<<(std::ostream& os, BST& t){
		/*for (const iterator& x: t){
			os << x.current->pair_type.second << " ";
		}
		os << std::endl;
		*/
		iterator x{t.begin()};
		os << "(" << x.current->pair_type.first << ", " << x.current->pair_type.second << ") ";
		while ((++x) != t.end()){
			os << "(" << x.current->pair_type.first << ", " << x.current->pair_type.second << ") ";
		}
		os << std::endl;
		
		return os;
	}

	//					###copy and move###
	BST(BST&& t) = default;
	BST& operator=(BST&& t) = default;
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

template<typename Tkey, typename Tvalue, typename Tcompare>
class BST<Tkey, Tvalue, Tcompare>::iterator{
public:
	friend class BST;

	using node = BST<Tkey, Tvalue, Tcompare>::node;

	node* current{nullptr};

	// default ctor
	iterator() = default;

	// iterator ctor from a node
	iterator(node* Pcurrent) : current{Pcurrent}{};

	// pre-increment, fix search
	iterator& operator++()
	{
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

	// post-increment
	
	iterator operator++(int)
	{
		// copy in temp
		auto tmp{*this};
		// pre increment
		++(*this);
		// return temp
		return tmp;
	}
	

	// dereference operator
	std::pair<Tkey, Tvalue>& operator*()
	{
		return current -> pair_type;
	}

	// arrow operator, returns address to value
	Tvalue* operator->()
	{
		/*  meaning:
			*this = myself
			*(*this) = calls dereference function
			&(*(*this)) = address of returned object and return a pointer
		*/
		return &(*(*this));
	}


	// sum operator, useless
	iterator operator+(int n)
	{
		for (unsigned int i = 0; i < n; i++){
			*this++;
		}
		return *this;
	}

	// == operator, should have 2 args?
	bool operator==(const iterator& tmp) const 
	{
		// return true if the two current nodes have same key and value
		return (current == tmp.current);
	}

	// != operator
	bool operator!=(const iterator& tmp) const 
	{
		// uses == definition
		return !(*this == tmp);
	}

	/*

	iterator  operator++(int)          				{ return pos_++; }
    iterator& operator++()    			            { ++pos_; return *this; }
    reference operator* () const                    { return *pos_; }
    pointer   operator->() const                    { return pos_; }
    iterator  operator+ (difference_type v)   const { return pos_ + v; }
    bool      operator==(const iterator& rhs) const { return pos_ == rhs.pos_; }
    bool      operator!=(const iterator& rhs) const { return pos_ != rhs.pos_; }

    */


};