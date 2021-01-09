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
	
	
	//construct from a pair of key and value
	bst_node(const content_type& x) 
	: _content{x}, _prev{nullptr}
	{
		_left.reset(nullptr);
		_right.reset(nullptr);
		printf("default node constructor\n");
	}
	
	//construct from a pair of key and value and specify the previous node
	bst_node(const content_type& x, bst_node* prev_node ) 
	: _content{x}, _prev{prev_node}
	{
		_left.reset(nullptr);
		_right.reset(nullptr);
		printf("node constructor\n");
	}

	
	
	//construct from a pair of key and value
	bst_node( content_type&& x) 
	: _content{std::move(x)}, _prev{nullptr}
	{
		_left.reset(nullptr);
		_right.reset(nullptr);
		printf("def node move constructor\n");
	}
	
	//construct from a pair of key and value and specify the previous node
	bst_node( content_type&& x, bst_node* prev_node ) 
	: _content{std::move(x)}, _prev{prev_node}
	{
		_left.reset(nullptr);
		_right.reset(nullptr);
		printf("node move constructor\n");
	}



	content_type& content() {
		return _content;	
	}

	std::unique_ptr<bst_node>& left() {
		return 	_left;
	}

	std::unique_ptr<bst_node>& right() {
		return 	_right;
	}

	bst_node*  prev() const {
		return 	_prev;
	}
	
	void set_left( bst_node* leftnode) {
		_left.reset(leftnode);
	}
	
	void set_right( bst_node* rightnode) {
		_right.reset(rightnode);
	}
	
	void set_prev (bst_node* prevnode ) {
		_prev = prevnode;
	}

	void attach( direction dir, bst_node* new_node, bst_node* prev_node = nullptr){

		/*
		if (!new_node) {

		}
		*/

		switch (dir){
			case direction::left:
				_left.reset(new_node);
				_left->set_prev(prev_node);
			break;
			case direction::right:
				_right.reset(new_node);	
				_right->set_prev(prev_node);
			break;
		}

	}

	template <typename P >	
	void attach( direction dir, P&& new_content,  bst_node* prev_node = nullptr){
		
		switch (dir){
			case direction::left:
				_left.reset( new bst_node( std::forward<P>(new_content), prev_node ) );
			break;
			case direction::right:
				_right.reset( new bst_node( std::forward<P>(new_content), prev_node ) );
			break;
		}
	}

};
