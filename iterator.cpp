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

	// post-increment, fix search
	iterator& operator++()
	{
		current = current.left;
		return *this;
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
		// check if . syntax is correct of -> is
		return current.pair_type.second;
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
	bool operator==(const iterator& cmp) const 
	{
		return cmp == cmp.current;
	}

	// != operator
	bool operator!=(const iterator& cmp) const 
	{
		// uses == definition
		return !(*this == cmp);
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