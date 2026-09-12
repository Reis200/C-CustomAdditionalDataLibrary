#include "either.h"
#include "tests.h"

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
    int data = 32;
    Either *either = either_create(RIGHT, &data);

    print_test_result("tests_either_create #1", either != NULL && either->eitherState == RIGHT && either->data.right == &data && *(int *)either->data.right == data);
    either_destroy(either, NULL); // as int *data made in stack

    char *data1 = "test :D";
    Either *either1 = either_create(NONE, data1);
    print_test_result("tests_either_create #2", either1 != NULL && either1->eitherState == LEFT && either1->data.left == data1 && strcmp(either1->data.left, data1) == 0);
    either_destroy(either1, NULL); // as char *data1 is non-freeable

    Either *either2 = either_create(NONE, NULL);
    print_test_result("tests_either_create #4", either2 != NULL && either2->eitherState == NONE && either2->data.left == NULL);
    either_destroy(either2, NULL);

    char *data2 = "test :D";
    Either *either3 = either_create(EITHER_INVALID, data2);
    print_test_result("tests_either_create #3", either3 != NULL && either3->eitherState == LEFT && either3->data.left == data2 && strcmp(either3->data.left, data2) == 0);
    either_destroy(either3, NULL); // as char *data2 is is non-freeable

    // reset destroy count for future use
    destroy_count = 0;
}


static void tests_either_destroy(void){
    Either *either = either_create(NONE, NULL);

    either_destroy(either, test_destroy_int);
    print_test_result("tests_either_destroy #1", destroy_count == 0);

    char *str1 = malloc(sizeof(char) * 5);
    strcpy(str1, "test");
    Either *either1 = either_create(RIGHT, str1);
    either_destroy(either1, test_destroy_string);
    print_test_result("tests_either_destroy #2", destroy_count == 1);
    
    // reset destroy count for future use
    destroy_count = 0;
}


static void tests_either_clear(void){
    Either *either = either_create(NONE, NULL);

    either_clear(either, test_destroy_int);
    print_test_result("tests_either_clear #1", destroy_count == 0 && either != NULL && either->data.left == NULL && either->eitherState == NONE);

    char *str1 = malloc(sizeof(char) * 5);
    strcpy(str1, "test");
    Either *either1 = either_create(RIGHT,str1);
    either_clear(either1, test_destroy_string);
    print_test_result("tests_either_clear #2", destroy_count == 1 && either1 != NULL && either1->data.right == NULL && either1->eitherState == NONE);
    

    either_destroy(either, NULL);
    either_destroy(either1, NULL);
    // reset destroy count for future use
    destroy_count = 0;
}

static void tests_either_getState(void){
    char *str1 = malloc(sizeof(char) * 5);
    strcpy(str1, "test");
    Either *either = either_create(RIGHT, str1); 

    EitherState eitherState = either_getState(either);

    print_test_result("tests_either_getState #1", eitherState == RIGHT);


    either_setData(either, NONE, NULL, test_destroy_string);

    eitherState = either_getState(either);

    print_test_result("tests_either_getState #2", eitherState == NONE);
    either_destroy(either, NULL);


    Either *either1 = either_create(NONE, NULL); 

    EitherState eitherState1 = either_getState(either1);

    print_test_result("tests_either_getState #3", eitherState1 == NONE);
    either_destroy(either1, NULL);


    Either *either2 = NULL;

    EitherState eitherState2 = either_getState(either2);

    print_test_result("tests_either_getState #4", eitherState2 == EITHER_INVALID);

    // reset destroy count for future use
    destroy_count = 0;
}

static void tests_either_getData(void){
    Either *either = either_create(NONE, NULL);
    char *str1 = malloc(sizeof(char) * 5);
    strcpy(str1, "test");
    either_setData(either, RIGHT, str1, NULL);

    const char *eitherData = either_getData(either);

    print_test_result("tests_either_getData #1", eitherData != NULL && strcmp(eitherData, str1) == 0);
    either_destroy(either, test_destroy_string);


    Either *eitherPlus = either_create(NONE, NULL);
    char *str2 = malloc(sizeof(char) * 5);
    strcpy(str2, "test");
    either_setData(eitherPlus, LEFT, str2, NULL);

    const char *eitherDataPlus = either_getData(eitherPlus);

    print_test_result("tests_either_getData #2", eitherDataPlus != NULL && strcmp(eitherDataPlus, str2) == 0);
    either_destroy(eitherPlus, test_destroy_string);


    Either *either1 = either_create(NONE, NULL);

    const char *eitherData1 = either_getData(either1);

    print_test_result("tests_either_getData #3", eitherData1 == NULL);
    either_destroy(either1, NULL);


    Either *either2 = NULL;

    const char *eitherData2 = either_getData(either2);

    print_test_result("tests_either_getData #4", eitherData2 == NULL);

    // reset destroy count for future use
    destroy_count = 0;
}

static void tests_either_setData(void){
    Either *either = either_create(NONE, NULL);
    int *n = malloc(sizeof(int));
    *n = 5;
    bool result = either_setData(either, RIGHT, n, NULL);
    print_test_result("tests_either_setData #1", result && either->eitherState == RIGHT && either->data.right == n && *(int *)either->data.right == *n);
    

    int *n1 = malloc(sizeof(int));
    *n1 = 500;
    bool result1 = either_setData(either, LEFT, n1, test_destroy_int);
    print_test_result("tests_either_setData #2", destroy_count == 1 && result1 && either->eitherState == LEFT && either->data.left == n1 && *(int *)either->data.left == *n1);

    either_destroy(either, test_destroy_int);


    Either *either1 = either_create(NONE, NULL);

    bool result2 = either_setData(either1, NONE, NULL, NULL);

    print_test_result("tests_either_setData #3", result2 && either1->eitherState == NONE && either1->data.left == NULL);
    either_destroy(either1, NULL);


    Either *either2 = NULL;

    bool result3 = either_setData(either2, NONE, NULL, NULL);

    print_test_result("tests_either_setData #4", result3 == false);

    // reset destroy count for future use
    destroy_count = 0;
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