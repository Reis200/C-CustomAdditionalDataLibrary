#ifndef EITHER_H 
#define EITHER_H

/*
The Either type represents values with two possibilities: 
a value of type Either has either data.left or data.right.
The Either type is sometimes used to represent a value which is either correct or an error; 
by convention, the data.left for error value and the data.right for the correct value. 
(mnemonic: "right" also means "correct").
*/
typedef enum {
    LEFT, RIGHT
} EitherState;

typedef struct {
    EitherState eitherState;
    union {
        void *left; // "Left" value
        void *right; // "Right" value
    } data;
} Either;

// make an Either type with EitherState and data initiliased.
// also allocates a memory on heap.
// as void *data is assigned to either left or right it is not copied so shallow copy (only address)
// simpler words it is the same pointer put into Either container
Either* either_create(EitherState eitherState, void *data);

// free and release associated heap memory related to Either struct
// destroyData function pointer in order to destroy the void *data contents and free them up safely
// so caller needs to pass in a way to destroy the specific type
void either_destroy(Either *either,void (*destroyData)(void *data));

// get the current EitherState: LEFT or RIGHT
// do not access RIGHT when the state is LEFT or vice versa. As implemented via union
EitherState either_getState(Either *either);

// get the current void *data. Based on LEFT or RIGHT state.
void* either_getData(Either *either);

#endif