#include "queue.h"
#include "tests.h"

#include <stdbool.h>
#include <stddef.h>

// private global variable counts the amount of freeing done / objects destroyed safely
static size_t destroy_count = 0;

static void test_destroy_int(void *data){
    destroy_count++;
    free(data);
}


/* ============================================================
   Construction
   ============================================================ */

void test_queue_create_functions_when_memory_is_available(void){
    Queue *queue = queue_create();
    bool isNonNull = (queue != NULL);
    queue_destroy(queue,NULL);
    print_test_result("test_queue_create_functions_when_memory_is_available",isNonNull);
}

void test_queue_reserve_functions_intended(void){
    size_t capacity = 5;
    Queue *queue = queue_reserve(capacity);
    bool isNonNull = (queue != NULL);
    bool isCapacitySet = (queue_capacity(queue) == capacity);
    queue_destroy(queue,NULL);
    print_test_result("test_queue_reserve_functions_intended",isNonNull && isCapacitySet);
}

/* ============================================================
   Insertion
   ============================================================ */

void test_queue_enqueue_works_as_intended(void){
    size_t capacity = 5; size_t size = 5;
    Queue *queue = queue_reserve(capacity);
    int *tempPtr = &(int){5};
    bool enqueued_right = true;
    for (size_t n = 0; n < size; n++){
        enqueued_right = queue_enqueue(queue,tempPtr);
    }
    bool isNonNull = (queue != NULL);
    bool isCapacitySet = (queue_capacity(queue) == capacity);
    bool isSizeVerified = (queue_size(queue) == size);
    queue_destroy(queue,NULL);
    print_test_result("test_queue_enqueue_works_as_intended",isNonNull && isCapacitySet && isSizeVerified && enqueued_right);
}

void test_queue_enqueue_handles_overflows(void){
    size_t capacity = 5; size_t size = 10;
    Queue *queue = queue_reserve(capacity);
    bool enqueued_right = true;
    for (size_t n = 0; n < size; n++){
        int *tempPtr = malloc(sizeof(*tempPtr));
        *tempPtr = n;
        enqueued_right = queue_enqueue(queue,tempPtr);
    }
    bool isNonNull = (queue != NULL);
    bool isCapacitySet = (queue_capacity(queue) == capacity * 2);
    bool isSizeVerified = (queue_size(queue) == size);
    queue_destroy(queue,test_destroy_int);
    print_test_result("test_queue_enqueue_handles_overflows",isNonNull && isCapacitySet && isSizeVerified && enqueued_right);
}

void test_queue_enqueue_handles_null_queue(void){
    int *temp = &(int){5};
    (queue_enqueue(NULL,temp)) ? print_test_result("test_queue_enqueue_handles_null_queue",false) : print_test_result("test_queue_enqueue_handles_null_queue",true); 
}

void test_queue_enqueue_handles_invalid_data(void){
    (queue_enqueue(NULL,NULL)) ? print_test_result("test_queue_enqueue_handles_invalid_data",false) : print_test_result("test_queue_enqueue_handles_null_queue",true);
}

/* ============================================================
   Access
   ============================================================ */

void test_queue_peek_succeeds_when_intended(void){
    print_test_result("test_queue_peek_succeeds_when_intended",false);
}
void test_queue_peek_handles_empty_queue(void){
    print_test_result("test_queue_peek_handles_empty_queue",false);
}
void test_queue_peek_does_not_modify_queue(void){
    print_test_result("test_queue_peek_does_not_modify_queue",false);
}


/* ============================================================
   Removal
   ============================================================ */

void test_queue_dequeue_succeeds_when_intended(void){
    print_test_result("test_queue_dequeue_succeeds_when_intended",false);
}
void test_queue_dequeue_handles_empty_queue(void){
    print_test_result("test_queue_dequeue_handles_empty_stack",false);
}


/* ============================================================
   Observation
   ============================================================ */

void test_queue_size(void){
    size_t capacity = 5; size_t size = 10;
    Queue *queue = queue_reserve(capacity);
    bool isSizeVerified0 = (queue_size(queue) == 0);
    for (size_t n = 0; n < size; n++){
        int *tempPtr = malloc(sizeof(*tempPtr));
        *tempPtr = n;
        queue_enqueue(queue,tempPtr);
    }
    bool isSizeVerified1 = (queue_size(queue) == size);
    queue_destroy(queue,test_destroy_int);
    print_test_result("test_queue_size",isSizeVerified0 & isSizeVerified1);
}
void test_queue_capacity(void){
    Queue *queue = queue_create();
    bool isCapacityVerified0 = (queue_capacity(queue) == 0);
    queue_destroy(queue,NULL);
    Queue *queue1 = queue_reserve(10);
    bool isCapacityVerified1 = (queue_capacity(queue1) == 10);
    queue_destroy(queue1,NULL);
    print_test_result("test_queue_capacity",isCapacityVerified0 & isCapacityVerified1);
}

void test_queue_is_empty(void){
    print_test_result("test_queue_is_empty",queue_is_empty(queue_create()) && queue_is_empty(NULL));
}


/* ============================================================
   Lifetime Management
   ============================================================ */

void test_queue_destroy_functions_as_intended(void){
    print_test_result("test_queue_destroy_functions_as_intended",false);
}
void test_queue_destroy_handles_null_stack(void){
    print_test_result("test_queue_destroy_handles_null_stack",false);
}
void test_queue_destroy_handles_non_freeable_memory(void){
    print_test_result("test_queue_destroy_handles_non_freeable_memory",false);
} 

void test_queue_clear_frees_only_internal_data(void){
    print_test_result("test_queue_clear_frees_only_internal_data",false);
}
void test_queue_clear_keeps_queue_capacity(void){
    print_test_result("test_queue_clear_keeps_queue_capacity",false);
}
void test_queue_clear_handles_non_freeable_memory(void){
    print_test_result("test_queue_clear_handles_non_freeable_memory",false);
}


/* ============================================================
   Debugging
   ============================================================ */
void test_queue_print(void){
    print_test_result("test_queue_print",false);
}


void queue_main(void){
    printf("\n========================================\n");
    printf("           STARTING QUEUE TESTS\n");
    printf("========================================\n\n");

    test_queue_create_functions_when_memory_is_available();
    test_queue_reserve_functions_intended();

    test_queue_enqueue_works_as_intended();
    test_queue_enqueue_handles_overflows();
    test_queue_enqueue_handles_null_queue();
    test_queue_enqueue_handles_invalid_data();

    test_queue_peek_succeeds_when_intended();
    test_queue_peek_handles_empty_queue();
    test_queue_peek_does_not_modify_queue();

    test_queue_dequeue_succeeds_when_intended();
    test_queue_dequeue_handles_empty_queue();

    test_queue_size();
    test_queue_capacity();
    test_queue_is_empty();


    test_queue_destroy_functions_as_intended();
    test_queue_destroy_handles_null_stack();
    test_queue_destroy_handles_non_freeable_memory(); // stack pointers (non-heap)

    test_queue_clear_frees_only_internal_data();
    test_queue_clear_keeps_queue_capacity();
    test_queue_clear_handles_non_freeable_memory(); // stack pointers (non-heap)

    test_queue_print();

    printf("\n========================================\n");
    printf("            QUEUE TESTS FINISHED\n");
    printf("========================================\n\n");
}