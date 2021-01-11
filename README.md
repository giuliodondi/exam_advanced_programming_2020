# Advanced Programming Exam - C++ Binary Search Tree

### Source files and compilation

The source files are contained in the folder **src/include/**, they consist in three header files: **bst.h** which implements the main binary tree class, **bst_mode.h** which contains the tree node class and **bst_iterator.h** which implements the iterator used to traverse the tree.
In addition, **src/** contains a program **test.cpp** which tests various features of the binary tree implementation and **benchmark.cpp** used to produce the performance results shown in the report.

Make has three targets (in addition to clean which removes all the binaries):

- **test** , which compiles the testing program **tree_test.x** which is executed without any arguments
- **test_dbg**, which compiles the very same program but sets a flag which, in turn, enables terminal printouts to the iterator that help to understand how the iterator is navigating through the tree
- **benchmark**, which compiles the program **tree_benchmark.x** used to test the performance of the binary tree when loaded with ordered integers (worst-case scenario) and with the same set of integers but re-shuffled at random.\\ The program is run as **./tree_benchmark.x N_MAX N_MIN STEP OUTFILE**, and it will generate trees and std::maps with size incrementin in a loop from N_MIN to N_MAX with step STEP. The program collects 

