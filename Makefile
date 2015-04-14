all: one two tests

LDFLAGS+= -lcurl -ggdb
CPPFLAGS += -Inode_modules/catch/single_include -ggdb

TWO_OBJS = rev.o two.o
TEST_OBJS = rev.o test_main.o

tests: $(TEST_OBJS)
	$(CXX) $(LDFLAGS) -o tests $(TEST_OBJS)

two: $(TWO_OBJS)
	$(CXX) $(LDFLAGS) -o two $(TWO_OBJS)


clean:
	rm -f *.o

.phony: clean
