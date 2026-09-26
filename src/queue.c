#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

// Produce a valid empty queue value.
// make an empty Queue type with NULL items, capacity = 0, size = 0, front_index = 0.
// also allocates a memory on heap.
Queue* queue_create(void){
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

// Append one data pointer to the logical rear of the given queue.
// => enqueue given data.
// true -> if successful
// false -> if unable to do the operation or error occurred.
bool queue_enqueue(Queue *queue,void *data){
    if (queue == NULL || data == NULL){
        return false;
    }
    // empty queue
    if (queue->capacity == 0 && queue->items == NULL){
        // default allocation of 5 pointer slots
        size_t capacity = 5;
        queue->items = malloc(sizeof(*queue->items) * capacity);
        if (queue->items == NULL){
            return false;
        }
        queue->capacity = capacity;
    }
    // grow if not enough space
    if (queue->size == queue->capacity && queue->items != NULL){
        size_t newCapacity = queue->capacity * 2; // doubles so insertions are amortised O(1)
        void **items = malloc(sizeof(*queue->items) * newCapacity);
        if (items == NULL){
            return false; // allocation failed so we cant insert so operation failed
        }
        /*
        self-note:
        old:
        [ D, E, A, B, C ]
            ^
            front

        new:
        [ A, B, C, D, E, -, -, -, -, - ]
        ^
        front = 0
        */
        // copy the old circular queue into the new one
        for (size_t index = 0; index < queue->size; index++){
            size_t oldIndex = (queue->front_index + index) % queue->capacity;
            items[index] = queue->items[oldIndex];
        }
        // free the old backing array and set the new array onto it
        free(queue->items);
        queue->front_index = 0;
        queue->capacity = newCapacity; // doubles so capacity increases gets less and less
        queue->items = items; 
        items = NULL;
    }
    // enqueue once
    size_t rear_index = (queue->front_index + queue->size) % queue->capacity;
    queue->items[rear_index] = data;
    queue->size++;
    return true;
}

// Observe the current front without removing it from the given queue.
// => peek given data.
const void* queue_peek(Queue *queue){
    if (queue != NULL && queue->size > 0){
        return queue->items[queue->front_index];
    }
    return NULL;
}

// Remove the current front and return the exact stored pointer from the given queue.
// The caller becomes responsible for the removed value. So freeing is caller's responsibility.
void* queue_dequeue(Queue *queue){
    if (queue == NULL || queue->size == 0){
        return NULL;
    }
    size_t current_front_index = queue->front_index;

    void *data = queue->items[current_front_index];
    queue->items[queue->front_index] = NULL;

    queue->size--;

    if (queue->size == 0){
        queue->front_index = 0;
    } else{
        queue->front_index = (queue->front_index + 1) % queue->capacity;
    }
    return data;
}

// preallocate enough capacity for a known workload.
// if allocation fails returns NULL so NULL checks necessary by the caller.
Queue* queue_reserve(size_t reservedSize){
    Queue *queue = queue_create();
    if (queue == NULL){
        return NULL;
    }
    if (reservedSize == 0) {
        return queue;
    }
    queue->items = malloc(reservedSize * sizeof(*queue->items));
    if (queue->items == NULL){
        free(queue);
        queue = NULL;
        return NULL;
    }
    queue->capacity = reservedSize;
    return queue;
}

// Return the number of elements within the queue.
size_t queue_size(Queue *queue){
    return queue == NULL ? 0 : queue->size;
}

// true  -> empty queue or NULL queue
// false -> non-empty queue 
bool queue_is_empty(Queue *queue){ // self note == instead of <= as size_t is unsigned
    return (queue == NULL || queue->size == 0) ? true : false;
}

// callers pointer is invalid after destruction
// it destroys the pointers present within the queue too, so caller just needs to pass in how to destroy internal data, rest is handled caller does not need to free any more memory.
// destroyData can be passed as null for stack only data. (as can not be freed)
void queue_destroy(Queue *queue, void (*destroyData)(void *data)){
    if (queue == NULL){
        return;
    }
    // we know that items has capacity data allocated so it must be freed first before freeing queue
    for (size_t index = 0; index < queue->size;index++){ // each individual data is destroyed
        size_t physicalIndex = (queue->front_index + index) % queue->capacity;
        if (destroyData != NULL){
            destroyData(queue->items[physicalIndex]);
        }
        queue->items[physicalIndex] = NULL;
    } 
    // finally items freed
    free(queue->items);
    queue->items = NULL;
    queue->capacity = 0;
    queue->size = 0; 
    queue->front_index = 0;
    // finally queue freed
    free(queue);
}

// remove all elements while retaining capacity, with a clearly defined destruction policy. 
// similar to destroy but pointer and memory allocated for queue and its capacity still remains. 
// (As elements removed only size changes) + elements are freed as well so no responsibility to caller.
// destroyData can be NULL for borrowed, static, or stack-allocated data.
void queue_clear(Queue *queue,void (*destroyData)(void *data)){
    if (queue == NULL){ // NULL queue case
        return;
    }
    // we know that items has capacity data allocated so it must be freed first before freeing queue (but this time do not free items nor queue just delete the data within)
    for (size_t index = 0; index < queue->size;index++){ // each individual data is destroyed
        size_t physicalIndex = (queue->front_index + index) % queue->capacity;
        if (destroyData != NULL){
            destroyData(queue->items[physicalIndex]);
        }
        queue->items[physicalIndex] = NULL;
    } 
    // do not free items just set size to 0 as capacity unchanged (and set other fields to default value)
    queue->size = 0;
    queue->front_index = 0;
}

// expose current storage capacity for diagnostics or benchmarking.
size_t queue_capacity(const Queue *queue){
    return queue == NULL ? 0 : queue->capacity;
}

// print out each element of queue for debugging
// caller needs to define how to print out each element within their queue.
// Prints queue from front to last item queued.
// The rightmost element is the last item to be dequeued.
void queue_printQueue(const Queue *queue, void (*print_func)(const void *)){
    if (queue == NULL || print_func == NULL){
        return;
    }
    
    printf("Front -> [");
    for (size_t index = 0; index < queue->size; index++){
        size_t physicalIndex = (queue->front_index + index) % queue->capacity;
        print_func(queue->items[physicalIndex]);
        if (index + 1 < queue->size){
            printf(", ");
        }
    }
    printf("] <- BACK\n");
}