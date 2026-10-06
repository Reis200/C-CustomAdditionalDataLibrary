#include "stack.h"

#include <stdlib.h>
#include <stdio.h>


/* ============================================================
   Construction
   ============================================================ */

// Create a valid empty stack.
//
// Initial state:
//     items    = NULL
//     capacity = 0
//     size     = 0
//
// The Stack structure itself is allocated on the heap.
Stack *stack_create(void){
    Stack *stack = malloc(sizeof(*stack));

    if (stack == NULL){
        return NULL;
    }

    stack->size = 0;
    stack->capacity = 0;
    stack->items = NULL;

    return stack;
}


// Create an empty stack with capacity reserved for at least
// reservedSize elements.
//
// The returned stack still has size == 0.
//
// For reservedSize == 0 empty stack returned
// cases reserving will overflow as SIZE_MAX is reached, NULL is returned.
//
// Returns NULL if allocation fails.
Stack *stack_reserve(size_t reservedSize){
    Stack *stack = stack_create();

    if (stack == NULL){
        return NULL;
    }

    if (reservedSize == 0){
        return stack;
    }

    if (reservedSize > SIZE_MAX / sizeof(*stack->items)){
        free(stack);
        return NULL;
    }

    stack->items = malloc(reservedSize * sizeof(*stack->items));

    if (stack->items == NULL){
        free(stack);
        stack = NULL;
        return NULL;
    }

    stack->capacity = reservedSize;

    return stack;
}


/* ============================================================
   Insertion
   ============================================================ */

// Push one data pointer onto the logical top of the stack.
//
// The stack stores the pointer itself; the pointed-to object
// is not copied.
//
// true  -> insertion succeeded
// false -> invalid arguments or allocation failure
bool stack_push(Stack *stack, void *data){
    if (stack == NULL || data == NULL){
        return false;
    }

    // Initial allocation for an empty stack with no backing storage.
    if (stack->capacity == 0){

        // Default allocation of 10 pointer slots.
        stack->capacity = 10;

        stack->items =
            calloc(stack->capacity, sizeof(*stack->items));

        if (stack->items == NULL){
            return false;
        }
    }

    // Grow the backing array if there is no remaining capacity.
    if (stack->size == stack->capacity){

        size_t newCapacity =
            stack->capacity * 2;

        void **items =
            realloc(
                stack->items,
                sizeof(*stack->items) * newCapacity
            );

        if (items == NULL){
            return false;
        }

        stack->capacity = newCapacity;
        stack->items = items;

        items = NULL;
    }

    // Push exactly once at the next free position.
    stack->items[stack->size] = data;
    stack->size++;

    return true;
}


/* ============================================================
   Access
   ============================================================ */

// Return the current logical top element without removing it.
//
// The returned pointer is borrowed.
const void *stack_peek(const Stack *stack){
    if (stack != NULL && stack->size > 0){
        return stack->items[stack->size - 1];
    }

    return NULL;
}


/* ============================================================
   Removal
   ============================================================ */

// Remove and return the current logical top element.
//
// Ownership of the removed pointer is transferred to the caller.
void *stack_pop(Stack *stack){
    if (stack == NULL){
        return NULL;
    }

    if (stack->size > 0){

        // After decrementing size, the new size value is also
        // the physical index of the old top element.
        stack->size--;

        void *data =
            stack->items[stack->size];

        stack->items[stack->size] = NULL;

        return data;
    }

    return NULL;
}


/* ============================================================
   Observation
   ============================================================ */

// Return the number of live elements currently stored.
size_t stack_size(const Stack *stack){
    return stack == NULL ? 0 : stack->size;
}


// Return the number of pointer slots currently allocated.
size_t stack_capacity(const Stack *stack){
    return stack == NULL ? 0 : stack->capacity;
}


// Return true if the stack contains no live elements.
//
// A NULL stack is treated as empty.
bool stack_is_empty(const Stack *stack){
    return stack == NULL || stack->size == 0;
}


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove all live elements while retaining the backing
// allocation and current capacity.
//
// If destroyData is non-NULL, it is called once for each
// live stored pointer.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void stack_clear(Stack *stack,void (*destroyData)(void *data)){
    if (stack == NULL){
        return;
    }

    for (size_t stack_size = stack->size;
         stack_size > 0;
         stack_size--){

        if (destroyData != NULL){
            destroyData(
                stack->items[stack_size - 1]
            );
        }

        stack->items[stack_size - 1] = NULL;
    }

    // Retain the backing allocation and capacity.
    stack->size = 0;
}


// Destroy the stack.
//
// If destroyData is non-NULL, it is called once for each
// live stored pointer before the backing array and Stack
// structure are freed.
//
// The caller's Stack pointer is invalid after this call.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void stack_destroy(Stack *stack,void (*destroyData)(void *data)){
    if (stack == NULL){
        return;
    }

    for (size_t stack_size = stack->size;
         stack_size > 0;
         stack_size--){

        if (destroyData != NULL){
            destroyData(
                stack->items[stack_size - 1]
            );
        }

        stack->items[stack_size - 1] = NULL;
    }

    // Free the backing array.
    free(stack->items);

    stack->items = NULL;
    stack->size = 0;
    stack->capacity = 0;

    // Finally free the Stack structure.
    free(stack);
}


/* ============================================================
   Debugging
   ============================================================ */

// Print every live stack element from bottom to top.
//
// Example:
//     BOTTOM -> [A, B, C] <- TOP
//
// The caller defines how one stored element is printed.
//
// This function does not modify the stack.
void stack_print(const Stack *stack,void (*print_func)(const void *data)){
    if (stack == NULL || print_func == NULL){
        return;
    }

    printf("BOTTOM -> [");

    for (size_t index = 0;
         index < stack->size;
         index++){

        print_func(stack->items[index]);

        if (index + 1 < stack->size){
            printf(", ");
        }
    }

    printf("] <- TOP\n");
}