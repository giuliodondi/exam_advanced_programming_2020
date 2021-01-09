#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>




template <typename K, typename T, typename CMP , bool iscost>
class bst_iterator {
	
	template<typename, typename, typename> friend class bst;
	template <typename, typename > friend class bst_node;
	
	using node_t = bst_node<const K,T>;
	node_t* cur;
		
	public:
	
	//using key_type = K;
	//using value_type = T;
	
	using content_type = typename std::conditional<iscost, const std::pair<const K, T> , std::pair<const K, T> >::type;
	
	//using content_type = std::pair<const K, T> ;
	//using content_type = const std::pair<const K, T> ;
		
	using content_ref = content_type &;
	using content_ptr = content_type *;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;
	
	bst_iterator() = default;
	explicit bst_iterator(node_t* n) : cur{n} {}
	
	~bst_iterator() = default;
	
	content_ref operator*() {
		return cur->content();
	}
	
	content_ptr operator->() {
		return &**this;	
	}
	
	
	friend bool operator == (const  bst_iterator &a , const bst_iterator &b ) {
		return a.cur == b.cur;	
	}
	
	friend bool operator != ( const bst_iterator &a , const bst_iterator &b ) {
		return !(a==b);	
	}
	
	
	bst_iterator& operator++() {
		
		cur = nextnode( cur);
		return *this;
		
	}
	
	bst_iterator operator++(int) {
		auto tmp{*this};
		++(*this);
		return tmp;
	}
	
	node_t* node( ) {
		return cur;
	}
	  
	  
	private:
	
	node_t* nextnode( node_t* head) {
	
		/*
		returns a pointer to the next node in the tree
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
			if (head->right().get()) {
			
				printf("moving to the right\n");

				head = head->right().get();
				while ( head->left().get()) {
					printf("descending to the left\n");
					head = head->left().get();
				}

			}
			else {
				printf("moving up\n");
				auto tmp{head};
				head = head->prev()	;
		
		
				while  (head) {
					if (head->left().get() == tmp) {
						break;
					}
					else if (head->right().get() == tmp) {
						printf("moving upp\n");
						tmp = head;
						head = head->prev()	;
					}
				}
					
				/*
				while ( (head) && ( head->right().get() == tmp ) ) {
					printf("moving upp\n");
					tmp = head;
					head = head->prev()	;
					if (head->left().get() == tmp) {
					
					}
				}
				*/
			}
			
			
		}
		
		return head;
		
	}
	
	
};