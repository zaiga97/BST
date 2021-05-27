#ifndef __bst
#define __bst

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <math.h>
#include <stdexcept>


	
/** \class _iterator
	\brief iterator class templated over std::pair<Tkey, Tvalue> with two aliases
	\tparam Tpair_i template parameter 
*/
template <typename Tkey_i, typename Tvalue_i, typename Tpair_i>
class _iterator;

/** \struct node
	\brief Node struct
*/
template <typename Tkey, typename Tvalue>
struct _node; 

/**	\class BST 
	\brief Binary Search Tree Class
	\tparam Tkey Key type
	\tparam Tvalue Value Type
	\tparam Tcompare Compare Type
*/

template<typename Tkey, typename Tvalue, typename Tcompare = std::less<Tkey>>
class BST{

	using node = _node<Tkey, Tvalue>;
	using iterator = _iterator<Tkey, Tvalue, std::pair<Tkey, Tvalue>>;
	using const_iterator = _iterator<Tkey, Tvalue, const std::pair<Tkey, Tvalue>>;

	/**	\brief recursive function that builds a queue of integers such that, using this order
		to insert a sorted array of nodes into an empty tree we end up with a balanced tree.
		\param i start index
		\param f end index
		\param q queue
	*/
	void build_balance_queue(int i, int f, std::queue<int>& q);


	void build_copy_queue(node* root, std::queue<std::pair<Tkey, Tvalue>>& q);

public:

	/** Yields the implementation-defined strict total order */
	Tcompare lt;

	/** pointer to the root node */
	std::unique_ptr<node> root;

	/** \brief default constructor */
	BST() = default;

	/** \brief default destructor */
	~BST() noexcept = default;

	/** \class _iterator
		\brief iterator class templated over std::pair<Tkey, Tvalue> with two aliases
		\tparam Tpair_i template parameter 
	*/


	/**	\brief Wrapper for _insert
		\param std::pair<Tkey, Tvalue>&& x
	*/
	std::pair<iterator, bool> insert(std::pair<Tkey, Tvalue>&& x){
		return _insert(std::move(x));
	}

	/**	\brief Wrapper for _insert
		\param const std::pair<Tkey, Tvalue>& x
	*/
	std::pair<iterator, bool> insert(const std::pair<Tkey, Tvalue>& x){
		return _insert(x);
	}

	/** 
		\brief inserts a new node in the BST if the passed key is not already in the tree.
		\param const std::pair<Tkey, Tvalue>& x
		\return std::pair<iterator, bool>. The iterator points to the new node
		while the bool returns true if the node was allocated, false otherwise.
	*/
	template <typename O>
	std::pair<iterator, bool> _insert(O&& x);

	//					###emplace();###
	/** \brief Insert a new element into the container constructed in-place.
		\Tparam Types
		\param args: args passed to insert function.
		\return std::pair<iterator, bool>: the pointer points to the new node
		while the bool returns true if the node was allocated, false otherwise.
	*/
	template<typename... Types >
	std::pair<iterator,bool> emplace(Types&&... args){
		// std::move unconditionally transforms the passed argument in an rvalue
		// std::forward forwards r(l)values if r(l)values are passed
		return insert(std::pair<Tkey, Tvalue> (std::forward<Types>(args)...));
	}

	/** \brief clear the content of the tree */
	void clear(){
		root.reset();
	}

	/** \brief return a pointer to the node with the smallest key 
		\return node* */
	node* _begin() const noexcept;

	/** \brief Wrapper for _begin
		\return iterator
	*/
	iterator begin() noexcept {return iterator{_begin()};}

	/** \brief Wrapper for _begin
		\return const_iterator 
	*/
	const_iterator begin() const noexcept {return const_iterator{_begin()};}

	/** \brief Wrapper for _begin
		\return const_iterator
	*/
	const_iterator cbegin() const noexcept {return const_iterator{_begin()};}

	/** \brief return a pointer to the the last node + 1
		\return iterator
	*/
	iterator end() noexcept {return iterator{nullptr};}

	/** \brief return a pointer to the the last node + 1
		\return const_iterator
	*/
	const_iterator end() const noexcept {return const_iterator{nullptr};}

	/** \brief return a pointer to the the last node + 1
		\return const_iterator
	*/
	const_iterator cend() const noexcept {return const_iterator{nullptr};}

	/** \brief Find a given key
		\return pointer to a node with the key == search_key found in the tree. 
		If the key is not present then it returns a pointer to nullptr.
	*/

	node* _find(const Tkey& search_key);

	/** \brief Wrapper for _find
		\return iterator to a node with the key == search_key found in the tree. 
		If the key is not present then it returns an iterator pointing to one past the last element, 
		a.k.a end().
	*/
	iterator find(const Tkey& search_key);

	/** \brief Find a given key
		\return const_iterator to a node with the key == search_key found in the tree. 
		If the key is not present then it returns an iterator pointing to one past the last element, 
		a.k.a end().
	*/
	const_iterator find(const Tkey& search_key) const;

	/** \brief Balance the tree. It copies the content of the tree in a std::vector using 
		an iterator. A new tree is generated and the content of the vector is inserted in 
		such a way to keep the new tree as balanced as possible. In the end a move between the new
		tree and the old one is performed.
	*/
	void balance();


