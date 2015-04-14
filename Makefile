all: one two tests

LDFLAGS+= -lcurl 
CPPFLAGS += -Inode_modules/catch/single_include

tests: rev.o test_main.o
