# Advanced Programming Exam - C++ Binary Search Tree

### Source files and compilation

The source files are contained in the folder **src/include/**, they consist in three header files: **bst.h** which implements the main binary tree class, **bst_mode.h** which contains the tree node class and **bst_iterator.h** which implements the iterator used to traverse the tree.
In addition, **src/** contains a program **test.cpp** which tests various features of the binary tree implementation and **benchmark.cpp** used to produce the performance results shown in the report.

Make has three targets (in addition to clean which removes all the binaries):

- **test** , which compiles the testing program **tree_test.x** which is executed without any arguments.
- **test_dbg**, which compiles the very same program but sets a flag which, in turn, enables terminal printouts to the iterator that help to understand how the iterator is navigating through the tree.
- **benchmark**, which compiles the program **tree_benchmark.x** used to test the performance of the binary tree against std::map.

The benchmark is executed as **./tree_benchmark.x N_MAX N_MIN STEP OUTFILE**, and it will generate trees and std::maps with size incrementing in a loop from N_MIN to N_MAX with step STEP. For each size, the time of a full lookup of all the elements as well as the number of comparisons done in the process is gathered for every test. The test results are written in the file OUTFILE, if a full path is specified the directories will not automatically be created. 
The tests are: std::map loaded with ordered integers, binary tree loaded with ordered integers, balancing of the tree, balanced binary tree loaded with ordered integer, std::map loaded with shuffled integers, binary tree loaded with shuffled integers, balancing of the shuffled tree, balanced binary tree loaded with shuffled integers.

The output file is formatted as follows (worst indicates the ordered case and shuf indicates the shuffled case):

|Size|worst_map_time|worst_map_count|worst_tree_time|worst_tree_count|worst_balancing_t|worst_tree_balanced_time|worst_tree_balanced_count|shuf_map_time|shuf_map_count|shuf_tree_time|shuf_tree_count|shuf_balancing_t|shuf_tree_balanced_time|shuf_tree_balanced_count

