#ifndef EITHER_H 
#define EITHER_H

#include <stdbool.h>

/*
The Either type represents values with two possibilities: 
a value of type Either has either data.left or data.right.
The Either type is sometimes used to represent a value which is either correct or an error; 
by convention, the data.left for error value and the data.right for the correct value. 
(mnemonic: "right" also means "correct").

There is also NONE for absence of either RIGHT or LEFT.
EITHER_INVALID is a query result for an invalid/null container,
not a normal stored state.
*/
typedef enum {
    EITHER_INVALID,NONE,LEFT,RIGHT
} EitherState;

typedef struct {
    EitherState eitherState;
    union {
        void *left; // "Left" value
        void *right; // "Right" value
    } data;
} Either;

// make an Either type with EitherState and data initiliased.
// if eitherState EITHER_INVALID or NONE defaults to LEFT.
// also allocates a memory on heap.
// as void *data is assigned to either left or right it is not copied so shallow copy (only address)
// simpler words it is the same pointer put into Either container
Either* either_create(EitherState eitherState, void *data);

// free and release associated heap memory related to Either struct
// destroyData function pointer in order to destroy the void *data contents and free them up safely
// so caller needs to pass in a way to destroy the specific type
void either_destroy(Either *either,void (*destroyData)(void *data));

// remove either data, with a clearly defined destruction policy. 
// similar to destroy but pointer and memory allocated for either still remains. 
// elements are freed as well so no responsibility to caller.
// destroyData can be NULL for borrowed, static, or stack-allocated data.
void either_clear(Either *either,void (*destroyData)(void *data));


// get the current EitherState: LEFT or RIGHT or NONE or EITHER_INVALID; NONE if either is has no data or EITHER_INVALID if either is NULL
// do not access RIGHT when the state is LEFT or vice versa. As implemented via union
EitherState either_getState(const Either *either);

// get the current void *data. Based on LEFT or RIGHT state. If NONE or EITHER_INVALID state or NULL either then NULL returned
const void* either_getData(const Either *either);

// modify void *data. Also previous data needs to be destroyed safely so a function pointer required (can be null if not needed).
// true = successful (modified and changed)
// false = unsuccessful
// INVARIANT:
// eitherState == LEFT
//     => active union member is data.left
//     => data.left != NULL

// eitherState == RIGHT
//     => active union member is data.right
//     => data.right != NULL

// eitherState == NONE
//     => no active value
//     => union storage is NULL
// EITHER_INVALID is a query result for an invalid/null container,
// not a normal stored state.
bool either_setData(Either *either, EitherState newState, void *newData, void (*destroyPreviousData)(void *data));

#endif