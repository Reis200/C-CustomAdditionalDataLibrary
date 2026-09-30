#include "dynamicarray.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

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
DynamicArray *array_create(void){
   DynamicArray *dynamicArray = malloc(sizeof(*dynamicArray));
   if (dynamicArray == NULL){
      return NULL;
   }
   dynamicArray->size = 0;
   dynamicArray->capacity = 0;
   dynamicArray->items = NULL;
   return dynamicArray;
}


// Create an empty dynamic array with capacity reserved for
// at least reservedSize elements.
//
// The returned array still has size == 0.
//
// Returns NULL if allocation fails.
DynamicArray *array_reserve(size_t reservedSize){
   DynamicArray *dynamicArray = array_create();

   if (dynamicArray == NULL){
      return NULL;
   }

   if (reservedSize == 0 || reservedSize > SIZE_MAX / sizeof(*dynamicArray->items)){
      return dynamicArray;
   }

   dynamicArray->items = malloc(sizeof(*dynamicArray->items) * reservedSize);

   if (dynamicArray->items == NULL){
      free(dynamicArray);
      return NULL;
   }

   dynamicArray->capacity = reservedSize;

   return dynamicArray;
}


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
bool array_append(DynamicArray *array, void *data){
   if (array == NULL || data == NULL){
      return false;
   }
   if (array->capacity == 0 && array->items == NULL){
      // as items not initialised (it is initialised with default size 5)
      size_t newCapacity = 5;
      array->items = malloc(sizeof(*array->items) * newCapacity);
      if (array->items == NULL){
         return false;
      }
      array->capacity = newCapacity;
   } else if (array->size == array->capacity && array->items != NULL){
      // resize and expand the array (doubling * 2 of capacity used for amortised O(1) resizing)
      if (array->capacity > SIZE_MAX / 2){
         return false; // check for [allocation-size multiplication could theoretically overflow for absurdly huge arrays]
      }
      size_t newCapacity = array->capacity * 2;
      void **items = malloc(sizeof(*array->items) * newCapacity);
      if (items == NULL){
         return false;
      }
      array->capacity = newCapacity;
      for (size_t i = 0; i < array->size; i++){
         items[i] = array->items[i];
      }
      free(array->items); // free the old allocation
      array->items = items;
   }
   array->items[array->size] = data;
   array->size++;
   return true;
}


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
bool array_insert(DynamicArray *array,size_t index,void *data){
   if (array == NULL || index > array->size || data == NULL){
      return false;
   }
   if (array->capacity == 0 && array->items == NULL){
      // as items not initialised (initialised with default capacity 5)
      size_t newCapacity = 5;
      array->items = malloc(sizeof(*array->items) * newCapacity);
      if (array->items == NULL){
         return false;
      }
      array->capacity = newCapacity;
   } else if (array->size == array->capacity && array->items != NULL){
      // resize and expand the array (doubling * 2 of capacity used for amortised O(1) resizing)
      if (array->capacity > SIZE_MAX / 2){
         return false; // check for [allocation-size multiplication could theoretically overflow for absurdly huge arrays]
      }
      size_t newCapacity = array->capacity * 2;
      void **items = malloc(sizeof(*array->items) * newCapacity);
      if (items == NULL){
         return false;
      }
      array->capacity = newCapacity;
      for (size_t i = 0; i < array->size; i++){
         items[i] = array->items[i];
      }
      free(array->items); // free the old allocation
      array->items = items;
   } 
   if (array->size < array->capacity && array->items != NULL){
      // shifting elements to the right until and including the element at the insertedIndex
      for (size_t i = array->size; i > index; i--){
         array->items[i] = array->items[i - 1];
      }
   }

   array->items[index] = data;
   array->size++;
   return true;
}


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
const void *array_get(const DynamicArray *array,size_t index){
   if (array == NULL || index >= array->size){
      return NULL;
   }
   return array->items[index];
}


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
bool array_set(DynamicArray *array,size_t index,void *newData,void (*destroyPreviousData)(void *data)){
   if (array == NULL || index >= array->size || newData == NULL){
      return false;
   }
   if (newData == array->items[index]){
      return true; // nothing changed
   }
   if (destroyPreviousData != NULL){
      destroyPreviousData(array->items[index]);
   }
   array->items[index] = NULL;
   array->items[index] = newData;
   return true;
}


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
void *array_remove_last(DynamicArray *array){
   if (array == NULL || array->size == 0){
      return NULL;
   }

   void *data = array->items[array->size-1];
   array->items[array->size-1] = NULL;
   array->size--;
   return data;
}


