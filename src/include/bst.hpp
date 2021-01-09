#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>
#include <math.h>

#ifndef BST_H
#define BST_H


template <typename K, typename T, typename CMP=std::less<K> >
class bst;

#include "bst_node.hpp"
#include "bst_iterator.hpp"



template <typename K, typename T, typename CMP >
class bst{
			
	template <typename, typename > friend class bst_node;
	
	using node = bst_node<const K,T>;
	using content_type = std::pair<const K, T>;
	using const_content_type = const std::pair<const K, T>;
	
	
	std::unique_ptr<node>  root;
	std::size_t _nodes;
	
	CMP comp_operator;	
	
	
	public:

		
	using iterator = bst_iterator<const K, T, content_type , CMP>;
  	using const_iterator = bst_iterator<const K, T, const_content_type, CMP>;
	

	
	
	//constructor(s)
		
	bst() noexcept = default;
	
	/*
	explicit bst( CMP op = CMP{} ) noexcept
	: root{nullptr}, _nodes{0}, comp_operator{op}
	{
		printf("tree constructor\n");
	}
	*/
	
	~bst() = default;
	
	
	//move semantics
	
	//bst(bst&& x ) = default;
	//bst &operator=(bst && x) = default;
	
	bst(bst&& x) 
	:root{std::move(x.root)}, _nodes{std::move(x._nodes)},  comp_operator{std::move(x.comp_operator)}
	{
		printf("move constructor\n");
		x._nodes = 0;
		x.root.reset(nullptr);
	}
	
	bst& operator=(bst && x)
	{
		printf("move assign\n");
		root = std::move(x.root);
		_nodes = std::move(x._nodes);
		comp_operator = std::move(x.comp_operator);
		return *this;
	}
	
	//copy semantics
	
	bst( const bst &x)
	:_nodes{x._nodes}, comp_operator{x.comp_operator}
	{
		printf("copy constructor\n");
		if (x.root) {
			copy_tree( root, nullptr,  x.root.get() ) ;
		}
	}
	
	
	bst& operator=( const bst &x) {
		
		printf("copy assign\n");
		root.reset();
		auto tmp_bst = x;
		*this = std::move(tmp_bst);
		return *this;
	}
	
	
	
	std::pair<iterator, bool> insert(const content_type& new_content) {
		printf("l-insert\n");
		return _insert(new_content);
	}
	
	std::pair<iterator, bool> insert(content_type&& new_content) {
		printf("r-insert\n");
		return _insert(std::move(new_content));
	}
	
	
	template<class... Types>
	std::pair<iterator,bool> emplace(Types&&... args)
	{
		return insert(content_type{(args)...});
		//return insert(content_type{std::forward<Types>(args)...});
	}

	void clear() {
		root.reset(nullptr);	
		_nodes=0;
	};
	
	iterator begin() {
		return 	iterator{endnode(root.get(),direction::left)};
	}
	
	const_iterator begin() const {
		return 	const_iterator{endnode(root.get(),direction::left)};
	}
	
	const_iterator cbegin() const {
		return 	const_iterator{endnode(root.get(),direction::left)};
	}
	
	iterator end() {
		return 	iterator{nullptr};
	}
	
	const_iterator end() const {
		return 	const_iterator{nullptr};
	}
	
	const_iterator cend() const {
		return 	const_iterator{nullptr};
	}
	
	
	iterator find(const K& key) ;
	const_iterator find(const K& key) const ;
	
	//template <typename P >
	T& operator[](const K& key) ;
	
	T& operator[](K&& key) ;

	
	size_t size() const {
		return _nodes;	
	}
	
	size_t depth() const {
		return 	_depth(root.get() );
	}
	
	bool is_balanced() const{
		return _balanced( root.get() );
	}
	
	/*
	bool has_key(const K& key ) const {
		auto find_key = key;
		auto elem = find(find_key);
		if 	( elem==end() ) {
			return false;	
		}
		else {
			return true;	
		}
	}
	*/
	
	void balance(); 
	
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
	
		
	void copy_tree(std::unique_ptr<node>& node_ptr,node* prev_node, node* copy_node) const {
		if (!copy_node) {
			node_ptr.reset(nullptr);
		}
		else {
			node_ptr.reset( new node(content_type (copy_node->content()), prev_node ) );
			copy_tree(node_ptr.get()->left(),node_ptr.get(),copy_node->left().get());	
			copy_tree(node_ptr.get()->right(),node_ptr.get(),copy_node->right().get());	
		}
	}
	
