CC = gcc
OBJS = sp_KD_tree_and_array_test.o KDTree.o SPPoint.o SPConfig.o SPBPriorityQueue.o SPLogger.o
EXEC = sp_KD_tree_and_array_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -g

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
sp_KD_tree_and_array_test.o: $(TESTS_DIR)/sp_KD_tree_and_array_test.c KDTree.h SPBPriorityQueue.h SPPoint.h SPConfig.h SPLogger.h KNN.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
	
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
	
SPPoint.o: SPPoint.c SPPoint.h 
	$(CC) $(C_COMP_FLAG) -c $*.c
	
KDTree.o: KDTree.c SPLogger.h KNN.h SPBPriorityQueue.h SPPoint.h SPConfig.h KDTree.h
	$(CC) $(C_COMP_FLAG) -c $*.c
	
SPConfig.o: SPConfig.c SPConfig.h
	$(CC) $(C_COMP_FLAG) -c $*.c
	
SPBPriorityQueue.o: SPBPriorityQueue.c SPBPriorityQueue.h
	$(CC) $(C_COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
