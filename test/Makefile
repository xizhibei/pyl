CC = gcc
XX = g++
CXXFLAGS = -Wall -O
OBJS = test.o sqlite.o

all: $(OBJS)
	$(XX) $(OBJS) -o main_test  -lsqlite3
	$(XX) dir_test.cpp -o dir_test
	$(XX) sqlite_test.cpp -o sqlite_test

sqlite.o:sqlite.h
	$(XX) -c sqlite.cpp -o sqlite.o -lsqlite3

test.o:sqlite.h
	$(XX) -c sqlite_class_test.cpp -o test.o

.PHONY:clean
clean:
	-rm -rf *.o *~ main *_test

