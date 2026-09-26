#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

/*An attempt to implement efficient circular queues...*/
/*
Why for the design choice:
items -> Stores references to user-ownedvalues.
capacity -> Defines the valid physical index range and when growth is required.
size -> Makes emptiness/fullness unambiguous and provides O(1) size queries.
front -> Allows dequeue to advance without shifting elements.
*/
/*
 Representation invariants
 (self note size_t physicalIndex = (queue->front_index + index) % queue->capacity; !!! )
- 0 <= size <= capacity whenever the queue has allocated storage.
- If capacity is non-zero, front is always a valid physical index from 0 to capacity - 1.
- If size is zero, there is no live front element even if the numeric front field contains a
conventional value such as zero.
- Exactly size array slots belong to the logical queue sequence.
- Walking size positions from front with wrap-around yields elements in FIFO order.
- Unused slots are not part of the queue and must never be returned to the caller.
*/

typedef struct {
    void **items;
    size_t capacity; // Number of pointer slots (in memory) currently allocated.
    size_t size; // Number of live elements currently in the queue. + provides O(1) size queries.
    size_t front_index; // Physical index containing the logical front when size is non-zero (size > 0).
} Queue;

// Produce a valid empty queue value.
// make an empty Queue type with NULL items, capacity = 0, size = 0, front_index = 0.
// also allocates a memory on heap.
Queue* queue_create(void);

// Append one data pointer to the logical rear of the given queue.
// => enqueue given data.
// true -> if successful
// false -> if unable to do the operation or error occurred.
bool queue_enqueue(Queue *queue,void *data);

// Observe the current front without removing it from the given queue.
// => peek given data.
const void* queue_peek(Queue *queue);

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
void queue_printQueue(const Queue *queue, void (*print_func)(const void *));

#endif