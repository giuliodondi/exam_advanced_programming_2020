#pragma once

#include <memory>
#include <utility>
#include <iostream>



enum class direction{left, right};

template <typename K, typename T>
class bst_node {
	
	template <typename, typename, typename > friend class bst;
	
	using content_type = std::pair<const K, T>;
	
	content_type _content;
	std::unique_ptr<bst_node> _left,_right;
	//the pointer to the previous node cannot be unique
	bst_node* _prev;

	public:
	
	/*
	//construct from a pair of key and value
	bst_node(const content_type& x) 
	: _content{x}, _prev{nullptr}
	{
		_left.reset(nullptr);
		_right.reset(nullptr);
		printf("default node constructor\n");
	}
	*/
	
	//construct from a pair of key and value and specify the previous node
	explicit bst_node(const content_type& x, bst_node* prev_node = nullptr ) 
	: _content{x}, _prev{prev_node}
	{
		_left.reset(nullptr);
		_right.reset(nullptr);
		//printf("node constructor\n");
	}

	
	/*
	//construct from a pair of key and value
	bst_node( content_type&& x) 
	: _content{std::move(x)}, _prev{nullptr}
	{
		_left.reset(nullptr);
		_right.reset(nullptr);
		printf("def node move constructor\n");
	}
	*/
	
	//construct from a pair of key and value and specify the previous node
	explicit bst_node( content_type&& x, bst_node* prev_node = nullptr ) 
	: _content{std::move(x)}, _prev{prev_node}
	{
		_left.reset(nullptr);
		_right.reset(nullptr);
		//printf("node move constructor\n");
	}

	//copy semantics are never used
	bst_node( const bst_node &x) = delete;
	bst_node& operator=( const bst_node &x) = delete;

	content_type& content() {
		return _content;	
	}

	bst_node* left() {
		return 	_left.get();
	}

	bst_node* right() {
		return 	_right.get();
	}

	bst_node*  prev() const {
		return 	_prev;
	}
	
	
	void set_prev (bst_node* prevnode ) {
		//handling to prevent the creation of a closed loop in the tree
		//if the new prevnode is the node itself
		if (prevnode==this) {
			throw std::logic_error("Tried to set the parent node of a node to itself.");
			return;
		//if the new prevnode is one of the chjildren, if they are not null
		} else if ( ((_left) && (prevnode==left())) || ((_right) && (prevnode==right())) ) {
			throw std::logic_error("Tried to set the parent node of a node to  one of its children.");
			return;	
		}
		
		_prev = prevnode;
	}

	void attach( direction dir, bst_node* new_node){

		switch (dir){
			case direction::left:
				_left.reset(new_node);
				_left->set_prev(this);
			break;
			case direction::right:
				_right.reset(new_node);	
				_right->set_prev(this);
			break;
		}

	}

	template <typename P >	
	void attach( direction dir, P&& new_content){
		
		switch (dir){
			case direction::left:
				_left.reset( new bst_node( std::forward<P>(new_content), this ) );
			break;
			case direction::right:
				_right.reset( new bst_node( std::forward<P>(new_content), this ) );
			break;
		}
	}
	
	bst_node* detach_left() {
		return _left.release();	
	}
	
	bst_node* detach_right() {
		return _right.release();	
	}
	

};
