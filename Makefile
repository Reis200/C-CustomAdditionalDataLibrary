CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic -Iinclude -Itests
TARGET = tests/tests.exe
OBJECTS = build/hashmap.o \
		  build/tests_hashmap.o \
		  build/tests_main.o \
		  build/binarysearchtree.o \
		  build/tests_binarysearchtree.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

build/hashmap.o: src/hashmap.c include/hashmap.h | build
	$(CC) $(CFLAGS) -c src/hashmap.c -o build/hashmap.o

build/tests_hashmap.o: tests/tests_hashmap.c tests/tests.h include/hashmap.h | build
	$(CC) $(CFLAGS) -c tests/tests_hashmap.c -o build/tests_hashmap.o

build/binarysearchtree.o: src/binarysearchtree.c include/binarysearchtree.h | build
	$(CC) $(CFLAGS) -c src/binarysearchtree.c -o build/binarysearchtree.o

build/tests_binarysearchtree.o: tests/tests_binarysearchtree.c tests/tests.h include/binarysearchtree.h | build
	$(CC) $(CFLAGS) -c tests/tests_binarysearchtree.c -o build/tests_binarysearchtree.o 

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


.PHONY: all test clean