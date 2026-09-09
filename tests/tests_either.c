#include "either.h"
#include "tests.h"

#include <string.h>

// private global variable counts the amount of freeing done / objects destroyed safely
static size_t destroy_count = 0;

static void test_destroy_int(void *data){
    destroy_count++;
    free(data);
}

static void test_destroy_string(void *data){
    destroy_count++;
    free(data);
}


static void tests_either_create(void){

}


static void tests_either_destroy(void){
    
}


static void tests_either_clear(void){
    
}

static void tests_either_getState(void){
    
}

static void tests_either_getData(void){
    
}

static void tests_either_setData(void){
    
}


void either_main(void){
    printf("\n========================================\n");
    printf("           STARTING EITHER TESTS\n");
    printf("========================================\n\n");

    tests_either_create();

    tests_either_destroy();

    tests_either_clear();

    tests_either_getState();

    tests_either_getData();

    tests_either_setData();

    printf("\n========================================\n");
    printf("            EITHER TESTS FINISHED\n");
    printf("========================================\n\n");
}