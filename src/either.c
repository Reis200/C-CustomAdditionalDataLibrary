#include "either.h"

#include <stdlib.h>
#include <stdio.h>


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
Either *either_create(EitherState eitherState, void *data){
    Either *either = malloc(sizeof(*either));

    if (either == NULL){
        return NULL;
    }

    if (data == NULL){
        either->eitherState = NONE;
        either->data.left = NULL;
        return either;
    }

    switch (eitherState){
        case RIGHT:
            either->data.right = data;
            either->eitherState = RIGHT;
            break;

        case EITHER_INVALID:
        case NONE:
        case LEFT:
        default:
            either->data.left = data;
            either->eitherState = LEFT;
            break;
    }

    return either;
}


/* ============================================================
   Access
   ============================================================ */

// Return the current EitherState.
//
// A NULL Either pointer has no valid stored state, so
// EITHER_INVALID is returned.
EitherState either_getState(const Either *either){
    return (either == NULL)
        ? EITHER_INVALID
        : either->eitherState;
}


// Return the currently active stored value without removing it.
//
// The returned pointer is borrowed and read-only.
//
// Returns NULL if:
//     - either is NULL
//     - either is in the NONE state
//     - either is in the EITHER_INVALID state
const void *either_getData(const Either *either){
    if (either == NULL){
        return NULL;
    }

    switch (either->eitherState){
        case LEFT:
            return either->data.left;

        case RIGHT:
            return either->data.right;

        case NONE:
        case EITHER_INVALID:
        default:
            return NULL;
    }
}


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
// true  -> modification succeeded
// false -> either is NULL or newState is invalid
//
// Invariants:
//     LEFT:
//         active union member is data.left
//         data.left != NULL
//
//     RIGHT:
//         active union member is data.right
//         data.right != NULL
//
//     NONE:
//         no active value
//         union storage is NULL
//
// EITHER_INVALID is a query result for an invalid or NULL
// Either pointer and is not a normal stored state.
bool either_setData(Either *either,EitherState newState,void *newData,void (*destroyPreviousData)(void *data)){
    if (either == NULL){
        return false;
    }

    if (newState != NONE &&
        newState != LEFT &&
        newState != RIGHT){

        return false;
    }

    if (destroyPreviousData != NULL){

        switch (either->eitherState){

            case LEFT:
                // Do not destroy the currently stored pointer if
                // the same pointer will remain stored.
                if (either->data.left != NULL &&
                    (newState == NONE ||
                     newData != either->data.left)){

                    destroyPreviousData(either->data.left);
                }
                break;

            case RIGHT:
                // Do not destroy the currently stored pointer if
                // the same pointer will remain stored.
                if (either->data.right != NULL &&
                    (newState == NONE ||
                     newData != either->data.right)){

                    destroyPreviousData(either->data.right);
                }
                break;

            case EITHER_INVALID:
            case NONE:
                break;
        }
    }

    if (newState == NONE || newData == NULL){
        either->data.left = NULL;
        either->eitherState = NONE;
    }
    else if (newState == LEFT){
        either->data.left = newData;
        either->eitherState = LEFT;
    }
    else{
        either->data.right = newData;
        either->eitherState = RIGHT;
    }

    return true;
}


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
void either_clear(Either *either,void (*destroyData)(void *data)){
    if (either == NULL){
        return;
    }

    if (destroyData != NULL){

        switch (either->eitherState){

            case LEFT:
                if (either->data.left != NULL){
                    destroyData(either->data.left);
                }
                break;

            case RIGHT:
                if (either->data.right != NULL){
                    destroyData(either->data.right);
                }
                break;

            case EITHER_INVALID:
            case NONE:
                break;
        }
    }

    // Both union members share the same storage,
    // so clearing one pointer is sufficient.
    either->data.left = NULL;
    either->eitherState = NONE;
}


// Destroy the Either.
//
// If destroyData is non-NULL, it is called on the currently
// active stored value before the Either structure is freed.
//
// The caller's Either pointer is invalid after this call.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void either_destroy(Either *either,void (*destroyData)(void *data)){
    if (either == NULL){
        return;
    }

    if (destroyData != NULL){

        switch (either->eitherState){

            case LEFT:
                if (either->data.left != NULL){
                    destroyData(either->data.left);
                }
                break;

            case RIGHT:
                if (either->data.right != NULL){
                    destroyData(either->data.right);
                }
                break;

            case EITHER_INVALID:
            case NONE:
                break;
        }
    }

    free(either);
}


/* ============================================================
   Debugging
   ============================================================ */

// Print the current Either state for debugging.
//
// If the Either contains LEFT or RIGHT, the caller-provided
// print_func is used to print the active stored value.
//
// If the Either contains NONE, "NONE" is printed.
//
// If either is NULL, "EITHER_INVALID" is printed.
//
// This function does not modify the Either.
void either_print(const Either *either,void (*print_func)(const void *data)){
    if (either == NULL){
        printf("[EITHER_INVALID]\n");        
        return;
    }

    printf("[");

    switch (either->eitherState){
        case RIGHT:
            if (print_func == NULL){
                printf("RIGHT");
            } else{
                print_func(either->data.right);
            }
            break;
        case LEFT:
            if (print_func == NULL){
                printf("LEFT");
            } else{
                print_func(either->data.left);
            }
            break;
        case EITHER_INVALID:
            printf("EITHER_INVALID");
            break;
        case NONE:
            printf("NONE");
            break;
    }

    printf("]\n");
}
