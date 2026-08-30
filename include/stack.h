#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdbool.h>

/*
Representation Invariants:
    0 <= size <= capacity whenever storage exists.
    the live stack values occupy the contiguous prefix of slots from 0 to (size-1)
    If size is non-zero, the top is the live value at position (size-1).
    Slots at positions size through size through (capacity-1) are unused storage and are not part of the abstract stack.
    The logical order from bottom to top is the same as the physical live-array order.
*/
typedef struct {
    void **items; // items pointer to array of void pointers
    size_t capacity; // number of available pointer slots in the allocation
    size_t size; // number of live elements in the stack / also identifies top position and provides O(1) size/empty queries.
} Stack;

// produce a valid empty stack
Stack* stack_create(void);

// true  = inserted
// false = duplicate / failure
// stack stores the pointer while the value remains present.
bool stack_push(Stack *stack,void *data);

const void* stack_peek(const Stack *stack);

// responsibility for the removed value returns to the caller.
void* stack_pop(Stack *stack);

// number of live stack data present
size_t stack_size(const Stack *stack);

bool stack_is_empty(const Stack *stack);

// it destroys the pointers present within the stack too, so caller just needs to pass in how to destroy internal data, rest is handled caller does not need to free any more memory.
void stack_destroy(Stack *stack, void (*destroyData)(void *data));

// remove all elements while retaining capacity, with a clearly defined destruction policy. 
// similar to destroy but pointer and memory allocated for stack and its capacity still remains. 
// (As elements removed only size changes) + elements are freed as well so no responsibility to caller.
Stack* stack_clear(Stack *stack, void (*destroyData)(void *data)); // modifies the original pointer and returns the new stack pointer with cleared stack

// preallocate enough capacity for a known workload.
Stack* stack_reserve(size_t reservedSize);

// expose current storage capacity for diagnostics or benchmarking.
size_t stack_capacity(const Stack *stack);

// visit from bottom-to-top or top-to-bottom; the chosen order must be documented.
void stack_foreach(const Stack *stack);

// only if a clone callback and rollback behaviour on partial failure are defined.
Stack* stack_clone(const Stack *stack);

#endif