#ifndef TEST_H
#define TEST_H

#include <stdbool.h>

void print_test_result(const char *testName, bool passed){
    switch (passed){
        case true:
            printf("[PASS] %s\n", testName);
            break;
        case false:
            printf("[FAIL] %s\n", testName);
            break;
    }
}

void binarysearchtree_main(void);
void dynamicarray_main(void);
void hashmap_main(void);
void hashset_main(void);
void linkedlist_main(void);
void multiset_main(void);
void queue_main(void);
void stack_main(void);

#endif