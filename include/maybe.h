#ifndef MAYBE_H
#define MAYBE_H

#include <stdbool.h>

/* 
    Maybe states similar to Haskell (if not familiar it is a type where it encapsulates optional value)
    NOTHING is when there is no value present so void *data will be also NULL
    Just is when there is a value present so void *data will contain an address
*/
typedef enum {
    MAYBE_INVALID, // the Maybe pointer itself is invalid 
    NOTHING, // valid Maybe, no value
    JUST, // valid Maybe containing a value
} MaybeState;

/*
    Using Maybe is a good way to deal with errors or exceptional cases without resorting to 
    drastic measures such as error.
    INVARIANT:
    NOTHING <=> data == NULL
    JUST    <=> data != NULL
*/
typedef struct {
    MaybeState maybeState;
    void *data;
} Maybe;

// make an empty Maybe type with Nothing state and NULL void*.
// also allocates a memory on heap.
Maybe* maybe_create(void);

// free and release associated heap memory related to Maybe struct
// destroyData function pointer in order to destroy the void *data contents and free them up safely
// so caller needs to pass in a way to destroy the specific type
void maybe_destroy(Maybe *maybe,void (*destroyData)(void *data));

// remove maybe data, with a clearly defined destruction policy. 
// similar to destroy but pointer and memory allocated for maybe still remains. 
// elements are freed as well so no responsibility to caller.
// destroyData can be NULL for borrowed, static, or stack-allocated data.
void maybe_clear(Maybe *maybe,void (*destroyData)(void *data));

// get the current MaybeState: NOTHING (NULL void *data) or Just (value address present)
MaybeState maybe_getState(const Maybe *maybe);

// get the current void *data. [borrowed read-only access]
const void* maybe_getData(const Maybe *maybe);

// modify void *data. Also previous data needs to be destroyed safely so a function pointer required (can be null if not needed).
// true = successful (modified and changed)
// false = unsuccessful
// INVARIANT:
// NOTHING <=> data == NULL
// JUST    <=> data != NULL
bool maybe_setData(Maybe *maybe, void *newData, void (*destroyPreviousData)(void *data));


#endif