#include "stack.h"
#include "tests.h"

// private global variable counts the amount of freeing done / objects destroyed safely
static size_t destroy_count = 0;

static void test_destroy_int(void *data){
    destroy_count++;
    free(data);
}

static void test_destroy_bool(void *data){
    destroy_count++;
    free(data);
}

static void test_destroy_string(void *data){
    destroy_count++;
    free(data);
}

// assumes you have sufficient space available when running this test.
static void test_stack_create_functions_when_memory_is_available(void){
    Stack *stack = stack_create();
    if (stack != NULL){
        print_test_result("test_stack_create_functions_when_memory_is_available",true);
    } else{
        print_test_result("test_stack_create_functions_when_memory_is_available",false);
    }
    free(stack);
}

static void test_stack_push_succeeds_when_intended(void){
    Stack *stack = stack_reserve(5);
    int *a = malloc(sizeof(*a));
    int *b = malloc(sizeof(*b));
    int *c = malloc(sizeof(*c));
    *a = 5;*b = 10;*a = 15;
    stack_push(stack,a);
    stack_push(stack,b);
    stack_push(stack,c);

    stack_destroy(stack,test_destroy_int);
    if (destroy_count == 3){
        print_test_result("test_stack_push_succeeds_when_intended",true);
    } else{
        print_test_result("test_stack_push_succeeds_when_intended",false);
    }
    // reset destroy count for future use
    destroy_count = 0;
}
static void test_stack_push_handles_null_stack(void){
    int *a = malloc(sizeof(*a));
    if (stack_push(NULL,a) == false){
        print_test_result("test_stack_push_succeeds_when_intended",true);
    } else{
        print_test_result("test_stack_push_succeeds_when_intended",false);
    }
    // reset destroy count for future use
    destroy_count = 0;
}
static void test_stack_push_handles_invalid_data(void){
    Stack *stack = stack_create();
    if (stack_push(stack,NULL) == false){
        print_test_result("test_stack_push_succeeds_when_intended",true);
    } else{
        print_test_result("test_stack_push_succeeds_when_intended",false);
    }
    // reset destroy count for future use
    destroy_count = 0;
}

static void test_stack_peek_succeeds_when_intended(void){

}
static void test_stack_peek_handles_empty_stack(void);
static void test_stack_peek_does_not_modify_stack(void);

static void test_stack_pop_succeeds_when_intended(void);
static void test_stack_pop_handles_empty_stack(void);

static void test_stack_size(void);
static void test_stack_capacity(void);
static void test_stack_is_empty(void);

static void test_stack_destroy_functions_as_intended(void);
static void test_stack_destroy_handles_null_stack(void);
static void test_stack_destroy_handles_non_freeable_memory(void); // stack pointers (non-heap)

static void test_stack_reserve_functions_intended(void);

static void test_stack_logic_is_valid_checking_invariants1(void);
static void test_stack_logic_is_valid_checking_invariants2(void);
static void test_stack_logic_is_valid_checking_invariants3(void);

static void test_stack_clear_frees_only_internal_data(void); 
static void test_stack_clear_keeps_stack_capacity(void); 
static void test_stack_clear_handles_non_freeable_memory(void); // stack pointers (non-heap)


void stack_main(void){
    printf("\n========================================\n");
    printf("           STARTING STACK TESTS\n");
    printf("========================================\n\n");

    test_stack_create_functions_when_memory_is_available();

    test_stack_push_succeeds_when_intended();
    test_stack_push_handles_null_stack();
    test_stack_push_handles_invalid_data();

    test_stack_peek_succeeds_when_intended();
    test_stack_peek_handles_empty_stack();
    test_stack_peek_does_not_modify_stack();

    test_stack_pop_succeeds_when_intended();
    test_stack_pop_handles_empty_stack();

    test_stack_size();
    test_stack_capacity();
    test_stack_is_empty();

    test_stack_destroy_functions_as_intended();
    test_stack_destroy_handles_null_stack();

    test_stack_reserve_functions_as_intended();

    test_stack_logic_is_valid_checking_invariants1();
    test_stack_logic_is_valid_checking_invariants2();
    test_stack_logic_is_valid_checking_invariants3();

    test_stack_clear_frees_only_internal_data();
    test_stack_clear_keeps_stack_capacity();
    test_stack_clear_handles_non_freeable_memory();

    printf("\n========================================\n");
    printf("            STACK TESTS FINISHED\n");
    printf("========================================\n\n");
}