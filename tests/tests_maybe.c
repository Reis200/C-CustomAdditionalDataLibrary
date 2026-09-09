#include "maybe.h"
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


static void tests_maybe_create(void){
    Maybe *maybe = maybe_create();

    print_test_result("tests_maybe_create", maybe != NULL && maybe->maybeState == NOTHING && maybe->data == NULL);

    maybe_destroy(maybe, NULL);
    // reset destroy count for future use
    destroy_count = 0;
}


static void tests_maybe_destroy(void){
    Maybe *maybe = maybe_create();

    maybe_destroy(maybe, test_destroy_int);
    print_test_result("tests_maybe_destroy #1", destroy_count == 0);

    Maybe *maybe1 = maybe_create();
    char *str1 = malloc(sizeof(char) * 5);
    strcpy(str1, "test");
    maybe_setData(maybe1, str1, NULL);
    maybe_destroy(maybe1, test_destroy_string);
    print_test_result("tests_maybe_destroy #2", destroy_count == 1);
    
    // reset destroy count for future use
    destroy_count = 0;
}


static void tests_maybe_clear(void){
    Maybe *maybe = maybe_create();

    maybe_clear(maybe, test_destroy_int);
    print_test_result("tests_maybe_clear #1", destroy_count == 0 && maybe != NULL && maybe->data == NULL && maybe->maybeState == NOTHING);

    Maybe *maybe1 = maybe_create();
    char *str1 = malloc(sizeof(char) * 5);
    strcpy(str1, "test");
    maybe_setData(maybe1, str1, NULL);
    maybe_clear(maybe1, test_destroy_string);
    print_test_result("tests_maybe_clear #2", destroy_count == 1 && maybe1 != NULL && maybe1->data == NULL && maybe1->maybeState == NOTHING);
    

    maybe_destroy(maybe, NULL);
    maybe_destroy(maybe1, NULL);
    // reset destroy count for future use
    destroy_count = 0;
}

static void tests_maybe_getState(void){
    Maybe *maybe = maybe_create();
    char *str1 = malloc(sizeof(char) * 5);
    strcpy(str1, "test");
    maybe_setData(maybe, str1, NULL);

    MaybeState maybeState = maybe_getState(maybe);

    print_test_result("tests_maybe_getState #1", maybeState == JUST);
    maybe_destroy(maybe, test_destroy_string);


    Maybe *maybe1 = maybe_create();

    MaybeState maybeState1 = maybe_getState(maybe1);

    print_test_result("tests_maybe_getState #2", maybeState1 == NOTHING);
    maybe_destroy(maybe1, NULL);


    Maybe *maybe2 = NULL;

    MaybeState maybeState2 = maybe_getState(maybe2);

    print_test_result("tests_maybe_getState #3", maybeState2 == MAYBE_INVALID);

    // reset destroy count for future use
    destroy_count = 0;
}

static void tests_maybe_getData(void){
    Maybe *maybe = maybe_create();
    char *str1 = malloc(sizeof(char) * 5);
    strcpy(str1, "test");
    maybe_setData(maybe, str1, NULL);

    const char *maybeData = maybe_getData(maybe);

    print_test_result("tests_maybe_getData #1", maybeData != NULL && strcmp(maybeData, str1) == 0);
    maybe_destroy(maybe, test_destroy_string);


    Maybe *maybe1 = maybe_create();

    const char *maybeData1 = maybe_getData(maybe1);

    print_test_result("tests_maybe_getData #2", maybeData1 == NULL);
    maybe_destroy(maybe1, NULL);


    Maybe *maybe2 = NULL;

    const char *maybeData2 = maybe_getData(maybe2);

    print_test_result("tests_maybe_getData #3", maybeData2 == NULL);

    // reset destroy count for future use
    destroy_count = 0;
}

static void tests_maybe_setData(void){
    Maybe *maybe = maybe_create();
    int *n = malloc(sizeof(int));
    *n = 5;
    bool result = maybe_setData(maybe, n, NULL);
    print_test_result("tests_maybe_setData #1", result);
    

    int *n1 = malloc(sizeof(int));
    *n1 = 500;
    bool result1 = maybe_setData(maybe, n1, test_destroy_int);
    print_test_result("tests_maybe_setData #2", destroy_count == 1 && result1);

    maybe_destroy(maybe, test_destroy_int);


    Maybe *maybe1 = maybe_create();

    bool result2 = maybe_setData(maybe1, NULL, NULL);

    print_test_result("tests_maybe_setData #3", result2 && maybe1->maybeState == NOTHING);
    maybe_destroy(maybe1, NULL);


    Maybe *maybe2 = NULL;

    bool result3 = maybe_setData(maybe2, NULL, NULL);

    print_test_result("tests_maybe_setData #4", result3 == false);

    // reset destroy count for future use
    destroy_count = 0;
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

    tests_maybe_setData();

    printf("\n========================================\n");
    printf("            MAYBE TESTS FINISHED\n");
    printf("========================================\n\n");
}