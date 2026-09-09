#include <stdlib.h>
#include "maybe.h"

// make an empty Maybe type with Nothing state and NULL void*.
// also allocates a memory on heap.
Maybe* maybe_create(void){
    Maybe *maybe = malloc(sizeof(*maybe));
    if (maybe == NULL){
        return NULL;
    }
    maybe->maybeState = NOTHING;
    maybe->data = NULL;
    return maybe;
}

// free and release associated heap memory related to Maybe struct
// destroyData function pointer in order to destroy the void *data contents and free them up safely
// so caller needs to pass in a way to destroy the specific type
void maybe_destroy(Maybe *maybe,void (*destroyData)(void *data)){
    if (maybe == NULL){
        return;
    }
    // first free internal data
    if (destroyData != NULL && maybe->data != NULL){
        destroyData(maybe->data);
    }
    // then free container itself
    free(maybe);
}

// remove maybe data, with a clearly defined destruction policy. 
// similar to destroy but pointer and memory allocated for maybe still remains. 
// elements are freed as well so no responsibility to caller.
// destroyData can be NULL for borrowed, static, or stack-allocated data.
void maybe_clear(Maybe *maybe,void (*destroyData)(void *data)){
    if (maybe == NULL){
        return;
    }
    // free internal data only
    if (destroyData != NULL && maybe->data != NULL){
        destroyData(maybe->data);
    }
    maybe->data = NULL;
    maybe->maybeState = NOTHING;
}

// get the current MaybeState: NOTHING (NULL void *data) or Just (value address present)
MaybeState maybe_getState(const Maybe *maybe){
    if (maybe == NULL){
        return MAYBE_INVALID; // treats NULL maybe differently. As NULL maybe does not have a state.
    }
    return maybe->maybeState;
}

// get the current void *data. [borrowed read-only access]
const void* maybe_getData(const Maybe *maybe){
    if (maybe == NULL || maybe->maybeState != JUST){
        return NULL;
    }
    return maybe->data;
}

// modify void *data. Also previous data needs to be destroyed safely so a function pointer required (can be null if not needed).
// true = successful (modified and changed)
// false = unsuccessful
// INVARIANT:
// NOTHING <=> data == NULL
// JUST    <=> data != NULL
bool maybe_setData(Maybe *maybe, void *newData, void (*destroyPreviousData)(void *data)){
    if (maybe == NULL){
        return false;
    }
    if (maybe->data != NULL && destroyPreviousData != NULL){
        destroyPreviousData(maybe->data);
    }
    maybe->data = newData;
    if (newData == NULL){
        maybe->maybeState = NOTHING;
    } else{
        maybe->maybeState = JUST;
    }
    return true;
}