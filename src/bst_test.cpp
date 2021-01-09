//#include "bst.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>
#include <exception>
#include <type_traits>
#include <math.h>

enum class direction{left, right};

template <typename K, typename T>
class bst_node {
	
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
				printf("attaching left\n");
				_left.reset( new bst_node( std::forward<P>(new_content), prev_node ) );
				//_left = std::make_unique<bst_node>(std::forward<P>(new_content),prev_node);
				//_left = std::make_unique<bst_node>(new_content,prev_node);
				//_left->set_prev(prev_node);
			break;
			case direction::right:
				printf("attaching right\n");
				_right.reset( new bst_node( std::forward<P>(new_content), prev_node ) );
				//_right = std::make_unique<bst_node>(std::forward<P>(new_content),prev_node);
				//_right = std::make_unique<bst_node>(new_content,prev_node);
				//_right->set_prev(prev_node);
			break;
		}
	}

};


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


template <typename K, typename T, typename CMP=std::less<K> >
class bst{
			
	template <typename, typename > friend class bst_node;
	
	using node = bst_node<const K,T>;
	using content_type = std::pair<const K, T>;
	
	

	
	std::unique_ptr<node>  root;
	std::size_t _nodes;
	
	CMP comp_operator;	
	
	
	public:

		
	using iterator = bst_iterator<const K, T, CMP, false>;
  	using const_iterator = bst_iterator<const K, T, CMP, true>;
	

	
	
	//constructor(s)
		
	//bst() noexcept = default;
	
	explicit bst( CMP op = CMP{} ) noexcept
	: root{nullptr}, _nodes{0}, comp_operator{op}
	{
	}
	
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
	   // return insert(content_type{(args)...});
		return insert(content_type{std::forward<Types>(args)...});
	}

	void clear() {
		root.reset();	
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
	
	//template <typename P >
	T& operator[](const K& key) ;
	
	friend std::ostream& operator<<(std::ostream& os, bst& x) {	
	//friend std::ostream& operator<<(std::ostream& os, const bst& x) {
		
		os << std::endl << "Size[" << x.size() << "] Depth[" << x.depth() << "]"<< std::endl;
		
		//bst<K,T,CMP>::const_iterator  iter;
		//for (auto &iter : x) {
		for ( auto iter = x.begin(); iter != x.end(); iter++) {
			//content_type cont = node->content();
			os << "(" << iter->first << " , " << iter->second << ")" << std::endl;
			//os << "Key : " << nodecont.first << " Val : " << nodecont.second <<std::endl;
		}
		
		return os;
	}
	
	size_t size() {
		return _nodes;	
	}
	
	size_t depth() {
		return 	_depth(root.get() );
	}
	
	
	
	void rotate(node* oldroot, std::unique_ptr<node>& root_link, direction dir) {
		
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
	
	/*
	void rotate(node* oldroot,  direction dir) {
		
		node* root_p{nullptr};
		
		direction root_dir;
		
		//detects if the root of the subtree is the root fo the entire tree
		//because if so we need to reset the tree root as well
		if (oldroot->prev()) {
			root_p = oldroot->prev();	
			std::cout << "root_p key: " << root_p->content().first << std::endl;
			if (root_p->right().get() == oldroot) {
				root_dir=direction::right; 
				root_p->right().release();	
			}
			else {
				root_dir=direction::left; 
				root_p->left().release();
			}
			printf("not root\n");
				
		} else {
			root.release();
			printf("isroot\n");
		}
		
		
		
		node* newroot;
		node* newsub{nullptr};
		
		switch (dir){
			case direction::left: {			
				//release the unique pointers to the new root and its left subtree
				newroot = oldroot->right().release();
				
				std::cout << "newroot key: " << newroot->content().first << std::endl;
				
				
				if (newroot->left()) {
					newsub = newroot->left().release();
					std::cout << "newright key: " << newsub->content().first << std::endl;
				} else {
					printf("newright is null\n");	
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
				
				std::cout << "newroot key: " << newroot->content().first << std::endl;
				
				if (newroot->right()) {
					newsub = newroot->right().release();
					std::cout << "newleft key: " << newsub->content().first << std::endl;
				} else {
					printf("newleft is null\n");	
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
		
		if (root_p) {
			switch (root_dir){
				case direction::left: {	
					root_p->left().reset(newroot);
					break;
				}
				case direction::right: {	
					root_p->right().reset(newroot);
					break;
				}
			}
		} else {
			printf("resetting root\n");
			root.reset(newroot);
		} 
	}
	*/
	

	
	void straighten_right( ) {
		
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
	

	
	void balance() {
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
		
	}
	
	private:
	
	
	template <typename P>
	std::pair<iterator, bool> _insert( P&& new_content);
	
		
	void copy_tree(std::unique_ptr<node>& node_ptr,node* prev_node, node* copy_node) const {
		if (!copy_node) {
			node_ptr.reset(nullptr);
		}
		else {
			//node_ptr = std::make_unique<node>(content_type (copy_node->content()), prev_node ) ;
			node_ptr.reset( new node(content_type (copy_node->content()), prev_node ) );
			
			copy_tree(node_ptr.get()->left(),node_ptr.get(),copy_node->left().get());	
			copy_tree(node_ptr.get()->right(),node_ptr.get(),copy_node->right().get());	
			
		}
	}
	
	node* endnode( node* root, direction dir) {
		
		switch (dir){
			case direction::left:
				while (root->left().get()) {
					root = root->left().get();	
				}
			break;
			case direction::right:
				while (root->right().get()) {
					root = root->right().get();	
				}
			break;
		}
		return root;
		
	}
	
	size_t _depth( node* sub_root ) {
		if (!sub_root) {
			return 0;
		}
		else {
			size_t left_depth{ _depth( sub_root->left().get() ) };
			size_t right_depth{ _depth( sub_root->right().get() ) };
			return std::max( left_depth, right_depth) + 1;
		}
	}
	
};


template <typename K, typename T, typename CMP >
template <typename P >
//std::pair< bst_iterator<const K,T, CMP, false> , bool>  bst<K,T, CMP>::_insert(P&& new_content) {
std::pair< typename bst<K, T, CMP>::iterator, bool>  bst<K,T, CMP>::_insert(P&& new_content) {


		node* head = root.get();
			
		while (head) {

			std::cout <<  new_content.first   << " " <<  head->content().first<< std::endl;

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
				std::cout <<  "Key " <<  new_content.first   << " is already present" << std::endl;
				iterator out_iter{head};
				return std::pair<iterator, bool> {out_iter, false};

			}

		}

		printf("key is root\n");
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
			std::cout <<  "Key " <<  key   << " is present" << std::endl;
			iterator out_iter{head};
			return out_iter;

		}
	
	}
	
	printf("key is not present\n");
	return end();
}

