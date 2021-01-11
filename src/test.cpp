#include "bst.hpp"


#include <iostream>




int main() {
	
	//create a backwards tree, test print the empty tree
	bst<int,int,std::greater<int>> tree1{};
	std::cout << std::endl << "empty tree1\n" <<  tree1 << std::endl;
	std::cout << "is balanced? : " << tree1.is_balanced() << std::endl;
	
	//test insert
	auto ins_pair = tree1.insert(std::pair<const int,int>(0,0));
	std::cout << "did I insert the new key? " << ins_pair.second <<   std::endl;
	std::cout << "whch key? " << ins_pair.first->first <<   std::endl;
	ins_pair = tree1.insert(std::pair<const int,int>(0,0));
	std::cout << "did I insert the new key? " << ins_pair.second <<   std::endl;
	
	
	//feed in positive and negative numbers
	for (auto i=1; i<10; ++i) {
		//auto x = std::pair<const int,int>( ( i%2) ? -i : i , i );
		auto x = std::pair<const int,int>(i,i);
		tree1.insert(x);
	}
	
	//test print 
	std::cout << std::endl << "full unbalanced tree\n " <<  tree1 << std::endl;
	
	
	
	//test emplace
	tree1.emplace(-1,111);
	const int x = -3;
	const int y = -333;
	auto emp_pair = tree1.emplace(x,y);
	std::cout << "did I insert the new key? " << emp_pair.second <<   std::endl;
	std::cout << "whch key? " << emp_pair.first->first <<   std::endl;
		
	//test print 
	std::cout << std::endl << "full unbalanced tree after emplace\n" <<  tree1 << std::endl;
	
	
	//test balancing 
	std::cout << "is balanced? : " << tree1.is_balanced() << std::endl;
	tree1.balance() ; 
	std::cout << "is balanced? : " << tree1.is_balanced() << std::endl;
	std::cout << std::endl << "balanced tree1\n" <<  tree1 << std::endl;
	
	
	//test clear
	std::cout << std::endl << "clearing tree1 " << std::endl;
	tree1.clear();
	std::cout << std::endl << "empty tree1\n" <<  tree1 << std::endl;
	

	//make a smaller tree
	//std::make_pair<int&, int&>{2,222};
	int k = 2;
	int v = 222;
	auto mypair = std::pair<const int, int>(k,v);
	tree1.insert(mypair);
	k = 1;
	v = 111;
	tree1.insert(std::pair<const int, int>(k,v));
	tree1.insert(std::pair<const int,int> (3,333));
	


	std::cout << std::endl << "new tree1\n" <<  tree1 << std::endl;
	
	//test copy constructor
	const bst<int,int,std::greater<int>> tree2{tree1};
	std::cout << std::endl << "tree2 copy constructed from 1\n" <<  tree2 << std::endl;
	//modify the first tree 
	tree1.emplace(-1,-111);
	std::cout << std::endl << "tree1 after modification\n" <<  tree1 << std::endl;
	std::cout << std::endl << "tree2 \n" <<  tree2 << std::endl;
	
	//test copy assign
	std::cout << std::endl << "clearing tree1 " << std::endl;
	tree1.clear();
	tree1 = tree2;
	std::cout << std::endl << "tree1 copy assigned from 2\n" <<  tree1 << std::endl;
	std::cout << std::endl << "tree2\n" <<  tree2 << std::endl;
	
	//test move constructor
	std::cout << std::endl << "moving tree1 into tree3\n" << std::endl;
	bst<int,int,std::greater<int>>  tree3 {std::move(tree1)};
	std::cout << std::endl << "tree3\n" <<  tree3 << std::endl;
	
	
	//test move assign
	std::cout << std::endl << "moving tree3 into tree1\n"<< std::endl;
	tree1 = bst<int,int,std::greater<int>> {std::move(tree3)};
	std::cout << std::endl << "tree1\n" <<  tree1 << std::endl;
	
	
	//testing subscript operator
	std::cout << std::endl << "lookng for the l-value key 3\n"<< std::endl;
	int find=3;
	std::cout << "tree1["<< find << "]=" << tree1[find] << std::endl;
	int newval = 1234;
	std::cout << "changing the value to "<< newval << std::endl;
	tree1[find] = newval;	
	std::cout << "tree1["<< find << "]=" << tree1[find] << std::endl;
	
	std::cout << std::endl << "creating the r-value key 4\n"<< std::endl;
	tree1[4] = 4321;
	std::cout << "tree1[4]=" << tree1[4] << std::endl;

	std::cout << std::endl << "tree1\n" <<  tree1 << std::endl;
	
	//testing the iterators and *, -> operators
	std::cout << std::endl <<"iterators" << std::endl;
	auto iter = tree1.begin();
	(iter.node()) ? std::cout << "iterator is at key " <<  iter->first << std::endl : std::cout << "iterator is at the end " << std::endl; 
	bst<int,int,std::greater<int>>::iterator iter_;
	iter_ = ++iter;
	(iter_.node()) ? std::cout << "iterator is at key " <<  (*iter_).first << std::endl : std::cout << "iterator is at the end " << std::endl; 
	iter = tree1.end();
	(iter.node()) ? std::cout << "iterator is at key " <<  (*iter).first << std::endl : std::cout << "iterator is at the end " << std::endl; 
	
	
	//test find 
	int a = 3;
	int b = 5;
	auto iter2 = tree1.find(a);
	(iter2.node()) ? std::cout << "Key "<< a << " is present" << std::endl : std::cout << "Key "<< a << " not present " << std::endl; 
	iter2 = tree1.find(b);
	(iter2.node() ) ? std::cout << "Key "<< b << " is present" << std::endl : std::cout << "Key "<< b << " not present " << std::endl;

	
	//testing constant iterators on constant tree2
	std::cout << std::endl <<"const iterators" << std::endl;
	auto c_iter = tree2.find(a);
	(c_iter.node() && (c_iter->first == a )) ? std::cout << "Key "<< a << " is present" << std::endl : std::cout << "Key "<< a << " not present " << std::endl;
	c_iter = tree2.find(b);
	(c_iter.node() && (c_iter->first == b )) ? std::cout << "Key "<< b << " is present" << std::endl : std::cout << "Key "<< b << " not present " << std::endl;
    
	
	
	//testing iterations on tree and const tree
	std::cout << std::endl <<"iterating on tree1" << std::endl;
	for (auto itr = tree1.begin(); itr != tree1.end(); ++itr)
    {
		(itr.node()) ? std::cout << "iterator is at key " <<  itr->first << std::endl : std::cout << "iterator is at the end " << std::endl; 
    }
	std::cout << std::endl <<"iterating on tree" << std::endl;
	for (auto itr = tree2.begin(); itr != tree2.end(); ++itr)
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
	
	
	
}