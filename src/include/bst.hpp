#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>
#include <math.h>

#ifndef BST_H
#define BST_H

//declaration needed by the node and iterator classes
template <typename K, typename T, typename CMP=std::less<K> >
class bst;

//enum to define directions
enum class direction{left, right};

#include "bst_node.hpp"
#include "bst_iterator.hpp"

//exception structure to throw a custom message during the balancing
struct exception_during_balance {
	std::string except_msg;
	
	exception_during_balance(std::string s)
	: except_msg{std::move(s)}
	{}
	
	const char* what() const { return except_msg.c_str(); }
};


template <typename K, typename T, typename CMP >
class bst{
	
	using node = bst_node<const K,T>;
	using content_type = std::pair<const K, T>;
	
	std::unique_ptr<node>  root;
	std::size_t _nodes;
	CMP comp_operator;	
	
	public:
	
		
	using iterator = bst_iterator<const K, T, content_type>;
  	using const_iterator = bst_iterator<const K, T, const content_type>;
	
	
	
	//constructor(s)
	
	
	explicit bst( )
	: root{nullptr}, _nodes{0}, comp_operator{}
	{}
	
	
	~bst() noexcept = default;
	
	
	//move semantics
	
	bst(bst&& x ) noexcept = default;
	bst &operator=(bst && x) noexcept  = default;
	

	//copy semantics
	
	bst( const bst &x)
	:_nodes{x._nodes}, comp_operator{x.comp_operator}
	{
		try{
			if (x.root) {
				copy_tree( root, nullptr,  x.root.get() ) ;
			}
		} catch (const std::bad_alloc& e) {
			 std::cout << "Allocation error during tree copy: " << e.what() << std::endl;
		}
	}
	
	
	bst& operator=( const bst &x) {
		root.reset();
		auto tmp_bst = x;
		*this = std::move(tmp_bst);
		return *this;
	}
	
	void clear() {
		root.reset(nullptr);	
		_nodes=0;
	};
	
	
	std::pair<iterator, bool> insert(const content_type& new_content) {
		auto out_pair = std::pair<iterator, bool> {end(), false};
		try{
			out_pair = _insert(new_content);
		} catch (const std::bad_alloc& e) {
			 std::cout << "Allocation error during node insertion: " << e.what() << std::endl;
		}
		return out_pair;
	}
	
	std::pair<iterator, bool> insert(content_type&& new_content) {
		auto out_pair = std::pair<iterator, bool> {end(), false};
		try{
			out_pair = _insert(std::move(new_content));
		} catch (const std::bad_alloc& e) {
			 std::cout << "Allocation error during node insertion: " << e.what() << std::endl;
		}
		return out_pair;
	}
	
	template<class... Types>
	std::pair<iterator,bool> emplace(Types&&... args)
	{
		return insert(content_type{(args)...});
	}
		
	
	T& operator[](const K& key) {
		return 	subscripting( key );
	}
	
	T& operator[](K&& key) {
		return 	subscripting( std::move(key) );
	}

	iterator find(const K& key) noexcept;
	const_iterator find(const K& key) const noexcept ;
	
	
	iterator begin() noexcept {
		return 	iterator{firstnode(root.get())};
	}
	
	const_iterator begin() const noexcept {
		return 	const_iterator{firstnode(root.get())};
	}
	
	const_iterator cbegin() const noexcept {
		return 	const_iterator{firstnode(root.get())};
	}
	
	iterator end() noexcept {
		return 	iterator{nullptr};
	}
	
	const_iterator end() const noexcept {
		return 	const_iterator{nullptr};
	}
	
	const_iterator cend() const noexcept {
		return 	const_iterator{nullptr};
	}
	
	
	auto size() const noexcept {
		return _nodes;	
	}
	
	auto depth() const noexcept{
		return 	_depth(root.get() );
	}
	
	auto is_balanced() const noexcept{
		return _balanced( root.get() );
	}
	
