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
	//using const_iterator = iterator<const Tkey, const Tvalue>;

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
	const iterator begin() const {return begin();}
	const iterator cbegin() const {return begin();}

	// return an iterator pointing to the last+1 node

	iterator end(){return iterator{nullptr};}
	const iterator end() const {return iterator{nullptr};}
	const iterator cend() const {return iterator{nullptr};};

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
		// handle error where nothing is found
		// If the search is successful return the iterator to that position
		iterator iter{tmp_node};
		return iter;

	}

	
	//const_iterator find(const Tkey& x) const;

//					###balance();###
	void build_balace_queue(int i, int f, std::queue<Tkey>* q){
		// warning, static may stay the same for different trees!
		//std::queue<int> q{};
		if (f == i){
			q->push(i);
		}
		else if (f-i+1 == 2){
			q->push(i);
			q->push(f);
		}
		else{
			int m = i + ceil((float)(f-i)/2);
			q->push(m);

			build_balace_queue(i, m-1, q);
			build_balace_queue(m+1, f, q);
		}
		return;
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
		std::queue<Tkey> queue;
		build_balace_queue(0, t_content.size()-1, &queue);

		// build a balaced tree
		BST<Tkey, Tvalue> tmp_t{};

		while (!queue.empty())
		{
			// queue.front() is the key of the last element in the stack
			// t_content[queue.front()] is the pair <key, value>
		   	tmp_t.insert(t_content[queue.front()]);
		   	std::cout << queue.front() << " " ;
		   	queue.pop();
		}
		std::cout << std::endl;

		// swap balanced tree and this

		*this = std::move(tmp_t);
	}
	//					###subscripting###
	/*
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
	*/

	// check questions
	Tvalue& operator[](Tkey&& x){
		iterator iter{};
		iter = find(std::forward<Tkey>(x)); // Need a find(&&) function ??
		// If a node with that key exists
		if (iter.current != nullptr){
			return iter.current->pair_type.second;
		}
		else{
			// Create new node with random value
			std::pair<Tkey, Tvalue> new_node(std::forward<Tkey>(x), Tvalue{});
			insert(new_node);
			return operator[](x);
		}
	}
	

	//					###put_to<<###
	// check why const gives seg fault
	// has to do with cbegin and cend
	friend std::ostream& operator<<(std::ostream& os, BST& t){
		for (const auto& x: t){
			os << x.first << " ";
		}
		os << std::endl;
		
		return os;
	}

	//					###copy and move###
	BST(BST&& t) = default;
	BST& operator=(BST&& t) = default;


	//					###erase###

	void erase(const Tkey& x){

		iterator iter {find(x)};
		node* current{iter.current};

		/*if (!iter.current){
			tree is empty, handle error
		}*/

		/*if (iter == nullptr){
			node doesn't exist, handle error
		}*/

		/*  3 possible cases:
		*	- x is a leaf: just delete it
		*	- x has one child: put the child in its place
		*	- x has two children: put the inorder successor in its place
		*	note: the inorder successor is the leftmost node in the subtree to the right of x
		*/

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
		// check correctness
		pair_type = std::move(x.pair_type);
		left_child = std::move(x.left_child);
		right_child = std::move(x.right_child);
		parent = std::move(x.parent);
		// is this second part necessary?
		x.pair_type.first = 0;
		x.pair_type.second = 0;
		x.left_child.reset(nullptr);
		x.right_child.reset(nullptr);
		x.parent = nullptr;

		return *this;
	}

	// 					###move_erase###

	void move_erase(int first_lr, int second_lr){

		switch(first_lr){
			case 0:
			this->parent->left_child.release();
			switch(second_lr){
				case 0:
				this->parent->left_child.reset(this->left_child.get());
				this->left_child->parent = this->parent;
				break;
				case 1:
				this->parent->left_child.reset(this->right_child.get());
				this->right_child->parent = this->parent;
				break;
			}
			break;
			case 1:
			this->parent->right_child.release();
			switch(second_lr){
				case 0:
				this->parent->right_child.reset(this->left_child.get());
				this->left_child->parent = this->parent;
				break;
				case 1:
				this->parent->right_child.reset(this->right_child.get());
				this->right_child->parent = this->parent;
				break;
			}

		}

		this->erase_node();

	}


	//					###erase node###
	void erase_node(){
		left_child.release();
		right_child.release();
		parent = nullptr;
		delete this;
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