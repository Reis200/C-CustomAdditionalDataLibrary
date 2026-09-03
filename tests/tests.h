#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

void print_test_result(const char *testName, bool passed);

void binarysearchtree_main(void);
void dynamicarray_main(void);
void hashmap_main(void);
void hashset_main(void);
void linkedlist_main(void);
void multiset_main(void);
void queue_main(void);
void stack_main(void);

#endif