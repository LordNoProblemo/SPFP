CC = gcc
OBJS = sp_KD_tree_and_array_test.o KDTree.o
EXEC = sp_KD_tree_and_array_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
#sp_KD_tree_and_array_test.o: $(TESTS_DIR)/sp_KD_tree_and_array_test.c $(TESTS_DIR)/unit_test_util.h KDTree.h
#	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
sp_KD_tree_and_array_test.o: $(TESTS_DIR)/sp_KD_tree_and_array_test.c $(TESTS_DIR)/unit_test_util.h KDTree.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
KDTree.o: KDTree.c KDTree.h 
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
