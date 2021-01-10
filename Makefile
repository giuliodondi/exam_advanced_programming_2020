COMPILER = g++
TEST_EXE = tree_test.x
BENCH_EXE = tree_benchmark.x
FLAGS = -std=c++14 -Wall -Wextra 

INCLUDE = -I src/include
test_nodbg: CFLAGS=
test: CFLAGS='-DTREE_DBG'


TEST_OBJ = src/test.o
BENCH_OBJ = src/benchmark.o

default: test_nodbg

test:  $(TEST_OBJ)
	$(COMPILER) $^ -o $(TEST_EXE) $(FLAGS) -lm 

test_nodbg:  $(TEST_OBJ)
	$(COMPILER) $^ -o $(TEST_EXE) $(FLAGS) -lm 
	
benchmark: $(BENCH_OBJ)
	$(COMPILER) $^ -o $(BENCH_EXE) $(FLAGS) -lm 
	
clean:
	rm -rf $(TEST_OBJ) $(TEST_EXE)
	rm -rf $(BENCH_OBJ) $(BENCH_EXE)
	
%.o: %.cpp
	$(COMPILER) -c $< -o $@ $(FLAGS) $(CFLAGS) $(INCLUDE)