	node* endnode( node* root, direction dir) const {
		if (root) {
			switch (dir){
				case direction::left: {
					while (root->left().get()) {
						root = root->left().get();	
					}
					break;
				}
				case direction::right: {
					while (root->right().get()) {
						root = root->right().get();	
					}
					break;
				}
			}
		}
		return root;
		
	}
	
	size_t _depth( node* sub_root ) const {
		if (!sub_root) {
			return 0;
		}
		else {
			size_t left_depth{ _depth( sub_root->left().get() ) };
			size_t right_depth{ _depth( sub_root->right().get() ) };
			return std::max( left_depth, right_depth) + 1;
		}
	}
	
	bool _balanced ( node* sub_root ) const {
		bool flag = true;
		
		if (sub_root) { 
			auto depth_l = 	_depth(sub_root->left().get() );
			auto depth_r = 	_depth(sub_root->right().get() );
			long int diff = depth_l - depth_r;
			flag = ((abs( diff  )<=1)  && _balanced(sub_root->left().get()) &&  _balanced(sub_root->right().get()) );
		}
		return flag;
	}

	void rotate(node* oldroot, std::unique_ptr<node>& root_link, direction dir);
		
	void straighten_right( ) ;
	
};


template <typename K, typename T, typename CMP >
template <typename P >
std::pair< typename bst<K, T, CMP>::iterator, bool>  bst<K,T, CMP>::_insert(P&& new_content) {


		node* head = root.get();
			
		while (head) {
			//if the comparison is positive go left
			if ( comp_operator(  new_content.first,  head->content().first  ) ) {

				if (head->left().get()) {
					head = head->left().get();	
				}
				else {
					++_nodes;
					head->attach(direction::left, std::forward<P>(new_content), head );
					iterator out_iter{head->left().get()};
					return std::pair<iterator, bool> {out_iter, true};
				}

			}
			else if ( comp_operator( head->content().first, new_content.first  ) ) {
				if (head->right().get()) {
					head = head->right().get();	
				}
				else {
					++_nodes;
					head->attach(direction::right, std::forward<P>(new_content), head );
					iterator out_iter{head->right().get()};
					return std::pair<iterator, bool> {out_iter, true};
				}
			}
			else {
				//failing the comparison and reverse comparison means the values are equal
				iterator out_iter{head};
				return std::pair<iterator, bool> {out_iter, false};
			}

		}

		_nodes=1;
		root.reset(new node( new_content ) );
		//root = std::make_unique<node>(new_content);;
		iterator out_iter{root.get()};
		return std::pair<iterator, bool> {out_iter, true};


}



template <typename K, typename T, typename CMP >
typename bst<K, T, CMP>::iterator  bst<K,T, CMP>::find(const K& key) {
		
	node* head = root.get();
			
	while (head) {
		
		//if the comparison is positive go left
		if ( comp_operator(  key,  head->content().first  ) ) {
			head = head->left().get();	
		}
		else if ( comp_operator( head->content().first, key  ) ) {
			head = head->right().get();	
		}
		else {
			//failing the comparison and reverse comparison means the values are equal
			iterator out_iter{head};
			return out_iter;
		}
	}
	
	return end();
}

template <typename K, typename T, typename CMP >
T&  bst<K,T, CMP>::operator[](const K& key) {
		
	node* head = root.get();
	content_type new_content{key,T{} };
	
	while (head) {

		//if the comparison is positive go left
		if ( comp_operator(  key,  head->content().first  ) ) {

			if (head->left().get()) {
				head = head->left().get();	
			}
			else {
				head->attach(direction::left, new_content, head );
				head = head->left().get();	
				return head->content().second;
			}

		}
		else if ( comp_operator( head->content().first, key  ) ) {
			if (head->right().get()) {
				head = head->right().get();	
			}
			else {
				head->attach(direction::right, new_content, head );
				head = head->right().get();	
				return head->content().second;
			}
		}
		else {
			//failing the comparison and reverse comparison means the values are equal
			std::cout <<  "I found the key, here's the value" << std::endl;
			return head->content().second;

		}

	}

	//printf("key is root\n");
	root.reset( new node(new_content) );
	//root = std::make_unique<node>(new_content);
	return root->content().second;

}

	

