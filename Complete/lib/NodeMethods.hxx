#include "../include/bst.h"

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <math.h>
#include <stdexcept>

//					### move erase ###
template<typename Tkey, typename Tvalue>
void _node<Tkey, Tvalue>::move_erase(int first_lr, int second_lr){
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

//					### erase node ###
template<typename Tkey, typename Tvalue>
void _node<Tkey, Tvalue>::erase_node() noexcept{
	// need to release in order not to delete the pointed nodes
	left_child.release();
	right_child.release();
	delete this;
}