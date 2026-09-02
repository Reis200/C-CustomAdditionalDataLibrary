#include <stdlib.h>

#include "stack.h"


// produce a valid empty stack
Stack* stack_create(void){
    Stack* stack = malloc(sizeof(*stack));
    if (stack == NULL){
        return NULL;
    }
    stack->size = 0;
    stack->capacity = 0;
    stack->items = NULL;
    return stack;
}

// true  = inserted
// false = duplicate / failure
// stack stores the pointer while the value remains present.
bool stack_push(Stack *stack,void *data){
    if (stack == NULL || data == NULL){
        return false;
    }
    // default empty stack just initialised with no data allocated
    if (stack->capacity == 0){ // size and capacity 0 pre-condition
        stack->capacity = 10; // array of size 10 is initialised by default
        stack->items = calloc(stack->capacity,sizeof(*stack->items));
        if (stack->items == NULL){
            return false; // allocation failed so we cant insert so operation failed
        }
    }
    // grow if not enough space
    if (stack->size == stack->capacity){
        size_t newCapacity = stack->capacity * 2; // doubles so capacity increases gets less and less
        void **items = realloc(stack->items,sizeof(*stack->items) * newCapacity);
        if (items == NULL){
            return false; // allocation failed so we cant insert so operation failed
        }
        stack->capacity = newCapacity; // doubles so capacity increases gets less and less
        stack->items = items; 
        items = NULL;
    }
    // push once
    stack->items[stack->size] = data;
    stack->size++; // size also tells us the current index location to access
    return true;
}

const void* stack_peek(const Stack *stack){
    if (stack != NULL && stack->size > 0){
        return stack->items[stack->size - 1];
    }
    return NULL;
}

// responsibility for the removed value returns to the caller.
void* stack_pop(Stack *stack){
    if (stack == NULL){
        return NULL;
    }
    if (stack->size > 0){
        stack->size--; // if size is non zero the top position is size - 1
        void *data = stack->items[stack->size];
        stack->items[stack->size] = NULL;
        return data;
    }
    return NULL;
}

// number of live stack data present
size_t stack_size(const Stack *stack){
    return stack == NULL ? 0 : stack->size;
}

bool stack_is_empty(const Stack *stack){
    return stack == NULL || stack->size == 0; // Null stack is treated as empty
}

// callers pointer is invalid after destruction
// it destroys the pointers present within the stack too, so caller just needs to pass in how to destroy internal data, rest is handled caller does not need to free any more memory.
// destroyData can be passed as null for stack only data. (as can not be freed)
void stack_destroy(Stack *stack, void (*destroyData)(void *data)){
    if (stack == NULL){
        return;
    }
    // we know that items has capacity data allocated so it must be freed first before freeing stack
    for (size_t stack_size = stack->size; stack_size > 0;stack_size--){ // each individual data is destroyed
        if (destroyData != NULL){
            destroyData(stack->items[stack_size-1]);
        }
        stack->items[stack_size-1] = NULL;
    } 
    // finally items freed
    free(stack->items);
    stack->items = NULL;
    stack->size = 0;
    stack->capacity = 0;
    // finally stack freed
    free(stack);
}

// remove all elements while retaining capacity, with a clearly defined destruction policy. 
// similar to destroy but pointer and memory allocated for stack and its capacity still remains. 
// (As elements removed only size changes) + elements are freed as well so no responsibility to caller.
// destroyData can be passed as null for stack only data. (as can not be freed)
void stack_clear(Stack *stack, void (*destroyData)(void *data)){ // modifies the original pointer and returns the new stack pointer with cleared stack
    if (stack == NULL){ // NULL stack case
        return;
    }
    // we know that items has capacity data allocated so it must be freed first before freeing stack (but this time do not free items nor stack just delete the data within)
    for (size_t stack_size = stack->size; stack_size > 0;stack_size--){ // each individual data is destroyed
        if (destroyData != NULL){
            destroyData(stack->items[stack_size-1]);
        }
        stack->items[stack_size-1] = NULL;
    } 
    // do not free items just set size to 0 as capacity unchanged
    stack->size = 0;
}


// preallocate enough capacity for a known workload.
// if allocation fails returns NULL so NULL checks necessary by the caller.
Stack* stack_reserve(size_t reservedSize){
    Stack *stack = stack_create();
    if (stack == NULL){
        return NULL;
    }
    stack->items = calloc(reservedSize,sizeof(*stack->items));
    if (stack->items == NULL){
        free(stack);
        stack = NULL;
        return NULL;
    }
    stack->capacity = reservedSize;
    return stack;
}

// expose current storage capacity for diagnostics or benchmarking.
size_t stack_capacity(const Stack *stack){
    return stack == NULL ? 0 : stack->capacity;
}

