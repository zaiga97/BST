#ifndef __bst
#define __bst

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <math.h>
#include <stdexcept>

// use tcompare
template<typename Tkey, typename Tvalue, typename Tcompare = std::less<Tkey>>
class BST{

	void build_balace_queue(int i, int f, std::queue<int>& q);

public:

	struct node; 

	// pointer to the root node
	std::unique_ptr<node> root;

	// default ctor
	BST() = default;

	// default dtor
	~BST() noexcept = default;

	template <typename Tpair_i>
	class _iterator;
	using iterator = _iterator<std::pair<Tkey, Tvalue>>;
	using const_iterator = _iterator<const std::pair<Tkey, Tvalue>>;

	//					###insert();###

	std::pair<iterator, bool> insert(std::pair<Tkey, Tvalue>&& x){
		return _insert(std::forward<std::pair<Tkey, Tvalue>> (x));
	}

	std::pair<iterator, bool> insert(const std::pair<Tkey, Tvalue>& x){
		return _insert(x);
	}

	std::pair<iterator, bool> _insert(const std::pair<Tkey, Tvalue>& x){
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
		return (std::pair<iterator, bool> (iterator{nullptr}, false));
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

	/** Return a pointer to the node with the smallest key*/
	node* _begin() const noexcept {
		if (!root){return nullptr;}

		node* tmp_node = root.get();
		while (tmp_node->left_child){
			tmp_node = tmp_node->left_child.get();
		}
		return tmp_node;
	}

	iterator begin() noexcept {return iterator{_begin()};}
	const_iterator begin() const noexcept {return const_iterator{_begin()};}
	const_iterator cbegin() const noexcept{return const_iterator{_begin()};}

	// return an iterator pointing to the last+1 node

	iterator end() noexcept {return iterator{nullptr};}
	const_iterator end() const noexcept {return const_iterator{nullptr};}
	const_iterator cend() const noexcept {return const_iterator{nullptr};}

	//					###find();###

	iterator find(const Tkey& search_key);

	
	//const_iterator find(const Tkey& x) const;

//					###balance();###
	void balance();


	//					###subscripting###

	Tvalue& operator[](const Tkey& x) noexcept{
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

	// check questions
	Tvalue& operator[](Tkey&& x) noexcept{
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
	/** Print the tree keys in order, starting from the smaller one and ending at the larger*/
	friend std::ostream& operator<<(std::ostream& os, const BST& t){
		for (const auto& x: t){os << x.first << " ";}
		return os;
	}

	//					###copy and move###

	// inserire copy
	BST(BST&& t) noexcept = default;
	BST& operator=(BST&& t) noexcept = default;


	//					###erase###

	void erase(const Tkey& x){

		iterator iter {find(x)};
		node* current{iter.current};

		/*if (!iter.current){
			tree is empty, handle return
		}*/

		/*if (iter == nullptr){
			node doesn't exist, handle return
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

	std::unique_ptr<node> left_child;
	std::unique_ptr<node> right_child;
	node* parent;
	std::pair<Tkey, Tvalue> pair_type;

	// default ctor
	node() = default;

	// custom ctor: passing std::pair
	node(const std::pair<Tkey, Tvalue>& Npair): 
	left_child{nullptr}, right_child{nullptr}, parent{nullptr}, pair_type{Npair} {
		std::cout << "Ctor std::pair" << std::endl;
	    //try {
	    //   	throw std::invalid_argument( "received negative value" );
	    	//AP_ERROR(false) << "Error in ManyResources ctor." << std::endl;
	    //} catch (...) {
	    //	std::cout << "inside" << std::endl;
	      //delete[] ptr;  // <----
	      //throw;         // re-throw
	    //}
	}

	// custom ctor: passing std::pair and parent
	// node(const std::pair<Tkey, Tvalue>& Npair, node* Nparent): 
	// left_child{nullptr}, right_child{nullptr}, parent{Nparent -> parent}, pair_type{Npair} {}

	// default dtor
	~node() noexcept = default;

	node& operator=(node& x) noexcept{
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

	void erase_node() noexcept{
		left_child.release();
		right_child.release();
		parent = nullptr;
		delete this;
	}

};

/** _iterator is a templated class used for the pythonic
	for loop syntax. It only contains a raw pointer to a node
	as a private member and several operators to move
	in the tree and compare/extract informations from the node 
	it's pointing at. The common use is to travers the tree from 
	the smallest key node to the biggest one.
*/
template<typename Tkey, typename Tvalue, typename Tcompare>
template <typename Tpair_i>
class BST<Tkey, Tvalue, Tcompare>::_iterator{

	friend class BST;
	using node = BST::node;

	/** this is the node the iterator is currently pointing at*/
	node* current{nullptr};

public:

	// Constructors
	/** Default constructor, the iterator point to a nullptr. */
	_iterator() = default;
	/** Costume constructor, the iterator point to the passed pointer. */
	explicit _iterator(node* pcurrent) : current{pcurrent}{};

	// Increment operators
	_iterator& operator++();
	_iterator operator++(int);
	_iterator operator+(int n);

	// dereference operator
	/** Return a reference to the pair type stored in the node.
		Note that the reference could be const or not depeding on the
		template of the _iterator. 
	*/
	Tpair_i& operator*(){return current -> pair_type;}
	Tpair_i* operator->(){return &(*(*this));}	

	// Boolean operators
	/** Return true if the two iterator are pointing to the same node */
	bool operator==(const _iterator& tmp) const {return (current == tmp.current);}
	/** Return true if the two iterator are pointing to different nodes */
	bool operator!=(const _iterator& tmp) const {return !(*this == tmp);}
};


#include "bst_iterator.cpp"
#include "bst_balance.cpp"
#include "bst_find.cpp"
#endif