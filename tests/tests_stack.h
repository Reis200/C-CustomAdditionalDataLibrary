#ifndef TEST_STACK_H
#define TEST_STACK_H


void test_stack_create_functions_when_memory_is_available(void);

void test_stack_push_succeeds_when_intended(void);
void test_stack_push_handles_null_stack(void);
void test_stack_push_handles_invalid_data(void);

void test_stack_peek_succeeds_when_intended(void);
void test_stack_peek_handles_empty_stack(void);
void test_stack_peek_does_not_modify_stack(void);

void test_stack_pop_succeeds_when_intended(void);
void test_stack_pop_handles_empty_stack(void);

void test_stack_size(void);
void test_stack_capacity(void);
void test_stack_is_empty(void);

void test_stack_destroy_functions_as_intended(void);
void test_stack_destroy_handles_null_stack(void);

void test_stack_reserve_functions_intended(void);

void test_stack_logic_is_valid_checking_invariants1(void);
void test_stack_logic_is_valid_checking_invariants2(void);
void test_stack_logic_is_valid_checking_invariants3(void);

void test_stack_clear_frees_only_internal_data(void); 
void test_stack_clear_keeps_stack_capacity(void); 
void test_stack_clear_handles_non_freeable_memory(void); // stack pointers (non-heap)

#endif