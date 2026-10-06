#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdbool.h>

/*
    Generic Stack

    Representation invariants:
    - 0 <= size <= capacity.
    - If capacity == 0, items may be NULL.
    - If capacity > 0, items points to storage for at least
      capacity elements of type void *.
    - Live elements occupy the contiguous range:
          items[0] through items[size - 1]
    - If size > 0, the logical top is:
          items[size - 1]
    - Slots from items[size] through items[capacity - 1]
      are unused storage and are not part of the abstract stack.
    - Logical stack order from bottom to top is identical to
      physical live-array order.
*/

typedef struct {
    void **items;      // Backing array of stored pointers.
    size_t capacity;   // Number of pointer slots currently allocated.
    size_t size;       // Number of live elements currently stored.
} Stack;


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
// Returns NULL if allocation of the Stack structure fails.
Stack *stack_create(void);


// Create an empty stack with capacity reserved for at least
// reservedSize elements.
//
// The returned stack still has size == 0.
//
// For reservedSize == 0 empty stack returned
// cases reserving will overflow as SIZE_MAX is reached, NULL is returned.
//
// Returns NULL if allocation fails.
Stack *stack_reserve(size_t reservedSize);


/* ============================================================
   Insertion
   ============================================================ */

// Push data onto the logical top of the stack.
//
// The stack stores the pointer itself; the pointed-to object
// is not copied.
//
// The backing array grows automatically if necessary.
//
// Returns:
//     true  -> insertion succeeded
//     false -> invalid arguments or allocation failure
bool stack_push(Stack *stack,void *data);


/* ============================================================
   Access
   ============================================================ */

// Return the current logical top element without removing it.
//
// The returned pointer is borrowed and remains owned according
// to the stack's existing ownership policy.
//
// Returns NULL if the stack is NULL or empty.
const void *stack_peek(const Stack *stack);


/* ============================================================
   Removal
   ============================================================ */

// Remove and return the current logical top element.
//
// Ownership of the removed pointer is transferred to the caller.
//
// Returns NULL if the stack is NULL or empty.
void *stack_pop(Stack *stack);


/* ============================================================
   Observation
   ============================================================ */

// Return the number of live elements currently stored.
//
// Returns 0 for a NULL stack.
size_t stack_size(const Stack *stack);


// Return the number of pointer slots currently allocated.
//
// Returns 0 for a NULL stack.
size_t stack_capacity(const Stack *stack);


// Return true if the stack contains no live elements.
//
// A NULL stack is treated as empty.
bool stack_is_empty(const Stack *stack);


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove all live elements while retaining the backing allocation
// and current capacity.
//
// If destroyData is non-NULL, it is called once for each live
// stored pointer.
//
// Pass NULL for borrowed, static, or stack-allocated data.
//
// After clearing:
//     size     = 0
//     capacity = unchanged
//     items    = unchanged
void stack_clear(Stack *stack,void (*destroyData)(void *data));


// Destroy the stack.
//
// If destroyData is non-NULL, it is called once for each live
// stored pointer before the backing array and Stack structure
// are freed.
//
// After this function returns, the caller's Stack pointer is
// invalid and must not be dereferenced.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void stack_destroy(Stack *stack,void (*destroyData)(void *data));


/* ============================================================
   Debugging
   ============================================================ */

// Print every live stack element from bottom to top.
//
// Example:
//
//     BOTTOM -> [A, B, C] <- TOP
//
// The caller supplies print_func to define how one stored value
// should be printed.
//
// This function does not modify the stack.
void stack_print(const Stack *stack,void (*print_func)(const void *data));

#endif