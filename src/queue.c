#include "queue.h"

#include <stdlib.h>
#include <stdio.h>


/* ============================================================
   Construction
   ============================================================ */

// Produce a valid empty queue.
// Creates an empty Queue with:
//     items       = NULL
//     capacity    = 0
//     size        = 0
//     front_index = 0
//
// Also allocates the Queue structure on the heap.
Queue *queue_create(void){
    Queue *queue = malloc(sizeof(*queue));

    if (queue == NULL){
        return NULL;
    }

    queue->items = NULL;
    queue->capacity = 0;
    queue->size = 0;
    queue->front_index = 0;

    return queue;
}


// Preallocate enough capacity for a known workload.
// If allocation fails, NULL is returned.
Queue *queue_reserve(size_t reservedSize){
    Queue *queue = queue_create();

    if (queue == NULL){
        return NULL;
    }

    if (reservedSize == 0){
        return queue;
    }

    queue->items =
        malloc(reservedSize * sizeof(*queue->items));

    if (queue->items == NULL){
        free(queue);
        queue = NULL;
        return NULL;
    }

    queue->capacity = reservedSize;

    return queue;
}


/* ============================================================
   Insertion
   ============================================================ */

// Append one data pointer to the logical rear of the queue.
//
// true  -> insertion succeeded
// false -> insertion failed
bool queue_enqueue(Queue *queue, void *data){
    if (queue == NULL || data == NULL){
        return false;
    }

    // Empty queue with no backing allocation.
    if (queue->capacity == 0 && queue->items == NULL){

        // Default allocation of 5 pointer slots.
        size_t capacity = 5;

        queue->items =
            malloc(sizeof(*queue->items) * capacity);

        if (queue->items == NULL){
            return false;
        }

        queue->capacity = capacity;
    }


    // Grow if there is no remaining capacity.
    if (queue->size == queue->capacity &&
        queue->items != NULL){

        size_t newCapacity =
            queue->capacity * 2;

        void **items =
            malloc(sizeof(*queue->items) * newCapacity);

        if (items == NULL){
            return false;
        }

        /*
            Self-note:

            old physical arrangement:

            [ D, E, A, B, C ]
                  ^
                front

            logical queue:

            A, B, C, D, E

            new:

            [ A, B, C, D, E, -, -, -, -, - ]
              ^
            front = 0
        */

        // Copy the old circular queue into the new
        // contiguous backing array.
        for (size_t index = 0;
             index < queue->size;
             index++){

            size_t oldIndex =
                (queue->front_index + index)
                % queue->capacity;

            items[index] =
                queue->items[oldIndex];
        }

        // Free the old backing array and replace it
        // with the newly allocated one.
        free(queue->items);

        queue->front_index = 0;
        queue->capacity = newCapacity;
        queue->items = items;

        items = NULL;
    }


    // Determine the physical rear position.
    size_t rear_index =
        (queue->front_index + queue->size)
        % queue->capacity;

    queue->items[rear_index] = data;
    queue->size++;

    return true;
}


/* ============================================================
   Access
   ============================================================ */

// Observe the current logical front without removing it.
//
// The returned pointer is borrowed.
const void *queue_peek(const Queue *queue){
    if (queue != NULL && queue->size > 0){
        return queue->items[queue->front_index];
    }

    return NULL;
}


/* ============================================================
   Removal
   ============================================================ */

// Remove the current logical front and return the exact
// pointer stored there.
//
// Ownership of the removed value is transferred to the caller.
void *queue_dequeue(Queue *queue){
    if (queue == NULL || queue->size == 0){
        return NULL;
    }

    size_t current_front_index =
        queue->front_index;

    void *data =
        queue->items[current_front_index];

    queue->items[queue->front_index] = NULL;

    queue->size--;

    if (queue->size == 0){
        queue->front_index = 0;
    }
    else{
        queue->front_index =
            (queue->front_index + 1)
            % queue->capacity;
    }

    return data;
}


/* ============================================================
   Observation
   ============================================================ */

// Return the number of live elements currently stored.
size_t queue_size(const Queue *queue){
    return queue == NULL ? 0 : queue->size;
}


// Return the number of pointer slots currently allocated.
size_t queue_capacity(const Queue *queue){
    return queue == NULL ? 0 : queue->capacity;
}


// true  -> empty queue or NULL queue
// false -> non-empty queue
bool queue_is_empty(const Queue *queue){
    return (queue == NULL || queue->size == 0)
        ? true
        : false;
}


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove all live elements while retaining the backing
// allocation and capacity.
//
// destroyData may be NULL for borrowed, static,
// or stack-allocated data.
void queue_clear(Queue *queue,void (*destroyData)(void *data)){
    if (queue == NULL){
        return;
    }

    for (size_t index = 0;
         index < queue->size;
         index++){

        size_t physicalIndex =
            (queue->front_index + index)
            % queue->capacity;

        if (destroyData != NULL){
            destroyData(
                queue->items[physicalIndex]
            );
        }

        queue->items[physicalIndex] = NULL;
    }

    // Retain the backing allocation and capacity.
    queue->size = 0;
    queue->front_index = 0;
}


// Destroy the queue.
//
// If destroyData is non-NULL, every live stored value
// is destroyed before the queue storage is released.
//
// The caller's Queue pointer is invalid after this call.
void queue_destroy(Queue *queue,void (*destroyData)(void *data)){
    if (queue == NULL){
        return;
    }

    for (size_t index = 0;
         index < queue->size;
         index++){

        size_t physicalIndex =
            (queue->front_index + index)
            % queue->capacity;

        if (destroyData != NULL){
            destroyData(
                queue->items[physicalIndex]
            );
        }

        queue->items[physicalIndex] = NULL;
    }

    // Free the backing array.
    free(queue->items);

    queue->items = NULL;
    queue->capacity = 0;
    queue->size = 0;
    queue->front_index = 0;

    // Finally free the Queue structure.
    free(queue);
}


/* ============================================================
   Debugging
   ============================================================ */

// Print every live queue element in logical FIFO order.
//
// Example:
//     FRONT -> [A, B, C] <- BACK
//
// The caller defines how one stored element is printed.
void queue_print(const Queue *queue,void (*print_func)(const void *data)){
    if (queue == NULL || print_func == NULL){
        return;
    }

    printf("FRONT -> [");

    for (size_t index = 0;index < queue->size;index++){
        size_t physicalIndex =
            (queue->front_index + index)
            % queue->capacity;

        print_func(
            queue->items[physicalIndex]
        );

        if (index + 1 < queue->size){
            printf(", ");
        }
    }

    printf("] <- BACK\n");
}