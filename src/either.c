#include "either.h"

#include <stdlib.h>

// make an Either type with EitherState and data initiliased.
// if eitherState EITHER_INVALID or NONE defaults to LEFT.
// also allocates a memory on heap.
// as void *data is assigned to either left or right it is not copied so shallow copy (only address)
// simpler words it is the same pointer put into Either container
Either* either_create(EitherState eitherState, void *data){
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
        case NONE: // intended fall through
        case LEFT: 
        default:
            either->data.left = data;
            either->eitherState = LEFT;
            break;
    }
    return either;
}

// free and release associated heap memory related to Either struct
// destroyData function pointer in order to destroy the void *data contents and free them up safely
// so caller needs to pass in a way to destroy the specific type
void either_destroy(Either *either,void (*destroyData)(void *data)){
    if (either == NULL){
        return;
    }
    // first free internal data
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
            case EITHER_INVALID: // intended fall through
            case NONE:
                break;
        }
    }
    // then free container itself
    free(either);
}

// remove either data, with a clearly defined destruction policy. 
// similar to destroy but pointer and memory allocated for either still remains. 
// elements are freed as well so no responsibility to caller.
// destroyData can be NULL for borrowed, static, or stack-allocated data.
void either_clear(Either *either,void (*destroyData)(void *data)){
    if (either == NULL){
        return;
    }
    // free internal data only
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
            case EITHER_INVALID: // intended fall through
            case NONE:
                break;
        }
    }
    either->data.left = NULL; // because both union members share the same storage it is enough to NULL one.
    either->eitherState = NONE;
}


// get the current EitherState: LEFT or RIGHT or NONE or EITHER_INVALID; NONE if either is has no data or EITHER_INVALID if either is NULL
// do not access RIGHT when the state is LEFT or vice versa. As implemented via union
EitherState either_getState(const Either *either){
    return (either == NULL) ? EITHER_INVALID : either->eitherState;
}

// get the current void *data. Based on LEFT or RIGHT state. If NONE or EITHER_INVALID state or NULL either then NULL returned
const void* either_getData(const Either *either){
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
bool either_setData(Either *either, EitherState newState, void *newData, void (*destroyPreviousData)(void *data)){
    if (either == NULL){
        return false;
    }
    if (newState != NONE && newState != LEFT && newState != RIGHT){
        return false;
    }
    if (destroyPreviousData != NULL){
        switch (either->eitherState){
            case LEFT:                           // to check not destroying a pointer already present
                if (either->data.left != NULL && (newState == NONE || newData != either->data.left)){
                    destroyPreviousData(either->data.left);
                } 
                break;
            case RIGHT:                           // to check not destroying a pointer already present
                if (either->data.right != NULL && (newState == NONE || newData != either->data.right)){
                    destroyPreviousData(either->data.right);
                } 
                break; 
            case EITHER_INVALID: // intended fall through
            case NONE:
                break;
        }
    }
    if (newState == NONE || newData == NULL){
        either->data.left = NULL;
        either->eitherState = NONE;
    } else if (newState == LEFT){
        either->data.left = newData;
        either->eitherState = LEFT;
    } else{
        either->data.right = newData;
        either->eitherState = RIGHT;
    }
    return true;
}