// Remove and return the first element.
//
// Remaining live elements are shifted one position to the left.
//
// Ownership of the removed pointer is transferred to the caller.
//
// Returns NULL if the array is NULL or empty.
void *array_remove_first(DynamicArray *array){
   if (array == NULL || array->size == 0){
      return NULL;
   }

   void *data = array->items[0];
   array->items[0] = NULL;
   for (size_t index = 0; index < array->size - 1; index++){
      array->items[index] = array->items[index + 1];
   } 
   array->items[array->size-1] = NULL;
   array->size--;
   return data;
}


// Remove and return the element at index.
//
// Elements after index are shifted one position to the left.
//
// Ownership of the removed pointer is transferred to the caller.
//
// Returns NULL if:
//     - array is NULL
//     - index is outside the valid range [0, size - 1]
void *array_remove_at(DynamicArray *array,size_t index){
   if (array == NULL || array->size == 0 || index >= array->size){
      return NULL;
   }

   void *data = array->items[index];
   array->items[index] = NULL;
   for (size_t currentIndex = index; currentIndex < array->size - 1; currentIndex++){
      array->items[currentIndex] = array->items[currentIndex + 1];
   }
   array->items[array->size-1] = NULL;
   array->size--;
   return data;
}


/* ============================================================
   Observation
   ============================================================ */

// Return the number of live elements currently stored.
//
// Returns 0 for a NULL array.
size_t array_size(const DynamicArray *array){
   return (array == NULL) ? 0 : array->size;
}


// Return the number of pointer slots currently allocated.
//
// Returns 0 for a NULL array.
size_t array_capacity(const DynamicArray *array){
   return (array == NULL) ? 0 : array->capacity;
}


// Return true if the array contains no live elements.
//
// A NULL array is treated as empty.
bool array_is_empty(const DynamicArray *array){
   return (array == NULL || array->size == 0) ? true : false;
}


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
void array_clear(DynamicArray *array,void (*destroyData)(void *data)){
   if (array == NULL || array->size == 0){
      return; // don't need to clear empty array
   }
   for (size_t index = 0; index < array->size; index++){
      if (destroyData != NULL){
         destroyData(array->items[index]);
      }
      array->items[index] = NULL;
   }
   // do not free the array as capacity allocated needs to remain
   array->size = 0;
}


// Destroy the dynamic array.
//
// If destroyData is non-NULL, it is called once for each live
// stored pointer before the backing storage and container are freed.
//
// After this function returns, the caller's DynamicArray pointer
// is invalid and must not be dereferenced.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void array_destroy(DynamicArray *array,void (*destroyData)(void *data)){
   if (array == NULL){
      return;
   }
   // destroy the elements
   for (size_t index = 0; index < array->size; index++){
      if (destroyData != NULL){
         destroyData(array->items[index]);
      }
      array->items[index] = NULL;
   }
   // now free the array
   free(array->items);
   array->size = 0;
   array->capacity = 0;
   // now free the array
   free(array);
}


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
void array_print(const DynamicArray *array,void (*print_func)(const void *data)){
   if (array == NULL || print_func == NULL){
      return;
   }
   printf("[");
   for (size_t index = 0; index < array->size; index++){
      print_func(array->items[index]);
      if(index + 1 < array->size){
         printf(", ");
      }
   }
   printf("]");
   printf("\n");
}