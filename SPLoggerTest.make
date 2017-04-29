CC = gcc
OBJS = sp_logger_unit_test.o SPLogger.o
EXEC = sp_logger_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
<<<<<<< HEAD

=======
#sp_logger_unit_test.o: $(TESTS_DIR)/sp_logger_unit_test.c $(TESTS_DIR)/unit_test_util.h SPLogger.h
#	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
>>>>>>> e1dc86b7ac285de165272ff08ab286ab82698369
sp_logger_unit_test.o: $(TESTS_DIR)/sp_logger_unit_test.c $(TESTS_DIR)/unit_test_util.h SPLogger.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPLogger.o: SPLogger.c SPLogger.h 
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
