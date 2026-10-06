#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <stddef.h>
#include <stdbool.h>

/*
    Generic Dynamic Array

    Representation invariants:
    - 0 <= size <= capacity.
    - If capacity == 0, items may be NULL.
    - If capacity > 0, items points to storage for at least
      capacity elements of type void *.
    - Live elements occupy the contiguous range:
          items[0] through items[size - 1]
    - Slots from items[size] through items[capacity - 1]
      are unused storage and are not part of the logical array.
    - Logical element order is identical to physical array order.
*/
typedef struct {
    void **items;      // Backing array of stored pointers.
    size_t capacity;   // Number of pointer slots currently allocated.
    size_t size;       // Number of live elements currently stored.
} DynamicArray;


/* ============================================================
   Construction
   ============================================================ */

// Create a valid empty dynamic array.
//
// Initial state:
//     items    = NULL
//     capacity = 0
//     size     = 0
//
// Returns NULL if allocation of the DynamicArray structure fails.
DynamicArray *array_create(void);


// Create an empty dynamic array with capacity reserved for
// at least reservedSize elements.
//
// The returned array still has size == 0.
//
// For reservedSize == 0 empty dynamicArray returned
//
// cases reserving will overflow as SIZE_MAX is reached, NULL is returned.
//
// Returns NULL if allocation fails.
DynamicArray *array_reserve(size_t reservedSize);


/* ============================================================
   Insertion
   ============================================================ */

// Append data to the end of the dynamic array.
//
// The array stores the pointer itself; the pointed-to object
// is not copied.
//
// Returns:
//     true  -> insertion succeeded
//     false -> invalid arguments or allocation failure
bool array_append(DynamicArray *array, void *data);


// Insert data at the specified logical index.
//
// Valid insertion indexes are in the range [0, size].
// index == size is equivalent to appending.
//
// Existing elements from index onward are shifted one position
// to the right.
//
// The array grows automatically if additional capacity is needed.
//
// Returns:
//     true  -> insertion succeeded
//     false -> invalid index, invalid arguments, or allocation failure
bool array_insert(DynamicArray *array,size_t index,void *data);


/* ============================================================
   Access and Replacement
   ============================================================ */

// Return the element stored at index without removing it.
//
// Returns NULL if:
//     - array is NULL
//     - index is outside the valid range [0, size - 1]
//
// The returned pointer is borrowed and remains owned according
// to the array's existing ownership policy.
const void *array_get(const DynamicArray *array,size_t index);


// Replace the element currently stored at a valid index.
//
// If destroyPreviousData is non-NULL, it is called on the
// previously stored element before replacement.
//
// The new pointer is stored directly; the pointed-to object
// is not copied.
//
// newData is the same pointer at that index (duplicate) then nothing changed.
//
// Returns:
//     true  -> replacement succeeded
//     false -> array is NULL, newData is invalid, or index is invalid
bool array_set(DynamicArray *array,size_t index,void *newData,void (*destroyPreviousData)(void *data));


/* ============================================================
   Removal
   ============================================================ */

// Remove and return the last element.
//
// No shifting is required.
//
// Ownership of the removed pointer is transferred to the caller.
//
// Returns NULL if the array is NULL or empty.
void *array_remove_last(DynamicArray *array);


// Remove and return the first element.
//
// Remaining live elements are shifted one position to the left.
//
// Ownership of the removed pointer is transferred to the caller.
//
// Returns NULL if the array is NULL or empty.
void *array_remove_first(DynamicArray *array);


// Remove and return the element at index.
//
// Elements after index are shifted one position to the left.
//
// Ownership of the removed pointer is transferred to the caller.
//
// Returns NULL if:
//     - array is NULL
//     - index is outside the valid range [0, size - 1]
void *array_remove_at(DynamicArray *array,size_t index);


/* ============================================================
   Observation
   ============================================================ */

// Return the number of live elements currently stored.
//
// Returns 0 for a NULL array.
size_t array_size(const DynamicArray *array);


// Return the number of pointer slots currently allocated.
//
// Returns 0 for a NULL array.
size_t array_capacity(const DynamicArray *array);


// Return true if the array contains no live elements.
//
// A NULL array is treated as empty.
bool array_is_empty(const DynamicArray *array);


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
void array_clear(DynamicArray *array,void (*destroyData)(void *data));


// Destroy the dynamic array.
//
// If destroyData is non-NULL, it is called once for each live
// stored pointer before the backing storage and container are freed.
//
// After this function returns, the caller's DynamicArray pointer
// is invalid and must not be dereferenced.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void array_destroy(DynamicArray *array,void (*destroyData)(void *data));


/* ============================================================
   Debugging
   ============================================================ */

// Print every live element in logical array order from index 0
// through index size - 1.
//
// The caller supplies print_func to define how one stored value
// should be printed.
//
// This function does not modify the array.
void array_print(const DynamicArray *array,void (*print_func)(const void *data));

#endif