#ifndef MAYBE_H
#define MAYBE_H

/* 
    Maybe states similar to Haskell (if not familiar it is a type where it encapsulates optional value)
    NOTHING is when there is no value present so void *data will be also NULL
    Just is when there is a value present so void *data will contain an address
*/
typedef enum {
    NOTHING,
    JUST,
} MaybeState;

/*
    Using Maybe is a good way to deal with errors or exceptional cases without resorting to 
    drastic measures such as error.
*/
typedef struct {
    MaybeState maybeState;
    void *data;
} Maybe;

// make an empty Maybe type with Nothing state and NULL void*.
// also allocates a memory on heap.
Maybe* maybe_create();

// free and release associated heap memory related to Maybe struct
// destroyData function pointer in order to destroy the void *data contents and free them up safely
// so caller needs to pass in a way to destroy the specific type
void maybe_destroy(Maybe *maybe,void (*destroyData)(void *data));

// get the current MaybeState: NOTHING (NULL void *data) or Just (value address present)
MaybeState maybe_getState(Maybe *maybe);

// get the current void *data. Recommended to getState(Maybe *maybe) first or 
// check [MaybeState: JUST] in order to access.
void* maybe_getData(Maybe *maybe);


#endif