template <typename K, typename T, typename CMP >
T&  bst<K,T, CMP>::operator[](const K& key) {
		
	node* head = root.get();
			
	/*
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
			std::cout <<  "Key " <<  key   << " is present" << std::endl;
			iterator out_iter{head};
			return out_iter;

		}
	
	}
	
	printf("key is not present\n");
	return end();
	*/
	
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

int main() {
	
	bst<int,int> mytree{};
	//bst<int,int,std::greater<int>> mytree{};
	
	/*
	mytree.insert(std::pair<int,int>(10,1));
	mytree.insert(std::pair<int,int>(8,1));
	mytree.insert(std::pair<int,int>(12,1));
	mytree.insert(std::pair<int,int>(11,1));
	*/
	

	mytree.emplace(10,0);
	mytree.emplace(5,0);
	mytree.emplace(7,0);
	mytree.emplace(20,0);
	mytree.emplace(15,0);
	mytree.emplace(30,0);
	mytree.emplace(25,0);
	mytree.emplace(40,0);
	mytree.emplace(23,0);
	
	std::cout << std::endl << "tree1\n" <<  mytree << std::endl;
	
	
	//mytree.rotate( mytree.find(10).node(), direction::left);
	//mytree.rotate( mytree.find(10).node(), direction::right);
	//mytree.rotate( mytree.find(12).node(), direction::right);
	
	//mytree.straighten_right() ; 
	mytree.balance() ; 
	std::cout << std::endl << "tree1\n" <<  mytree << std::endl;
	
	//mytree.rotate( mytree.find(12).node(), direction::left);
	//std::cout << std::endl << "tree1\n" <<  mytree << std::endl;
	
	return 0;
	
	
	std::pair<int,int> mypair (3,123);
	
	mytree.insert(mypair);
	
	const std::pair<int,int> mypair2 (1,1);
	
	mytree.insert(mypair2);
	
	mytree.insert(std::pair<int,int>(1,1));
	
	
	
	mytree.emplace(5,555);
	
	int a=7;
	mytree.emplace(a,a);
	
		
	
	std::cout << std::endl << "tree1\n" <<  mytree << std::endl;
		
	std::cout <<  mytree[4] << std::endl;
	
	
	std::cout <<  (*mytree.find(3)).second << std::endl;
	
	int i = 5;
	int val_i = mytree[i];
	
	std::cout <<  val_i << std::endl;
	
	//bst<int,int>::iterator itr = mytree.begin();
	//bst<int,int>::const_iterator c_itr = mytree.cbegin();
	
	/*
	bst<int,int> mytree2 {};
	
	mytree2 = mytree;
		
	std::cout << "tree2\n" <<  mytree2 << std::endl;
	
	mytree.clear();
	
	mytree.insert(std::pair<int,int>(2,1));
	mytree.insert(std::pair<int,int>(-1,-1));
	std::cout << mytree << std::endl;
	
	
	//bst<int,int> mytree2 {std::move(mytree)};
	//mytree2 = bst<int,int>{std::move(mytree)};
	
	
	mytree = bst<int,int>{};
	mytree.insert(std::pair<int,int>(-2,-2));
	std::cout << mytree << std::endl;
	*/
	
	return 0;	
	
}