	//returns a copy of the comparator object
	CMP key_comp() noexcept{
		return comp_operator;	
	}
	
	
	//wrapper for balancing
	//checks if the tree is actually unbalanced, if so
	//creates a copy of the original and balances in a try block
	//if anything is caught, revert
	void balance() {
		
		if (!is_balanced() ) {
			std::unique_ptr<node> tree_bk;
			try {
				copy_tree( tree_bk, nullptr, root.get() ) ;
			} catch (const std::bad_alloc& e) {
				 std::cout << "Allocation error during tree copy: " << e.what() << std::endl;
				std::cout << "Cannot continue with balancing." << std::endl;
				return;
			}

			try {
				_balance();
			}
			catch (...) {
				std::cout << "Error occured during balancing.\n"
					<< "Reverting the tree to its original state." << std::endl;
				root.reset(tree_bk.release());
			}
		}
		
	}; 
	
	
	friend std::ostream& operator<<(std::ostream& os, const bst& x) {		
		os <<  "Size[" << x.size() << "]  Depth[" << x.depth() << "]"<< std::endl;

		for ( auto iter = x.begin(); iter != x.end(); iter++) {
			os << "(" << iter->first << " , " << iter->second << ")" << std::endl;
		}
		
		return os;
	}
	

	
	
	private:
	
	
	template <typename P>
	std::pair<iterator, bool> _insert( P&& new_content);
	
	template <typename O >
	T& subscripting( O&& key) {
		auto out_pair{_insert(content_type(key,T{}))};
		return out_pair.first->second;
	};
	
	//recursive copy of nodes
	//given the previous node's pointer to this node, creates a new node
	//then does the same for the two children
	void copy_tree(std::unique_ptr<node>& node_ptr,node* prev_node, node* copy_node) const {
		if (!copy_node) {
			node_ptr.reset(nullptr);
		}
		else {
			node_ptr.reset( new node(content_type (copy_node->content()), prev_node ) );
			copy_tree(node_ptr.get()->_left,node_ptr.get(),copy_node->left());	
			copy_tree(node_ptr.get()->_right,node_ptr.get(),copy_node->right());	
		}
	}
	
	//navigates to the far left of the tree
	node* firstnode( node* head) const noexcept {
		if (head) {
			while (head->left()) {
				head = head->left();	
			}
		}
		return head;
	}
	
	//calculates the depth of each subtree recursively, selects the max
	//then adds one for the root itself
	size_t _depth( node* head ) const noexcept {
		if (!head) {
			return 0;
		}
		else {
			size_t left_depth{ _depth( head->left() ) };
			size_t right_depth{ _depth( head->right() ) };
			return std::max( left_depth, right_depth) + 1;
		}
	}
	
	//calculates the difference in depth of the left and right subtrees (must be <=1)
	//as well as if the left and right sub-trees are themselves balanced

	bool _balanced ( node* head ) const noexcept {
		bool flag = true;
		if (head) { 
			auto depth_l = 	_depth(head->left());
			auto depth_r = 	_depth(head->right() );
			long int diff = depth_l - depth_r;
			flag = ((abs( diff  )<=1)  && _balanced(head->left()) &&  _balanced(head->right()) );
		}
		return flag;
	}


	void _balance();
	
	node* build_tree_recursive( std::vector<std::unique_ptr<node>>& ptr_vec, int start, int end );
	
};


//common method to insert nodes in the right place
//navigate down and compare keys. If there is equality, no new node is attached
//special case if the tree is empty and this is the first node
template <typename K, typename T, typename CMP >
template <typename P >
std::pair< typename bst<K, T, CMP>::iterator, bool>  bst<K,T, CMP>::_insert(P&& new_content) {

	node* head = root.get();
	bool newflag=true;
	
	if(head) {
		while (head) {
			//if the comparison is positive go left
			if ( comp_operator(  new_content.first,  head->content().first  ) ) {

				if (head->left()) {
					head = head->left();	
				}
				else {
					++_nodes;
					head->attach(direction::left, std::forward<P>(new_content) );
					head = head->left();
					break;
				}

			}
			else if ( comp_operator( head->content().first, new_content.first  ) ) {
				if (head->right()) {
					head = head->right();	
				}
				else {
					++_nodes;
					head->attach(direction::right, std::forward<P>(new_content) );
					head = head->right();
					break;
				}
			}
			else {
				//failing the comparison and reverse comparison means the values are equal
				newflag=false;
				break;
			}

		}
	} else {
		_nodes=1;
		root.reset(new node( new_content ) );
		head = root.get();
	}

	iterator out_iter{head};
	return std::pair<iterator, bool> {out_iter, newflag};
}



