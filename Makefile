COMPILER = g++
TEST_EXE = tree_test.x
BENCH_EXE = tree_benchmark.x
FLAGS = -std=c++14 -Wall -Wextra 

INCLUDE = -I src/include
test: CFLAGS=
test_dbg: CFLAGS='-DTREE_DBG'
benchmark: FLAGS+=-march=native -O3


TEST_OBJ = src/test.o
BENCH_OBJ = src/benchmark.o

default: test

test_dbg:  $(TEST_OBJ)
	$(COMPILER) $^ -o $(TEST_EXE) $(FLAGS) -lm 

test:  $(TEST_OBJ)
	$(COMPILER) $^ -o $(TEST_EXE) $(FLAGS) -lm 
	
benchmark: $(BENCH_OBJ)
	$(COMPILER) $^ -o $(BENCH_EXE) $(FLAGS) -lm 
	
clean:
	rm -rf $(TEST_OBJ) $(TEST_EXE)
	rm -rf $(BENCH_OBJ) $(BENCH_EXE)
	
%.o: %.cpp
	$(COMPILER) -c $< -o $@ $(FLAGS) $(CFLAGS) $(INCLUDE)
	
