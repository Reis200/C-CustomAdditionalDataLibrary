#ifndef MAYBE_H
#define MAYBE_H

#include <stdbool.h>

/*
    Generic Maybe Type

    A Maybe represents an optional value.

    States:
    - MAYBE_INVALID:
          Used as a query result when the Maybe pointer itself is NULL.
          This should not normally be stored inside a valid Maybe object.

    - NOTHING:
          Represents a valid Maybe containing no value.

    - JUST:
          Represents a valid Maybe containing a value.

    Representation invariants:
    - NOTHING <=> data == NULL
    - JUST    <=> data != NULL
*/

typedef enum {
    MAYBE_INVALID,
    NOTHING,
    JUST
} MaybeState;


typedef struct {
    MaybeState maybeState;
    void *data;
} Maybe;


/* ============================================================
   Construction
   ============================================================ */

// Construct a valid empty Maybe.
//
// Initial state:
//     maybeState = NOTHING
//     data       = NULL
//
// The Maybe structure itself is allocated on the heap.
//
// Returns NULL if allocation fails.
Maybe *maybe_create(void);


/* ============================================================
   Access
   ============================================================ */

// Return the current MaybeState.
//
// Returns:
//     MAYBE_INVALID -> maybe is NULL
//     NOTHING       -> valid Maybe containing no value
//     JUST          -> valid Maybe containing a value
MaybeState maybe_getState(const Maybe *maybe);


// Return the currently stored value without removing it.
//
// The returned pointer is borrowed and read-only.
//
// Returns NULL if:
//     - maybe is NULL
//     - maybe is in the NOTHING state
const void *maybe_getData(const Maybe *maybe);


/* ============================================================
   Modification
   ============================================================ */

// Replace the currently stored value with newData.
//
// If destroyPreviousData is non-NULL, it is called on the
// previously stored value before replacement.
//
// The new pointer is stored directly; the pointed-to object
// is not copied.
//
// The implementation must preserve:
//
//     NOTHING <=> data == NULL
//     JUST    <=> data != NULL
//
// Returns:
//     true  -> modification succeeded
//     false -> maybe is NULL
bool maybe_setData(Maybe *maybe,void *newData,void (*destroyPreviousData)(void *data));


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove the currently stored value while retaining the
// Maybe structure.
//
// If destroyData is non-NULL, it is called on the currently
// stored value before the Maybe becomes empty.
//
// Pass NULL for borrowed, static, or stack-allocated data.
//
// After clearing:
//     maybeState = NOTHING
//     data       = NULL
void maybe_clear(Maybe *maybe,void (*destroyData)(void *data));


// Destroy the Maybe.
//
// If destroyData is non-NULL, it is called on the currently
// stored value before the Maybe structure is freed.
//
// After this function returns, the caller's Maybe pointer is
// invalid and must not be dereferenced.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void maybe_destroy(Maybe *maybe,void (*destroyData)(void *data));


/* ============================================================
   Debugging
   ============================================================ */

// Print the current Maybe state for debugging.
//
// If the Maybe contains JUST, print_func is used to print
// the stored value.
//
// If the Maybe contains NOTHING, "NOTHING" is printed.
//
// If maybe is NULL, "MAYBE_INVALID" is printed.
//
// The caller supplies print_func to define how one stored
// value should be printed.
//
// This function does not modify the Maybe.
void maybe_print(const Maybe *maybe,void (*print_func)(const void *data));

#endif