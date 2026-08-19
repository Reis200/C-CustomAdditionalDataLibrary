CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic -Iinclude -Itests
TARGET = tests/tests.exe
OBJECTS = build/hashmap.o \
		  build/tests_hashmap.o \
		  build/binarysearchtree.o \
		  build/tests_binarysearchtree.o \
		  build/binarysearchtreemap.o \
		  build/tests_binarysearchtreemap.o \
		  build/dynamicarray.o \
		  build/tests_dynamicarray.o \
		  build/hashset.o \
		  build/tests_hashset.o \
		  build/linkedlist.o \
		  build/tests_linkedlist.o \
		  build/multiset.o \
		  build/tests_multiset.o \
		  build/queue.o \
		  build/tests_queue.o \
		  build/stack.o \
		  build/tests_stack.o \
		  build/treeset.o \
		  build/tests_treeset.o \
		  build/tests_main.o 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

build/binarysearchtree.o: src/binarysearchtree.c include/binarysearchtree.h | build
	$(CC) $(CFLAGS) -c src/binarysearchtree.c -o build/binarysearchtree.o

build/tests_binarysearchtree.o: tests/tests_binarysearchtree.c tests/tests.h include/binarysearchtree.h | build
	$(CC) $(CFLAGS) -c tests/tests_binarysearchtree.c -o build/tests_binarysearchtree.o 

build/binarysearchtreemap.o: src/binarysearchtreemap.c include/binarysearchtreemap.h | build
	$(CC) $(CFLAGS) -c src/binarysearchtreemap.c -o build/binarysearchtreemap.o

build/tests_binarysearchtreemap.o: tests/tests_binarysearchtreemap.c tests/tests.h include/binarysearchtreemap.h | build
	$(CC) $(CFLAGS) -c tests/tests_binarysearchtreemap.c -o build/tests_binarysearchtreemap.o 

build/dynamicarray.o: src/dynamicarray.c include/dynamicarray.h | build
	$(CC) $(CFLAGS) -c src/dynamicarray.c -o build/dynamicarray.o

build/tests_dynamicarray.o: tests/tests_dynamicarray.c tests/tests.h include/dynamicarray.h | build
	$(CC) $(CFLAGS) -c tests/tests_dynamicarray.c -o build/tests_dynamicarray.o

build/hashmap.o: src/hashmap.c include/hashmap.h | build
	$(CC) $(CFLAGS) -c src/hashmap.c -o build/hashmap.o

build/tests_hashmap.o: tests/tests_hashmap.c tests/tests.h include/hashmap.h | build
	$(CC) $(CFLAGS) -c tests/tests_hashmap.c -o build/tests_hashmap.o

build/hashset.o: src/hashset.c include/hashset.h | build
	$(CC) $(CFLAGS) -c src/hashset.c -o build/hashset.o

build/tests_hashset.o: tests/tests_hashset.c tests/tests.h include/hashset.h | build
	$(CC) $(CFLAGS) -c tests/tests_hashset.c -o build/tests_hashset.o

build/linkedlist.o: src/linkedlist.c include/linkedlist.h | build
	$(CC) $(CFLAGS) -c src/linkedlist.c -o build/linkedlist.o

build/tests_linkedlist.o: tests/tests_linkedlist.c tests/tests.h include/linkedlist.h | build
	$(CC) $(CFLAGS) -c tests/tests_linkedlist.c -o build/tests_linkedlist.o

build/multiset.o: src/multiset.c include/multiset.h | build
	$(CC) $(CFLAGS) -c src/multiset.c -o build/multiset.o

build/tests_multiset.o: tests/tests_multiset.c tests/tests.h include/multiset.h | build
	$(CC) $(CFLAGS) -c tests/tests_multiset.c -o build/tests_multiset.o

build/queue.o: src/queue.c include/queue.h | build
	$(CC) $(CFLAGS) -c src/queue.c -o build/queue.o

build/tests_queue.o: tests/tests_queue.c tests/tests.h include/queue.h | build
	$(CC) $(CFLAGS) -c tests/tests_queue.c -o build/tests_queue.o

build/stack.o: src/stack.c include/stack.h | build
	$(CC) $(CFLAGS) -c src/stack.c -o build/stack.o

build/tests_stack.o: tests/tests_stack.c tests/tests.h include/stack.h | build
	$(CC) $(CFLAGS) -c tests/tests_stack.c -o build/tests_stack.o

build/treeset.o: src/treeset.c include/treeset.h | build
	$(CC) $(CFLAGS) -c src/treeset.c -o build/treeset.o

build/tests_treeset.o: tests/tests_treeset.c tests/tests.h include/treeset.h | build
	$(CC) $(CFLAGS) -c tests/tests_treeset.c -o build/tests_treeset.o

build/tests_main.o: tests/tests_main.c tests/tests.h | build
	$(CC) $(CFLAGS) -c tests/tests_main.c -o build/tests_main.o

build:
	if not exist build mkdir build

compile: $(TARGET)

run: $(TARGET)	
	tests\tests.exe

clean:
	if exist build rmdir /S /Q build
	if exist tests\tests.exe del /Q tests\tests.exe


.PHONY: all compile run clean