	//					###subscripting###

	/** \brief Overloading of the subscripting operator.
		\param const Tkey& x.
		\return Tvalue&: reference to the value that is mapped to the key equivalent to x,
		performing an insertion if the node with such key is not present.
	*/
	Tvalue& operator[](const Tkey& x) noexcept;

	/** \brief Overloading of the subscripting operator.
		\param Tkey&& x.
		\return Tvalue&: reference to the value that is mapped to the key equivalent to x,
		performing an insertion if the node with such key is not present.
	*/
	Tvalue& operator[](Tkey&& x) noexcept;
	

	//					###put_to<<###
	/** \brief Overloading of put-to operator.
		\param std::ostream& os
		\param const BST& t
		\return os
	*/
	friend std::ostream& operator<<(std::ostream& os, const BST& t){
		for (const auto& x: t){os << x.first << " ";}
		return os;
	}

	//					###copy and move###

	BST(BST&& t) noexcept = default;
	BST& operator=(BST&& t) noexcept = default;
 

    BST(BST& other_t);


	//					###erase###
	/** \brief Erase a node from the bst while keeping the binary search properties intact.
		3 possible cases are possible:
		- x is a leaf: the function simply deletes it
		- x has one child: the function places the child in its place
		- x has two children: erase places the inorder successor in its place
		note: the inorder successor is the leftmost node in the subtree to the right of x
		\param const Tkey& x
	*/
	void erase(const Tkey& x);


};

/**	\struct BST::node
	\brief node class
	\tparam Tkey Key type
	\tparam Tvalue Value Type
	\tparam Tcompare Compare Type
*/

template<typename Tkey, typename Tvalue>
struct _node{

	std::unique_ptr<_node> left_child;
	std::unique_ptr<_node> right_child;
	_node* parent;
	std::pair<Tkey, Tvalue> pair_type;

	/** \brief default constructor */
	_node() = default;

	/** \brief custom constructor 
		\param const std::pair<Tkey, Tvalue>& Npair
	*/
	_node(const std::pair<Tkey, Tvalue>& Npair): 
	left_child{nullptr}, right_child{nullptr}, parent{nullptr}, pair_type{Npair} {}

	/** \brief default dtor */
	~_node() noexcept = default;

	/** \brief move assignement
		\param node&& x
		\return node&
	*/
	_node& operator=(_node&& x) noexcept = default;

	// 					###move_erase###
	/** \brief support function for erase to avoid code repetition as much as possible
		\param int first_lr: first selector
		\param int second_lr: second selector
	*/
	void move_erase(int first_lr, int second_lr);

	//					###erase node###
	/** \brief erase node and free memory. Used in the erase method.*/
	void erase_node() noexcept;

};

/** \class BST::_iterator
	\brief _iterator is a templated class used for the pythonic
	for loop syntax. It only contains a raw pointer to a node
	as a private member and several operators to move
	in the tree and compare/extract informations from the node 
	it's pointing at. The common use is to travers the tree from 
	the smallest key node to the biggest one.
*/

template <typename Tkey_i, typename Tvalue_i, typename Tpair_i> // -> node s
class _iterator{
	public:
	using node = _node<Tkey_i, Tvalue_i>;

	/** /brief current node the iterator is pointing to*/
	node* current{nullptr};

	// Constructors
	/** \brief Default constructor, the iterator points to nullptr. */
	_iterator() = default;
	/** \brief Custom constructor, the iterator points to the passed pointer. */
	explicit _iterator(node* pcurrent) : current{pcurrent} {};

	// Increment operators
	/** \brief Overloading of the pre-increment operator.
		This pre-increment operator is the core of the _iterator class 
		and it's responsable for the traversing of the tree. All it does
		is changing the node the iterator is pointing to the one in 
		the tree with the next larger key. If such a node it's not present,
		(it's pointing to the node with biggest key in the tree), returns
		an iterator pointing to a nullptr.*/

	_iterator& operator++();
	/** \brief Overloading of the post-increment operator*/
	//_iterator operator++(int);
	/** \brief Overloading of the n-increment operator. No bound checking.*/
	//_iterator operator+(int n);

	// dereference operator
	/** \brief Overloading of the dereference operator
		\return reference to the pair type stored in the node.
		Note that the reference could be const or not depeding on the
		template of the _iterator.
	*/
	Tpair_i& operator*(){return current -> pair_type;}

	/** \brief Overloading of the arrow operator
		\return pointer to the pair type stored in the node.
		Note that the reference could be const or not depeding on the
		template of the _iterator.
	*/

	Tpair_i* operator->(){return &(*(*this));}

	// Boolean operators
	/** \brief Overloading of the equality operator
		\return bool: true if the two iterator are pointing to the same node 
	*/
	bool operator==(const _iterator& tmp) const {return (current == tmp.current);}

	/** \brief Overloading of the equality operator
		\return bool: true if the two iterator are not pointing to the same node 
	*/
	bool operator!=(const _iterator& tmp) const {return !(*this == tmp);}
};



#include "../lib/bst.cpp"
#include "../lib/node.cpp"
#include "../lib/iterator.cpp"
#endif