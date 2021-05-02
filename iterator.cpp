#include "bst.h"
#include <iostream>
#include <vector>
#include <memory>

template<typename Tkey, typename Tvalue, typename Tcompare>
class BST<Tkey, Tvalue, Tcompare>::iterator{

	friend class BST;

	using node = BST<Tkey, Tvalue, Tcompare>::node;

	node* current;

	// default ctor
	iterator() = default;

	// iterator ctor from a node
	iterator(const node* Pcurrent) : current{Pcurrent}{};

	// post-increment, fix search
	iterator& operator++()
	{
		// If current has no right children then i have to search a new branch to move down
		if (!current->right_child){
			// Find the first node for who current is a left descendant
			while (current->parent.right_child.get() == current){ current = current->parent; }
			current = current->parent;
			return *this;
		}
		// Now if my node has a right child then the next node wiil be the leftmost on the right sub-tree:
		else{
			// Move on the right branch
			current = current->right_child.get();
			// Get to the leftmost node on this new branc
			while (current->left_child){ current = current->let_child.get(); }
			return *this;
		}		
	}

	// pre-increment
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
	Tvalue& operator*()
	{
		return current -> pair_type.second;
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
		return (current->pair_type.first == tmp.current->pair_type.first && current->pair_type.second == tmp.current->pair_type.second);
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