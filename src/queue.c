#include "queue.h"
#include <stdlib.h>

// Produce a valid empty queue value.
// make an empty Queue type with NULL items, capacity = 0, size = 0, front_index = 0.
// also allocates a memory on heap.
Queue* queue_create(void);

// Append one data pointer to the logical rear of the given queue.
// => enqueue given data.
void queue_enqueue(Queue *queue,void *data);

// Observe the current front without removing it from the given queue.
// => peek given data.
void queue_peek(Queue *queue,void *data);

// Remove the current front and return the exact stored pointer from the given queue.
// The caller becomes responsible for the removed value. So freeing is caller's responsibility.
void* queue_dequeue(Queue *queue);

// preallocate enough capacity for a known workload.
// if allocation fails returns NULL so NULL checks necessary by the caller.
Queue* queue_reserve(size_t reservedSize);

// Return the number of elements within the queue.
size_t queue_size(Queue *queue);

// true  -> empty queue or NULL queue
// false -> non-empty queue 
bool queue_is_empty(Queue *queue); 

// callers pointer is invalid after destruction
// it destroys the pointers present within the queue too, so caller just needs to pass in how to destroy internal data, rest is handled caller does not need to free any more memory.
// destroyData can be passed as null for stack only data. (as can not be freed)
void queue_destroy(Queue *queue, void (*destroyData)(void *data));

// remove all elements while retaining capacity, with a clearly defined destruction policy. 
// similar to destroy but pointer and memory allocated for queue and its capacity still remains. 
// (As elements removed only size changes) + elements are freed as well so no responsibility to caller.
// destroyData can be NULL for borrowed, static, or stack-allocated data.
void queue_clear(Queue *queue,void (*destroyData)(void *data));

// expose current storage capacity for diagnostics or benchmarking.
size_t queue_capacity(const Queue *queue);

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
        print_func(queue->items[index]);
        if (index + 1 < queue->size){
            printf(", ");
        }
    }
    printf("] <- BACK\n");
}