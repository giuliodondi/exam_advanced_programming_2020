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

struct time_avg_stdev {
	
	double t{0};
	double sq_t{0};
	double avg{0};
	double stdev{0};
	
	void accum( double dt) {
		t += dt;
		sq_t += pow(dt,2);
	}
	
	void calculate( int n ) {
		avg = t/(static_cast<double>(n) );	
		auto sqt = sq_t/(static_cast<double>(n) );	
		stdev= sqrt( sqt - pow(avg,2)  );
	}
	
	
};



template<typename TREE >
void do_the_reps( int n , int reps, TREE* mytree_p , time_avg_stdev* t_struct ,  std::vector<int>* elem_vec ) {
	
	
	auto local_vec = *elem_vec;
	
	for (int j =0; j < reps; j++){
		
		//full lookup for the std::map
		auto start = std::chrono::system_clock::now();
		for (int k =0; k < n; k++){
			mytree_p->find(local_vec[k]);
		}
		auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		double avgdt{ static_cast<double>(elapsed.count())/static_cast<double>(n)};
		t_struct->accum( avgdt );
	}
	t_struct->calculate(reps);
}



int main(int argc, char ** argv) {
	
	if (argc <3) {
		std::cerr << "Usage : "<< argv[0] << " tree_max_size tree_min_size [iterations_per_size]"  << std::endl;
		exit(1);
	}
	
	int max_n{atoi(argv[1])};
	int min_n{atoi(argv[2])};
	int reps {1};
	
	if (argc >3) {
		reps = atoi(argv[3]);
	}
	
	std::string outname{"tree_benchmark_out.txt"};
	std::remove ( "tree_benchmark_out.txt");
		

	std::default_random_engine gen(time(NULL));

	std::vector<int> elem;
	for (int k =0; k < min_n; k++){
		elem.insert(elem.begin(),k);
	}
	
	
	for (int n=min_n; n< max_n; n+=min_n ) {
		
		std::cout << std::endl << "\rCurrent size : " << n << std::flush;
	
		
		for (int k =0; k < n; k++){
			elem.insert(elem.begin(),k);
		}

		
		
		std::map<int, int> stdmap;
		time_avg_stdev map_t;
		
		
		
		bst<int,int> worst_tree{};
		time_avg_stdev worst_tree_t;
		time_avg_stdev worst_tree_balanced_t;
		
		std::pair<const int,int> data_pair;
		
		for (int k =0; k < n; k++){
			auto x = std::pair<const int,int>(elem[k],elem[k]);
			stdmap.insert( x );
			worst_tree.insert( x );
		}
		
		
		do_the_reps( n , reps, &stdmap, &map_t, &elem ) ;
		do_the_reps( n , reps, &worst_tree, &worst_tree_t, &elem ) ;
		
		worst_tree.balance();
		
		do_the_reps( n , reps, &worst_tree, &worst_tree_balanced_t, &elem ) ;
		
		worst_tree.clear();
		
		
		std::vector<int> shuf_elem{elem};
		std::shuffle(shuf_elem.begin(),shuf_elem.end(),gen);
		
		bst<int,int> shuf_tree{};
		time_avg_stdev shuf_tree_t;
		time_avg_stdev shuf_tree_balanced_t;
		
		for (int k =0; k < n; k++){
			auto x = std::pair<const int,int>(shuf_elem[k],shuf_elem[k]);
			shuf_tree.insert( x );
		}
		
		do_the_reps( n , reps, &shuf_tree, &shuf_tree_t, &shuf_elem ) ;
		shuf_tree.balance();
		do_the_reps( n , reps, &shuf_tree, &shuf_tree_balanced_t, &shuf_elem ) ;
		
		

		std::ofstream outfile;
		outfile.open(outname, std::ios_base::app);

		if (outfile.is_open()) {
			outfile << n << "," <<  map_t.avg << "," << map_t.stdev;
			outfile << n << "," <<  worst_tree_t.avg << "," << worst_tree_t.stdev;
			outfile << n << "," <<  worst_tree_balanced_t.avg << "," << worst_tree_balanced_t.stdev;
			outfile << n << "," <<  shuf_tree_t.avg << "," << shuf_tree_t.stdev;
			outfile << n << "," <<  shuf_tree_balanced_t.avg << "," << shuf_tree_balanced_t.stdev << "\n";
			outfile.close();
		}
		
		
		for (int k =n; k < (n+min_n); k++){
			elem.insert(elem.begin(),k);
		}
	
	}
	
	return 0;
	
	
}