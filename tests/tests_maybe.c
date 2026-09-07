#include "maybe.h"
#include "tests.h"

// private global variable counts the amount of freeing done / objects destroyed safely
static size_t destroy_count = 0;

static void test_destroy_int(void *data){
    destroy_count++;
    free(data);
}

// static void test_destroy_bool(void *data){
//     destroy_count++;
//     free(data);
// }

// static void test_destroy_string(void *data){
//     destroy_count++;
//     free(data);
// }


static void tests_maybe_create(void){
    
}


static void tests_maybe_destroy(void){
    
}


static void tests_maybe_clear(void){
    
}

static void tests_maybe_getState(void){
    
}

static void tests_maybe_getData(void){
    
}

static void tests_maybe_modifyData(void){
    
}


void maybe_main(void){
    printf("\n========================================\n");
    printf("           STARTING MAYBE TESTS\n");
    printf("========================================\n\n");

    tests_maybe_create();

    tests_maybe_destroy();

    tests_maybe_clear();

    tests_maybe_getState();

    tests_maybe_getData();

    tests_maybe_modifyData();

    printf("\n========================================\n");
    printf("            STACK MAYBE FINISHED\n");
    printf("========================================\n\n");
}