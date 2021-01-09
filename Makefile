COMPILER = g++
EXE_NAME = test.x
FLAGS = -Wall -Wextra 
INCLUDE = -I src/include
test_nodbg: CFLAGS=
test: CFLAGS='-DTREE_DBG'

OBJ = src/test.o

default: test_nodbg

test:  $(OBJ)
	$(COMPILER) $^ -o $(EXE_NAME) $(FLAGS) -lm 

test_nodbg:  $(OBJ)
	$(COMPILER) $^ -o $(EXE_NAME) $(FLAGS) -lm 
	
clean:
	rm -rf $(OBJ) $(EXE_NAME)
	
%.o: %.cpp
	$(COMPILER) -c $< -o $@ $(FLAGS) $(CFLAGS) $(INCLUDE)