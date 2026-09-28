#include "maybe.h"

#include <stdlib.h>
#include <stdio.h>


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
Maybe *maybe_create(void){
    Maybe *maybe = malloc(sizeof(*maybe));

    if (maybe == NULL){
        return NULL;
    }

    maybe->maybeState = NOTHING;
    maybe->data = NULL;

    return maybe;
}


/* ============================================================
   Access
   ============================================================ */

// Return the current MaybeState.
//
// A NULL Maybe pointer has no valid stored state, so
// MAYBE_INVALID is returned.
MaybeState maybe_getState(const Maybe *maybe){
    if (maybe == NULL){
        return MAYBE_INVALID;
    }

    return maybe->maybeState;
}


// Return the currently stored value without removing it.
//
// The returned pointer is borrowed and read-only.
//
// Returns NULL if the Maybe is NULL or does not contain JUST.
const void *maybe_getData(const Maybe *maybe){
    if (maybe == NULL || maybe->maybeState != JUST){
        return NULL;
    }

    return maybe->data;
}


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
// true  -> modification succeeded
// false -> maybe is NULL
//
// Invariant:
//     NOTHING <=> data == NULL
//     JUST    <=> data != NULL
bool maybe_setData(Maybe *maybe,void *newData,void (*destroyPreviousData)(void *data)){
    if (maybe == NULL){
        return false;
    }

    if (maybe->data != NULL &&
        destroyPreviousData != NULL){

        destroyPreviousData(maybe->data);
    }

    maybe->data = newData;

    if (newData == NULL){
        maybe->maybeState = NOTHING;
    }
    else{
        maybe->maybeState = JUST;
    }

    return true;
}


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove the currently stored value while retaining the
// Maybe structure.
//
// If destroyData is non-NULL, it is called on the currently
// stored value.
//
// Pass NULL for borrowed, static, or stack-allocated data.
//
// After clearing:
//     maybeState = NOTHING
//     data       = NULL
void maybe_clear(Maybe *maybe,void (*destroyData)(void *data)){
    if (maybe == NULL){
        return;
    }

    if (destroyData != NULL &&
        maybe->data != NULL){

        destroyData(maybe->data);
    }

    maybe->data = NULL;
    maybe->maybeState = NOTHING;
}


// Destroy the Maybe.
//
// If destroyData is non-NULL, it is called on the currently
// stored value before the Maybe structure is freed.
//
// The caller's Maybe pointer is invalid after this call.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void maybe_destroy(Maybe *maybe,void (*destroyData)(void *data)){
    if (maybe == NULL){
        return;
    }

    if (destroyData != NULL &&
        maybe->data != NULL){

        destroyData(maybe->data);
    }

    free(maybe);
}


/* ============================================================
   Debugging
   ============================================================ */

// Print the current Maybe state for debugging.
//
// Examples:
//     [JUST: 42]
//     [NOTHING]
//     [MAYBE_INVALID]
//
// If the Maybe contains JUST, the caller-provided print_func
// is used to print the stored value.
//
// This function does not modify the Maybe.
void maybe_print(const Maybe *maybe,void (*print_func)(const void *data)){
    if (maybe == NULL){
        printf("[MAYBE_INVALID]\n");
        return;
    }

    if (maybe->maybeState == JUST){
        if (print_func == NULL){
            return;
        }

        printf("[JUST: ");
        print_func(maybe->data);
        printf("]\n");
    }
    else{
        printf("[NOTHING]\n");
    }
}