template <typename K, typename T, typename CMP >
T&  bst<K,T, CMP>::operator[](K&& key) {
		
	node* head = root.get();
	content_type new_content{key,T{} };
	
	while (head) {

		//if the comparison is positive go left
		if ( comp_operator(  key,  head->content().first  ) ) {

			if (head->left().get()) {
				head = head->left().get();	
			}
			else {
				head->attach(direction::left, new_content, head );
				head = head->left().get();	
				return head->content().second;
			}

		}
		else if ( comp_operator( head->content().first, key  ) ) {
			if (head->right().get()) {
				head = head->right().get();	
			}
			else {
				head->attach(direction::right, new_content, head );
				head = head->right().get();	
				return head->content().second;
			}
		}
		else {
			//failing the comparison and reverse comparison means the values are equal
			std::cout <<  "I found the key, here's the value" << std::endl;
			return head->content().second;

		}

	}

	//printf("key is root\n");
	root.reset( new node(new_content) );
	//root = std::make_unique<node>(new_content);
	return root->content().second;

}

template <typename K, typename T, typename CMP >
void bst<K,T, CMP>::balance() {
	
	if (!is_balanced() ) {
		straighten_right();

		node* head{root.get()};

		if (head) {
			size_t _size = size();
			size_t _balanced_size = static_cast<size_t>( pow(2, floor(std::log2( _size + 1 )) ) ) - 1 ;

			size_t bottom_n{  _size - _balanced_size };

			for (size_t i=0; i< bottom_n; ++i) {
				if (i==0) {
					rotate( root.get(), root , direction::left);	
					head = root.get();
				}
				else {
					rotate( head->right().get(), head->right() , direction::left);	
					head = head->right().get();
				}
			}


			head = root.get();

			auto t = _balanced_size;		
			while (t>1) {
				t = floor(static_cast<double>(t)/2);
				for (size_t i=0; i< t; ++i) {
					if (i==0) {
						rotate( root.get(), root , direction::left);	
						head = root.get();
					}
					else {
						rotate( head->right().get(), head->right() , direction::left);	
						head = head->right().get();
					}
				}

			}

		}	
		
		//final check
		if (!is_balanced() ) {
			std::cout << "error during balancing" << std::endl;	
		} else {
			std::cout << "succesfully balanced" << std::endl;		
		}
		
	}
}

template <typename K, typename T, typename CMP >
void bst<K,T, CMP>::straighten_right() {
	node* head{root.get()};
	node* prev{nullptr};

	while(head) {
		while (head->left()) {
			//rotate( head, direction::right);

			if (!prev) {
				rotate( head, root , direction::right);
				head = root.get();	
			} else {
				rotate( head, prev->right() , direction::right);
				head = prev->right().get();	
			}
		}
		prev = head;
		head = prev->right().get();
	}
}

template <typename K, typename T, typename CMP >
void bst<K,T, CMP>::rotate(node* oldroot, std::unique_ptr<node>& root_link, direction dir) {
		
	node* root_p{nullptr};
	if (oldroot->prev()) {
		root_p = oldroot->prev();
	}
	root_link.release();


	node* newroot;
	node* newsub{nullptr};

	switch (dir){
		case direction::left: {			
			//release the unique pointers to the new root and its left subtree
			newroot = oldroot->right().release();

			if (newroot->left()) {
				newsub = newroot->left().release();
			} 

			//the left subtree becomes the right subtree of the old root
			// the new root becomes the prevnode of the old root 
			oldroot->right().reset( newsub );
			oldroot->set_prev( newroot );

			//the old root becomes the prevnode of the new right subtree
			if (newsub) {
				newsub->set_prev( oldroot );
			}

			//the old root becomes the new left subtree of the new root
			//the prevnode of the new root is the root_p pointer
			newroot->left().reset( oldroot );
			newroot->set_prev( root_p );


			break;
		}
		case direction::right: {
			//release the unique pointers to the new root and its right subtree
			newroot = oldroot->left().release();

			if (newroot->right()) {
				newsub = newroot->right().release();
			} 

			//the right subtree becomes the left subtree of the old root
			// the new root becomes the prevnode of the old root 
			oldroot->left().reset( newsub );
			oldroot->set_prev( newroot );
			//the old root becomes the prevnode of the new left subtree
			if (newsub) {
				newsub->set_prev( oldroot );
			}

			//the old root becomes the new right subtree of the new root
			//the prevnode of the new root is the root_p pointer
			newroot->right().reset( oldroot );
			newroot->set_prev( root_p );
			break;

		}
	}

	root_link.reset(newroot);

}


#endif

