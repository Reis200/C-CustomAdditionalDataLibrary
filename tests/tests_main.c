#include "tests.h"

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

int main(void){
    stack_main();
    return 0;
}