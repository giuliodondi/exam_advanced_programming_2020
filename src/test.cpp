#include "bst_node.hpp"
#include "bst.hpp"
#include "bst_iterator.hpp"


#include <iostream>


int main() {
	
	//create a backwards tree, test print the empty tree
	bst<int,int,std::greater<int>> tree1{};
	std::cout << std::endl << "empty tree1\n" <<  tree1 << std::endl;
	std::cout << "is balanced? : " << tree1.is_balanced() << std::endl;

	
	//feed in positive and negative numbers
	for (auto i=1; i<10; ++i) {
		auto x = std::pair<int,int>( ( i%2) ? -i : i , int{} );
		tree1.insert(x);
	}
	
	//test print 
	std::cout << std::endl << "full unbalanced tree\n " <<  tree1 << std::endl;
	
	//test emplace
	tree1.emplace(1,111);
	tree1.emplace(3,333);
		
	//test print 
	std::cout << std::endl << "full unbalanced tree after emplace\n" <<  tree1 << std::endl;
	
	
	//test balancing 
	std::cout << "is balanced? : " << tree1.is_balanced() << std::endl;
	tree1.balance() ; 
	std::cout << std::endl << "balanced tree1\n" <<  tree1 << std::endl;
	
	//test clear
	std::cout << std::endl << "clearing tree1 " << std::endl;
	tree1.clear();
	std::cout << std::endl << "empty tree1\n" <<  tree1 << std::endl;
	
	
	//make a smaller tree
	tree1.insert(std::pair<int,int> (2,222));
	tree1.insert(std::pair<int,int> (1,111));
	tree1.insert(std::pair<int,int> (3,333));
	
	//try to insert a key that already exists
	const std::pair<int, int> mypair{2,222};
	tree1.insert(mypair);

	std::cout << std::endl << "new tree1\n" <<  tree1 << std::endl;
	
	
	//test copy constructor
	const bst<int,int,std::greater<int>> tree2{tree1};
	std::cout << std::endl << "tree2 copy of 1\n" <<  tree2 << std::endl;
	
	
	//test copy assign
	std::cout << std::endl << "clearing tree1 " << std::endl;
	tree1.clear();
	tree1 = tree2;
	std::cout << std::endl << "tree1 copy of 2\n" <<  tree1 << std::endl;
	std::cout << std::endl << "tree2\n" <<  tree1 << std::endl;
	
	
	
	//testing the iterators
	auto iter = tree1.begin();
	(iter.node()) ? std::cout << "iterator is at key " <<  iter->first << std::endl : std::cout << "iterator is at the end " << std::endl; 
	iter++;
	(iter.node()) ? std::cout << "iterator is at key " <<  iter->first << std::endl : std::cout << "iterator is at the end " << std::endl; 
	iter = tree1.end();
	(iter.node()) ? std::cout << "iterator is at key " <<  iter->first << std::endl : std::cout << "iterator is at the end " << std::endl; 
	
	
	//test find 
	int a = 3;
	auto iter2 = tree1.find(a);
	(iter2.node() && (iter2->first == a )) ? std::cout << "Key "<< a << " is present" << std::endl : std::cout << "Key "<< a << " not present " << std::endl; 
	const auto b = 4;
	iter2 = tree1.find(b);
	(iter2.node() && (iter2->first == b )) ? std::cout << "Key "<< b << " is present" << std::endl : std::cout << "Key "<< b << " not present " << std::endl;
	a = 4; 
	//std::cout << std::endl << "is key " << a << "in tree1? " <<  tree1.has_key(a) << std::endl;
	
	
	std::cout << std::endl <<"const iterators" << std::endl;
    for (auto itr = tree1.cbegin(); itr != tree1.cend(); ++itr)
    {
		(itr.node()) ? std::cout << "iterator is at key " <<  itr->first << std::endl : std::cout << "iterator is at the end " << std::endl; 
    }
	
	
	//testing iterators on an empty tree
	std::cout << std::endl << "clearing tree1 " << std::endl;
	tree1.clear();
	auto iter3 = tree1.begin();
	(iter3.node()) ? std::cout << "iterator is at key " <<  iter3->first << std::endl : std::cout << "iterator is at the end " << std::endl; 
	iter3++;
	(iter3.node()) ? std::cout << "iterator is at key " <<  iter3->first << std::endl : std::cout << "iterator is at the end " << std::endl; 
	iter3 = tree1.end();
	(iter3.node()) ? std::cout << "iterator is at key " <<  iter3->first << std::endl : std::cout << "iterator is at the end " << std::endl; 
	
	
	
	return 0;
	//std::cout <<  mytree[4] << std::endl;
	
	/*
	
	//mytree.rotate( mytree.find(12).node(), direction::left);
	//std::cout << std::endl << "tree1\n" <<  mytree << std::endl;
	
	const bst<int,int,std::greater<int>> mytree2{mytree};
	
	
	bst<int,int,std::greater<int>>::iterator iter = mytree.begin();
	//bst<int,int>::const_iterator iter3 = mytree.cbegin();
	
	//bst<int,int>::const_iterator iter2 = mytree2.begin();
	//bst<int,int>::const_iterator iter4 = mytree2.cbegin();
	
	return 0;
	
	
	std::pair<int,int> mypair (3,123);
	
	mytree.insert(mypair);
	
	const std::pair<int,int> mypair2 (1,1);
	
	mytree.insert(mypair2);
	
	mytree.insert(std::pair<int,int>(1,1));
	
	
	
	
	
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
	
	
	return 0;	
	*/
}