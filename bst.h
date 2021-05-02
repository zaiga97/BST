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
	//std::pair<iterator, bool> insert(const std::pair<Tkey, Tvalue>& x);

	//					###emplace();###

	//template< class... Types >
	//std::pair<iterator,bool> emplace(Types&&... args);

	//					###clear();###

	//void clear();

	//					###iterator###

	/*		return an iteratori pointing to the leftmost node
	iterator begin();
	const_iterator begin() const;
	const_iterator cbegin() const;

			return an iteratori pointing to the last+1 node
	iterator end();
	const_iterator end() const;
	const_iterator cend() const;
	*/

	//					###find();###

	//iterator find(const key_type& x);
	//const_iterator find(const key_type& x) const;

	//					###balance();###

	//void balance();

	//					###subscripting###

	//value_type& operator[](const key_type& x);
	//value_type& operator[](key_type&& x);

	//					###put_to<<###

	//friend std::ostream& operator<<(std::ostream& os, const bst& x);

	//					###copy and move###

	//					###erase###

	//void erase(const key_type& x);
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