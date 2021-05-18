#include "bst.h"
#include <iostream>
#include <vector>
#include <memory>

template<typename Tkey, typename Tvalue, typename Tcompare>
template <typename Tpair_i> // Used to make a const_iterator
class BST<Tkey, Tvalue, Tcompare>::iterator_temp{
public:

	friend class BST;
	using node = BST::node;

	node* current{nullptr};

	// default ctor
	// fix "0" error
	iterator_temp() = default;

	// iterator ctor from a node
	iterator_temp(node* Pcurrent) : current{Pcurrent}{};

	// pre-increment, fix search
	iterator_temp& operator++()
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
	iterator_temp operator++(int)
	{
		// copy in temp
		auto tmp{*this};
		// pre increment
		++(*this);
		// return temp
		return tmp;
	}
	

	// dereference operator
	Tpair_i& operator*()
	{
		return current -> pair_type;
	}

	// arrow operator, returns address to value
	Tpair_i* operator->()
	{
		/*  meaning:
			*this = myself
			*(*this) = calls dereference function
			&(*(*this)) = address of returned object and return a pointer
		*/
		return &(*(*this));
	}


	// sum operator, useless
	iterator_temp operator+(int n)
	{
		for (unsigned int i = 0; i < n; i++){
			*this++;
		}
		return *this;
	}

	// == operator, should have 2 args?
	bool operator==(const iterator_temp& tmp) const 
	{
		// return true if the two current nodes have same key and value
		return (current == tmp.current);
	}

	// != operator
	bool operator!=(const iterator_temp& tmp) const 
	{
		// uses == definition
		return !(*this == tmp);
	}
};