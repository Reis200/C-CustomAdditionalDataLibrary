#ifndef EITHER_H
#define EITHER_H

#include <stdbool.h>

/*
    Generic Either Type

    An Either represents a value with two possible alternatives.

    By convention:
    - LEFT  is commonly used for an error or alternative value.
    - RIGHT is commonly used for a successful or expected value.

    States:
    - EITHER_INVALID:
          Used as a query result when the Either pointer itself is NULL.
          This should not normally be stored inside a valid Either object.

    - NONE:
          Represents a valid Either containing no active value.

    - LEFT:
          Represents a valid Either containing a value in data.left.

    - RIGHT:
          Represents a valid Either containing a value in data.right.

    Representation invariants:
    - LEFT:
          active union member is data.left
          data.left != NULL

    - RIGHT:
          active union member is data.right
          data.right != NULL

    - NONE:
          no active value
          union storage is NULL

    - EITHER_INVALID:
          query result for an invalid or NULL Either pointer;
          not a normal stored state.
*/

typedef enum {
    EITHER_INVALID,
    NONE,
    LEFT,
    RIGHT
} EitherState;


typedef struct {
    EitherState eitherState;

    union {
        void *left;
        void *right;
    } data;
} Either;


/* ============================================================
   Construction
   ============================================================ */

// Construct an Either using eitherState and data.
//
// If data == NULL:
//     eitherState becomes NONE.
//
// If data != NULL:
//     RIGHT stores data in data.right.
//     LEFT stores data in data.left.
//     NONE or EITHER_INVALID default to LEFT.
//
// The pointer itself is stored directly; the pointed-to object
// is not copied.
//
// The Either structure itself is allocated on the heap.
//
// Returns NULL if allocation fails.
Either *either_create(EitherState eitherState,void *data);


/* ============================================================
   Access
   ============================================================ */

// Return the current EitherState.
//
// Returns:
//     EITHER_INVALID -> either is NULL
//     NONE           -> valid Either containing no value
//     LEFT           -> valid Either containing a left value
//     RIGHT          -> valid Either containing a right value
EitherState either_getState(const Either *either);


// Return the currently active stored value without removing it.
//
// If the state is LEFT, data.left is returned.
// If the state is RIGHT, data.right is returned.
//
// The returned pointer is borrowed and read-only.
//
// Returns NULL if:
//     - either is NULL
//     - either is in the NONE state
//     - either is in the EITHER_INVALID state
const void *either_getData(const Either *either);


/* ============================================================
   Modification
   ============================================================ */

// Replace the currently active value and state.
//
// If destroyPreviousData is non-NULL, it is called on the
// previously stored value before replacement, unless the same
// pointer remains stored.
//
// newState must be one of:
//     NONE
//     LEFT
//     RIGHT
//
// If newState == NONE or newData == NULL, the Either becomes NONE.
//
// If newState == LEFT, newData becomes the active data.left value.
//
// If newState == RIGHT, newData becomes the active data.right value.
//
// The new pointer is stored directly; the pointed-to object
// is not copied.
//
// Returns:
//     true  -> modification succeeded
//     false -> either is NULL or newState is invalid
bool either_setData(Either *either,EitherState newState,void *newData,void (*destroyPreviousData)(void *data));


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove the currently active value while retaining the
// Either structure.
//
// If destroyData is non-NULL, it is called on the currently
// active stored value.
//
// Pass NULL for borrowed, static, or stack-allocated data.
//
// After clearing:
//     eitherState = NONE
//     union storage = NULL
void either_clear(Either *either,void (*destroyData)(void *data));


// Destroy the Either.
//
// If destroyData is non-NULL, it is called on the currently
// active stored value before the Either structure is freed.
//
// After this function returns, the caller's Either pointer is
// invalid and must not be dereferenced.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void either_destroy(Either *either,void (*destroyData)(void *data));


/* ============================================================
   Debugging
   ============================================================ */

// Print the current Either state for debugging.
//
// If the Either contains LEFT or RIGHT, print_func is used
// to print the active stored value.
//
// If the Either contains NONE, "NONE" is printed.
//
// If either is NULL, "EITHER_INVALID" is printed.
//
// The caller supplies print_func to define how one stored
// value should be printed.
//
// This function does not modify the Either.
void either_print(const Either *either,void (*print_func)(const void *data));

#endif