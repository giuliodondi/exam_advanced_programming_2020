#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <iostream>
#include <ctime>
#include <fstream>
#include <math.h>
#include <chrono>

#include "bst.hpp"

struct mydata {
	
	double lookup_avg_t{0};
	double balance_t{0};
	long int comps{0};
	
};


template <typename T, typename CMP>
struct comp_op {
	
	size_t c;
	CMP op;
	
	comp_op()
	: c{0}, op{}
	{}


	bool operator()( const T& a, const T& b) {
		++c;
        return op(a,b);
    }

	auto count() const {
		return c;
	}
	

};





template<typename TREE , typename VEC_T >
void full_lookup( int n , TREE* mytree_p , mydata* data ,  VEC_T* elem_vec ) {
	
	
	auto local_vec = *elem_vec;
	
	auto count_before{ mytree_p->key_comp().count() };
		
	//full lookup for the std::map
	auto start = std::chrono::system_clock::now();
	for (int k =0; k < n; k++){
		mytree_p->find(local_vec[k]);
	}
	auto end = std::chrono::system_clock::now();
	
	auto count_after{ mytree_p->key_comp().count() };
	
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	double avgdt{ static_cast<double>(elapsed.count())/static_cast<double>(n)};
	
	data->comps = ( count_after - count_before);
	data->lookup_avg_t = avgdt;

}

template<typename TREE >
void balance_my_tree( TREE* mytree_p , mydata* data ) {


	auto start = std::chrono::system_clock::now();
	
	mytree_p->balance();
	
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	double balancet = static_cast<double>(elapsed.count());
	data->balance_t = balancet;
}



int main(int argc, char ** argv) {
	
	if (argc <5) {
		std::cerr << "Usage : "<< argv[0] << " tree_max_size tree_min_size size_step output_file"  << std::endl;
		exit(1);
	}
	
	int max_n{atoi(argv[1])};
	int min_n{atoi(argv[2])};
	int step{atoi(argv[3])};
	std::string outname{argv[4]};
	
	
	
	std::ofstream outfile;
	outfile.open(outname);

	if (outfile.is_open()) {
		outfile << "size,worst_m,worst_m_c,worst_t,worst_t_c,worst_t_bal_t,worst_t_b,worst_t_b_c,";
		outfile << "shuf_m,shuf_m_c,shuf_t,shuf_t_c,shuf_t_bal_t,shuf_t_b,shuf_t_c\n";
	}
	outfile.close();
	
		
	using T = int;
	using comp_t = comp_op<T,std::less<T>>;
	using pair_t = std::pair<const T,T>;
	using tree_t = bst<T,T,comp_t> ;
	using map_t = std::map<T,T,comp_t> ;
	using vec_t = std::vector<T>;
	
	
	for (int n=min_n; n< max_n; n+=step ) {
		
		//std::cout << "\rCurrent size : " << n << std::flush;
	
		//elements in order
		vec_t elem;
		for (int k =0; k < n; k++){
			elem.push_back(static_cast<T>(k));
		}

		//vector of shuffled elements
		vec_t shuf_elem{elem};
			
		std::random_device dev;
		std::mt19937 gen(dev());
		std::shuffle(shuf_elem.begin(),shuf_elem.end(),gen);
		
		std::cout << elem.size() << std::endl;
		std::cout << shuf_elem.size() << std::endl;
		
		map_t std_map{};
		mydata worst_map_data;
		
		
		tree_t worst_tree{};
		mydata worst_tree_data;
		mydata worst_tree_balanced_data;
		
		
		for (int k =0; k < n; k++){
			auto x = pair_t(elem[k],elem[k]);
			std_map.insert( x );
			worst_tree.insert( x );
		}
		
		
		full_lookup( n , &std_map, &worst_map_data, &shuf_elem ) ;
		full_lookup( n , &worst_tree, &worst_tree_data, &shuf_elem ) ;
		
		balance_my_tree( &worst_tree, &worst_tree_balanced_data );
		
		full_lookup( n , &worst_tree, &worst_tree_balanced_data, &shuf_elem ) ;
		
		worst_tree.clear();
		std_map.clear();
		
		mydata shuf_map_data;
		

		tree_t shuf_tree{};
		mydata shuf_tree_data;
		mydata shuf_tree_t_balanced_data;
		
		for (int k =0; k < n; k++){
			auto x = pair_t(shuf_elem[k],shuf_elem[k]);
			shuf_tree.insert( x );
			std_map.insert( x );
		}
		
		full_lookup( n , &std_map, &shuf_map_data, &shuf_elem ) ;
		full_lookup( n , &shuf_tree, &shuf_tree_data, &shuf_elem ) ;
		
		balance_my_tree( &shuf_tree, &shuf_tree_t_balanced_data );
		
		full_lookup( n , &shuf_tree, &shuf_tree_t_balanced_data, &shuf_elem ) ;
		
		

		std::ofstream outfile;
		outfile.open(outname, std::ios_base::app);

		if (outfile.is_open()) {
			outfile << n << " " <<  worst_map_data.lookup_avg_t << " " <<  worst_map_data.comps ;
			
			outfile << " " <<  worst_tree_data.lookup_avg_t << " " <<  worst_tree_data.comps ;
			outfile << " " <<  worst_tree_balanced_data.balance_t ;
			outfile << " " <<  worst_tree_balanced_data.lookup_avg_t << " " <<  worst_tree_balanced_data.comps ;
			
			outfile << " " <<  shuf_map_data.lookup_avg_t << " " <<  shuf_map_data.comps ;
			
			outfile << " " <<  shuf_tree_data.lookup_avg_t << " " <<  shuf_tree_data.comps ;
			outfile << " " <<  shuf_tree_t_balanced_data.balance_t ;
			outfile << " " <<  shuf_tree_t_balanced_data.lookup_avg_t << " " <<  shuf_tree_t_balanced_data.comps ;
			
			outfile << "\n";
			outfile.close();
		}
		
		
	
	}
	
	return 0;
	
	
}