//same navigation as the insert case, except no insertions are made
//if the node is not present, an iterator to null is returned automatically
template <typename K, typename T, typename CMP >
typename bst<K,T, CMP>::iterator  bst<K,T, CMP>::find(const K& key) noexcept{
		
	node* head = root.get();
			
	while (head) {
		
		//if the comparison is positive go left
		if ( comp_operator(  key,  head->content().first  ) ) {
			head = head->left();	
		}
		else if ( comp_operator( head->content().first, key  ) ) {
			head = head->right();	
		}
		else {
			//failing the comparison and reverse comparison means the values are equal
			break;
		}
	}
	
	return 	iterator{head};
}

//same navigation as the insert case, except no insertions are made
//if the node is not present, an iterator to null is returned automatically
template <typename K, typename T, typename CMP >
typename bst<K,T, CMP>::const_iterator  bst<K,T, CMP>::find(const K& key) const noexcept{
		
	node* head = root.get();
			
	while (head) {
		
		//if the comparison is positive go left
		if ( comp_operator(  key,  head->content().first  ) ) {
			head = head->left();	
		}
		else if ( comp_operator( head->content().first, key  ) ) {
			head = head->right();	
		}
		else {
			//failing the comparison and reverse comparison means the values are equal
			break;
		}
	}
	
	return const_iterator{head};
}


//balance the tree
//we will destroy the tree structure and create a vector of smart pointers to the nodes in order
//we first create a vector fo simple pointers to the node in order so that we can navigate
//because as we destroy the tree the iterators no longer work
//check if the new tree is balanced
template <typename K, typename T, typename CMP >
void bst<K,T, CMP>::_balance() {
	
	std::vector<node*> store_nodes_tmp;
	std::vector<std::unique_ptr<node>> store_nodes;

	for ( auto iter = begin(); iter != end(); iter++) {
		store_nodes_tmp.push_back(iter.node());
	}


	for ( auto iter = store_nodes_tmp.begin(); iter != store_nodes_tmp.end(); iter++) {

		std::unique_ptr<node> this_node;
		if ((*iter)->prev()) {
			(*iter)->detach_prev();
			this_node.reset(*iter);
		} else {
			root.release();
			this_node.reset(*iter);
		}

		store_nodes.push_back(std::move(this_node));

	}


	root.reset( build_tree_recursive( store_nodes, 0 , store_nodes.size() ) );
	root.get()->set_prev(nullptr);
	
	
	//final check
	if (!is_balanced() ) {
		throw exception_during_balance{"Balancing did not produce a balanced tree."};
	}
		
}

//recursively build a balanced tree given an std vector of smart pointers to node
//find the new root as the middle node, call on the left and right sub-arrays and attach the reulting sub-trees
//finally release the node we're pointing to and return its pointer so that ownership is transferred 
//to the node above
template <typename K, typename T, typename CMP >
typename bst<K,T, CMP>::node* bst<K,T, CMP>::build_tree_recursive( std::vector<std::unique_ptr<node>>& ptr_vec, int start, int end ) {
	
	int subtree_mid = floor( static_cast<double>(end - start)/2 );
	int sub_root_idx{start + subtree_mid};

	node* sub_root_node{ptr_vec[sub_root_idx].get() };
	
	if (subtree_mid == 0) {
		sub_root_node->_left.reset(nullptr);
		sub_root_node->_right.reset(nullptr);
	}
	else {
		node* left_subtree{build_tree_recursive( ptr_vec, start , sub_root_idx )};
		sub_root_node->attach(direction::left, left_subtree );
		sub_root_node->left()->set_prev(sub_root_node);
		
		if (sub_root_idx<end - 1) {
			node* right_subtree{build_tree_recursive( ptr_vec, sub_root_idx+1 , end )};
			sub_root_node->attach(direction::right,right_subtree  );
			sub_root_node->right()->set_prev(sub_root_node);
		}
	}
		
	return ptr_vec[sub_root_idx].release();
	
}


#endif

