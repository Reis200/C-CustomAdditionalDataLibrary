#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

/*
    Generic Circular Queue

    Design:
    - items stores references to user-provided data.
    - capacity defines the number of pointer slots currently allocated.
    - size stores the number of live elements and provides O(1) size checks.
    - front_index identifies the physical index of the logical front element.

    Representation invariants:
    - 0 <= size <= capacity.
    - If capacity == 0, items may be NULL.
    - If capacity > 0, items points to storage for at least
      capacity elements of type void *.
    - If size > 0, front_index is always in the range
      [0, capacity - 1].
    - If size == 0, there is no live front element even if
      front_index contains the conventional value 0.
    - Exactly size physical slots belong to the logical queue.
    - Walking size positions from front_index with wrap-around
      yields the elements in FIFO order.
    - Unused slots are not part of the abstract queue and must
      never be returned to the caller.

    Physical index formula:

        physicalIndex =
            (front_index + logicalIndex) % capacity
*/

typedef struct {
    void **items;          // Circular backing array of stored pointers.
    size_t capacity;       // Number of pointer slots currently allocated.
    size_t size;           // Number of live elements currently stored.
    size_t front_index;    // Physical index of the logical front element.
} Queue;


/* ============================================================
   Construction
   ============================================================ */

// Create a valid empty queue.
//
// Initial state:
//     items       = NULL
//     capacity    = 0
//     size        = 0
//     front_index = 0
//
// Returns NULL if allocation of the Queue structure fails.
Queue *queue_create(void);


// Create an empty queue with capacity reserved for at least
// reservedSize elements.
//
// The returned queue still has size == 0.
//
// For reservedSize == 0 empty queue returned
// cases reserving will overflow as SIZE_MAX is reached, NULL is returned.
//
// Returns NULL if allocation fails.
Queue *queue_reserve(size_t reservedSize);


/* ============================================================
   Insertion
   ============================================================ */

// Append data to the logical rear of the queue.
//
// The queue stores the pointer itself; the pointed-to object
// is not copied.
//
// The backing array grows automatically if necessary.
//
// Returns:
//     true  -> insertion succeeded
//     false -> invalid arguments or allocation failure
bool queue_enqueue(Queue *queue,void *data);


/* ============================================================
   Access
   ============================================================ */

// Return the current logical front element without removing it.
//
// The returned pointer is borrowed and remains owned according
// to the queue's existing ownership policy.
//
// Returns NULL if the queue is NULL or empty.
const void *queue_peek(const Queue *queue);


/* ============================================================
   Removal
   ============================================================ */

// Remove and return the current logical front element.
//
// Ownership of the removed pointer is transferred to the caller.
//
// Returns NULL if the queue is NULL or empty.
void *queue_dequeue(Queue *queue);


/* ============================================================
   Observation
   ============================================================ */

// Return the number of live elements currently stored.
//
// Returns 0 for a NULL queue.
size_t queue_size(const Queue *queue);


// Return the number of pointer slots currently allocated.
//
// Returns 0 for a NULL queue.
size_t queue_capacity(const Queue *queue);


// Return true if the queue contains no live elements.
//
// A NULL queue is treated as empty.
bool queue_is_empty(const Queue *queue);


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove all live elements while retaining the backing allocation
// and current capacity.
//
// If destroyData is non-NULL, it is called once for each live
// stored pointer.
//
// Pass NULL for borrowed, static, or stack-allocated data.
//
// After clearing:
//     size        = 0
//     front_index = 0
//     capacity    = unchanged
//     items       = unchanged
void queue_clear(Queue *queue,void (*destroyData)(void *data));


// Destroy the queue.
//
// If destroyData is non-NULL, it is called once for each live
// stored pointer before the backing array and Queue structure
// are freed.
//
// After this function returns, the caller's Queue pointer is
// invalid and must not be dereferenced.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void queue_destroy(Queue *queue,void (*destroyData)(void *data));


/* ============================================================
   Debugging
   ============================================================ */

// Print every live element in logical FIFO order.
//
// Elements are printed from the logical front to the logical rear.
//
// Example:
//
//     FRONT -> [A, B, C, D] <- BACK
//
// The caller supplies print_func to define how one stored value
// should be printed.
//
// This function does not modify the queue.
void queue_print(const Queue *queue,void (*print_func)(const void *data));

#endif