#pragma once

#include <iostream>
//#include <iterator>




template <typename K, typename T, typename CONT, typename CMP>
class bst_iterator {
	
	using node_t = bst_node<const K,T>;
	node_t* cur;
		
	public:
	
		
	using content_type = CONT ;
		
	using content_ref = content_type &;
	using content_ptr = content_type *;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;
	
	bst_iterator() = default;
	explicit bst_iterator(node_t* n) noexcept
	: cur{n}
	{}
	
	~bst_iterator() = default;
	
	content_ref operator*() const noexcept {
		return cur->content();
	}
	
	content_ptr operator->() const noexcept{
		return &**this;	
	}
	
	
	friend bool operator == (const  bst_iterator &a , const bst_iterator &b ) noexcept {
		return a.cur == b.cur;	
	}
	
	friend bool operator != ( const bst_iterator &a , const bst_iterator &b ) noexcept {
		return !(a==b);	
	}
	
	
	bst_iterator& operator++()  {
		cur = nextnode( cur);
		return *this;
	}
	
	bst_iterator operator++(int)  {
		auto tmp{*this};
		++(*this);
		return tmp;
	}
	
	node_t* node( ) noexcept {
		return cur;
	}
	
	node_t* node( ) const noexcept {
		return cur;
	}
	  
	  
	private:
	
	node_t* nextnode( node_t* head) const;
	
};

//returns a pointer to the next node in the tree
template <typename K, typename T, typename CONT, typename CMP>
bst_node<const K,T>* bst_iterator<K,T,CONT,CMP>::nextnode( node_t* head) const {

	/*
	head starts at the current node 

	if the current node has right descendants:
		move head to the right descendant and then go as far left as possible
	if no right descendants:
		move up to the previous node but save the pointer to the current node in tmp
		check if we came from the left or right by comparing the pointers with tmp
		if tmp is left
			break the loop because the current head is the next node
		if tmp is right
			keep climbing up and reassign tmp

		if the previous node is null pointer this means we climbed above the root
		then by returning head(nullptr) we signal that we are at the end

	*/

	if (head) {
		if (head->right()) {

			#ifdef TREE_DBG
			std::cout <<"moving to the right" << std::endl;
			#endif

			head = head->right();
			while ( head->left()) {
				#ifdef TREE_DBG
				std::cout <<"descending to the left" << std::endl;
				#endif
				head = head->left();
			}
		}
		else {
			#ifdef TREE_DBG
			std::cout <<"moving up"  << std::endl;
			#endif
			auto tmp{head};
			head = head->prev()	;

			while  (head) {
				if (head->left() == tmp) {
					break;
				}
				else if (head->right() == tmp) {
					#ifdef TREE_DBG
					std::cout <<"moving up" << std::endl;
					#endif
					tmp = head;
					head = head->prev()	;
				}
			}
			#ifdef TREE_DBG
			if (!head) {
				std::cout <<"I'm at the top" << std::endl;
			}
			#endif

		}


	}
	return head;
}
	