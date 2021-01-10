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
	
	using node = bst_node<const K,T>;
	using content_type = std::pair<const K, T>;
	
	
	std::unique_ptr<node>  root;
	std::size_t _nodes;
	CMP comp_operator;	
	
	
	public:

		
	using iterator = bst_iterator<const K, T, content_type , CMP>;
  	using const_iterator = bst_iterator<const K, T, const content_type, CMP>;
	
	
	
	//constructor(s)
	
	
	explicit bst( )
	: root{nullptr}, _nodes{0}, comp_operator{}
	{
		//printf("tree constructor\n");
	}
	
	
	~bst() noexcept = default;
	
	
	//move semantics
	
	bst(bst&& x ) noexcept = default;
	bst &operator=(bst && x) noexcept  = default;
	

	//copy semantics
	
	bst( const bst &x)
	:_nodes{x._nodes}, comp_operator{x.comp_operator}
	{
		//printf("copy constructor\n");
		if (x.root) {
			copy_tree( root, nullptr,  x.root.get() ) ;
		}
	}
	
	
	bst& operator=( const bst &x) {
		
		//printf("copy assign\n");
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
		//printf("l-insert\n");
		return _insert(new_content);
	}
	
	std::pair<iterator, bool> insert(content_type&& new_content) {
		//printf("r-insert\n");
		return _insert(std::move(new_content));
	}
	
	template<class... Types>
	std::pair<iterator,bool> emplace(Types&&... args)
	{
		return insert(content_type{(args)...});
		//return insert(content_type{std::forward<Types>(args)...});
	}
		
	
	T& operator[](const K& key) {
		return 	subscripting( key );
	}
	
	T& operator[](K&& key) {
		return 	subscripting( std::move(key) );
	}

	
	iterator find(const K& key) noexcept {
		return 	iterator{_find( key )};	
	}
	
	const_iterator find(const K& key) const noexcept {
		return 	const_iterator{_find( key )};	
	}

	
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
	
	
	auto size() const {
		return _nodes;	
	}
	
	auto depth() const {
		return 	_depth(root.get() );
	}
	
	auto is_balanced() const{
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
	
	template <typename O >
	T& subscripting( O&& key) {
		auto out_pair{_insert(content_type(key,T{}))};
		return out_pair.first->second;
	};
	
	node* _find(const K& key) const;
		
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
	
	node* firstnode( node* head) const {
		if (head) {
			while (head->left()) {
				head = head->left();	
			}
		}
		return head;
	}
	
	
	size_t _depth( node* head ) const {
		if (!head) {
			return 0;
		}
		else {
			size_t left_depth{ _depth( head->left() ) };
			size_t right_depth{ _depth( head->right() ) };
			return std::max( left_depth, right_depth) + 1;
		}
	}
	
	bool _balanced ( node* head ) const {
		bool flag = true;
		if (head) { 
			auto depth_l = 	_depth(head->left());
			auto depth_r = 	_depth(head->right() );
			long int diff = depth_l - depth_r;
			flag = ((abs( diff  )<=1)  && _balanced(head->left()) &&  _balanced(head->right()) );
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

			if (head->left()) {
				head = head->left();	
			}
			else {
				++_nodes;
				head->attach(direction::left, std::forward<P>(new_content) );
				iterator out_iter{head->left()};
				return std::pair<iterator, bool> {out_iter, true};
			}

		}
		else if ( comp_operator( head->content().first, new_content.first  ) ) {
			if (head->right()) {
				head = head->right();	
			}
			else {
				++_nodes;
				head->attach(direction::right, std::forward<P>(new_content) );
				iterator out_iter{head->right()};
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
	iterator out_iter{root.get()};
	return std::pair<iterator, bool> {out_iter, true};


}

/*
template <typename K, typename T, typename CMP >
template <typename O >
T&  bst<K,T, CMP>::subscripting( O&& key) {
		
	node* head = root.get();
	content_type new_content{key,T{} };
	
	while (head) {

		//if the comparison is positive go left
		if ( comp_operator(  key,  head->content().first  ) ) {

			if (head->left()) {
				head = head->left();	
			}
			else {
				head->attach(direction::left, new_content, head );
				head = head->left();	
				return head->content().second;
			}

		}
		else if ( comp_operator( head->content().first, key  ) ) {
			if (head->right()) {
				head = head->right();	
			}
			else {
				head->attach(direction::right, new_content, head );
				head = head->right();	
				return head->content().second;
			}
		}
		else {
			//failing the comparison and reverse comparison means the values are equal
			return head->content().second;

		}

	}

	root.reset( new node(new_content) );
	return root->content().second;

}
*/

template <typename K, typename T, typename CMP >
typename bst<K,T, CMP>::node*  bst<K,T, CMP>::_find(const K& key) const{
//typename bst<K, T, CMP>::iterator  bst<K,T, CMP>::find(const K& key) {
		
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
			//iterator out_iter{head};
			//return out_iter;
			break;
		}
	}
	
	return head;
}




template <typename K, typename T, typename CMP >
void bst<K,T, CMP>::balance() {
	
	if (!is_balanced() ) {
		straighten_right();

		node* head{root.get()};


		size_t _size = size();
		size_t _balanced_size = static_cast<size_t>( pow(2, floor(std::log2( _size + 1 )) ) ) - 1 ;

		size_t bottom_n{  _size - _balanced_size };

		for (size_t i=0; i< bottom_n; ++i) {
			if (i==0) {
				rotate( root.get(), root , direction::left);	
				head = root.get();
			}
			else {
				rotate( head->right(), head->_right , direction::left);	
				head = head->right();
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
					rotate( head->right(), head->_right , direction::left);	
				head = head->right();
				}
			}

		}

		/*
		//final check
		if (!is_balanced() ) {
			std::cout << "error during balancing" << std::endl;	
		} else {
			std::cout << "succesfully balanced" << std::endl;		
		}
		*/
		
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
				rotate( head, prev->_right, direction::right);
				head = prev->right();	
			}
		}
		prev = head;
		head = prev->right();
	}
}

template <typename K, typename T, typename CMP >
void bst<K,T, CMP>::rotate(node* oldroot, std::unique_ptr<node>& root_link, direction dir) {
		
	node* root_p{nullptr};
	if (oldroot->prev()) {
		root_p = oldroot->prev();
	}
	
	//detach the old root from whatever is above
	root_link.release();

	node* newroot;
	node* newsub{nullptr};

	switch (dir){
		case direction::left: {			
			//detach the new root from the old root
			//detach the left subtree from the new root if it exists
			
			newroot = oldroot->detach_right();
			if (newroot->_left) {
				newsub = newroot->detach_left();
			} 
			
			//attach the subtree to the old root
			//attach the old root to the new root
			
			newroot->attach( direction::left, oldroot);
			if (newsub) {
				oldroot->attach( direction::right, newsub);
			}

			break;
		}
		case direction::right: {
			//detach the new root from the old root
			//detach the right subtree from the new root if it exists
			
			newroot = oldroot->detach_left();
			if (newroot->_right) {
				newsub = newroot->detach_right();
			} 
			
			//attach the subtree to the old root
			//attach the old root to the new root
			
			newroot->attach( direction::right, oldroot);
			if (newsub) {
				oldroot->attach( direction::left, newsub);
			}

			break;
		}
	}
	
	//fix the link between new root and parent
	newroot->set_prev( root_p );
	root_link.reset(newroot);

}


#endif

