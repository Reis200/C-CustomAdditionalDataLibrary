#include "multiset.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


/* ============================================================
   Construction
   ============================================================ */

// Construct a valid empty multiset.
//
// Initial state:
//
//     entries              = NULL
//     distinctSize         = 0
//     totalSize            = 0
//     capacity             = 0
//     multisetCompareFunc  = supplied comparison function
//     multisetCloneFunc    = supplied clone function or NULL
//
// multisetComparisonFunc defines logical equality and is required.
//
// multisetComparisonFunc must not be NULL.
//
// multisetCloneFunc is optional and may be NULL.
//
// If multisetCloneFunc != NULL:
//
//     multiset_remove_one() may materialize an independently owned
//     copy when removing one occurrence while other equal occurrences
//     remain.
//
// If multisetCloneFunc == NULL:
//
//     the multiset behaves as a mathematical counting multiset for
//     non-final removals.
//
// Returns:
//
//     non-NULL -> construction succeeded
//
//     NULL     -> construction failed because:
//
//                 - allocation of the Multiset structure failed, or
//                 - multisetCompareFunc was NULL
//
// No partially constructed Multiset is returned on failure.
Multiset *multiset_create(MultisetComparisonFunc multisetCompareFunc,MultisetCloneFunc multisetCloneFunc){
    Multiset *multiset = malloc(sizeof(*multiset));
    if (multiset == NULL){
        return NULL;
    }
    // check whether valid multisetComparisonFunction supplied
    if (multisetCompareFunc == NULL){
        free(multiset);
        return NULL;
    }

    multiset->entries = NULL;
    multiset->distinctSize = 0;
    multiset->totalSize = 0;
    multiset->capacity = 0;
    multiset->multisetCompareFunc = multisetCompareFunc;
    multiset->multisetCloneFunc = multisetCloneFunc;

    return multiset;
}


// Construct an empty multiset with capacity reserved for at least
// reservedSize distinct values.
//
// Capacity refers to distinct values, NOT total occurrences.
//
// Example:
//
//     one value with count == 10,000
//
// still occupies only one MultisetEntry slot.
//
// multisetCompareFunc is required and must not be NULL.
//
// multisetCloneFunc is optional and may be NULL.
//
// If reservedSize == 0:
//
//     a valid empty multiset is returned with:
//
//         entries      == NULL
//         capacity     == 0
//         distinctSize == 0
//         totalSize    == 0
//
// If reservedSize cannot be represented safely as a backing-array
// allocation, construction fails.
//
// Returns:
//
//     non-NULL -> construction and requested reservation succeeded,
//                 or reservedSize was 0
//
//     NULL     -> construction failed because:
//
//                 - multisetCompareFunc was NULL
//                 - the requested allocation size would overflow
//                 - allocation of the Multiset structure failed
//                 - allocation of the backing entries array failed
//
// No partially constructed Multiset is returned on failure.
Multiset *multiset_reserve(size_t reservedSize,MultisetComparisonFunc multisetCompareFunc,MultisetCloneFunc multisetCloneFunc){
   Multiset *multiset = multiset_create(multisetCompareFunc, multisetCloneFunc);
   if (multiset == NULL){
      return NULL;
   }

   if (reservedSize == 0){
      return multiset;
   }

   if (reservedSize > SIZE_MAX / sizeof(*multiset->entries)){
      free(multiset);
      return NULL;
   }

   multiset->entries = malloc(sizeof(*multiset->entries) * reservedSize);
   if (multiset->entries == NULL){
      free(multiset);
      return NULL;
   }

   multiset->capacity = reservedSize;

   return multiset;
}


/* ============================================================
   Insertion
   ============================================================ */

// Add one logical occurrence of data.
//
// If no equal entry currently exists:
//
//     - a new MultisetEntry is created
//     - data becomes its canonical stored pointer
//     - count becomes 1
//     - distinctSize increases by 1
//     - totalSize increases by 1
//
// If an equal entry already exists:
//
//     - no additional MultisetEntry is created
//     - the existing canonical pointer remains stored
//     - count increases by 1
//     - totalSize increases by 1
//     - distinctSize remains unchanged
//
// IMPORTANT:
//
// Equality does not imply pointer identity.
//
// Two different pointers may compare logically equal:
//
//     pointer A -> "hello"
//     pointer B -> "hello"
//
// Ownership:
//
// If data becomes the canonical pointer of a newly created distinct
// entry, the multiset retains that pointer until it is removed,
// cleared, or destroyed.
//
// If data compares equal to an existing canonical value but is a
// different pointer, the multiset does NOT retain or take ownership
// of that redundant pointer. Responsibility for that pointer remains
// with the caller.
//
// Therefore, after:
//
//     multiset_add(multiset, data)
//
// the caller must not assume that every successfully supplied pointer
// has been retained by the multiset.
//
// The clone callback is NOT used during insertion.
//
// It exists to materialize owned occurrences when required by
// removal operations.
//
// Returns:
//
//     true  -> insertion succeeded
//     false -> invalid arguments, allocation failure,
//              multiplicity overflow, or capacity overflow
bool multiset_add(Multiset *multiset,void *data){
   if (multiset == NULL || data == NULL){
      return false;
   }
   if (multiset->entries != NULL){
      for (size_t entryIndex = 0; entryIndex < multiset->distinctSize; entryIndex++){
         if (multiset->multisetCompareFunc(multiset->entries[entryIndex].data, data)){
            if (multiset->totalSize >= SIZE_MAX || multiset->entries[entryIndex].count >= SIZE_MAX){
               return false; // check for [increments could theoretically overflow for absurdly huge arrays]
            }
            multiset->entries[entryIndex].count++;
            multiset->totalSize++;
            return true;
         }
      }
      if (multiset->distinctSize == multiset->capacity){
         // resize and expand the array (doubling * 2 of capacity used for amortised O(1) resizing)
         if (multiset->capacity > SIZE_MAX / 2){
            return false; // check for [allocation-size multiplication could theoretically overflow for absurdly huge arrays]
         }
         size_t newCapacity = multiset->capacity * 2;
         if (newCapacity > SIZE_MAX / sizeof(*multiset->entries)){
            return false;
         }
         MultisetEntry *items = malloc(sizeof(*multiset->entries) * newCapacity);
         if (items == NULL){
            return false;
         }
         multiset->capacity = newCapacity;
         for (size_t i = 0; i < multiset->distinctSize; i++){
            items[i] = multiset->entries[i];
         }
         free(multiset->entries); // free the old allocation
         multiset->entries = items;
      }
   } else{
      // default size of 5 (MultisetEntry slot) is allocated
      size_t defaultCapacity = 5;
      multiset->entries = malloc(sizeof(*multiset->entries) * defaultCapacity);
      if (multiset->entries == NULL){
         return false;
      }
      multiset->capacity = defaultCapacity;
   }
   if (multiset->totalSize >= SIZE_MAX || multiset->distinctSize >= SIZE_MAX){
      return false; // check for [increments could theoretically overflow for absurdly huge arrays]
   }
   multiset->entries[multiset->distinctSize].data = data;
   multiset->entries[multiset->distinctSize].count = 1;
   multiset->distinctSize++;
   multiset->totalSize++;
   return true;
}


/* ============================================================
   Access and Lookup
   ============================================================ */

// Return the multiplicity of data.
//
// Returns:
//
//     0   -> no equal logical value exists
//     > 0 -> number of logical occurrences
//
// The supplied data pointer acts only as a lookup key.
size_t multiset_lookup(const Multiset *multiset,const void *data){
   if (multiset == NULL || data == NULL){
      return 0;
   }
   for (size_t entryIndex = 0; entryIndex < multiset->distinctSize; entryIndex++){
      if (multiset->multisetCompareFunc(multiset->entries[entryIndex].data, data)){
         return multiset->entries[entryIndex].count;
      }
   }
   return 0;
}


// Return whether at least one logically equal value exists.
//
// Equivalent conceptually to:
//
//     multiset_lookup(multiset, data) > 0
bool multiset_contains(const Multiset *multiset,const void *data){
   if (multiset == NULL || data == NULL){
      return false;
   }
   for (size_t entryIndex = 0; entryIndex < multiset->distinctSize; entryIndex++){
      if (multiset->multisetCompareFunc(multiset->entries[entryIndex].data, data)){
         return true;
      }
   }
   return false;
}


/* ============================================================
   Removal
   ============================================================ */

// Remove exactly one logical occurrence.
//
// Three important cases exist:
//
// ------------------------------------------------------------
// Case 1: count == 1
// ------------------------------------------------------------
//
// This is the final occurrence.
//
// The entire distinct entry is removed:
//
//     count disappears
//     distinctSize--
//     totalSize--
//
// The canonical stored pointer itself is returned.
//
// Ownership of that pointer transfers to the caller.
//
// No cloning is necessary because the multiset no longer needs
// a canonical representative for that value.
//
//
// ------------------------------------------------------------
// Case 2: count > 1 and multisetCloneFunc != NULL
// ------------------------------------------------------------
//
// Other equal occurrences must remain represented by the
// canonical stored pointer.
//
// Therefore:
//
//     - the canonical pointer remains stored
//     - an independent clone of the canonical value is created
//     - count--
//     - totalSize--
//     - the clone is returned to the caller
//
// The returned clone represents the removed logical occurrence.
//
// Ownership of the clone belongs to the caller.
//
// From the public API's perspective, the caller has therefore
// removed one occurrence and received an independently owned
// object representing that occurrence.
//
// Internally, however, the multiset still stores only:
//
//     one canonical pointer + remaining multiplicity
//
//
// ------------------------------------------------------------
// Case 3: count > 1 and multisetCloneFunc == NULL
// ------------------------------------------------------------
//
// The multiset behaves as a mathematical counting multiset.
//
// Only multiplicity changes:
//
//     count--
//     totalSize--
//
// The canonical pointer remains stored.
//
// NULL is returned because there is no separately materialized
// object corresponding to the removed occurrence.
//
// The logical removal still succeeds.
//
//
// ------------------------------------------------------------
//
// Example:
//
//     canonical A
//     count = 3
//
// With a clone callback:
//
//     remove one:
//
//         canonical A remains stored
//         count becomes 2
//         clone(A) is returned
//
//     remove one:
//
//         canonical A remains stored
//         count becomes 1
//         clone(A) is returned
//
//     remove final:
//
//         canonical A leaves the multiset
//         canonical A itself is returned
//
//
// Without a clone callback:
//
//     remove one:
//
//         count becomes 2
//         NULL returned
//
//     remove one:
//
//         count becomes 1
//         NULL returned
//
//     remove final:
//
//         canonical A itself is returned
//
//
// IMPORTANT:
//
// If cloning is required but the clone function fails and
// thus returns NULL, the implementation leaves the multiset
// unchanged.
//
// This preserves operation atomicity:
//
//     either cloning succeeds and one occurrence is removed,
//     or nothing changes.
//
// NOTE:
//
// Because NULL can also represent successful mathematical-mode
// removal, absence, or clone failure, callers needing to
// distinguish these outcomes may eventually require a richer
// status-returning API.
void *multiset_remove_one(Multiset *multiset,void *data){
   if (multiset == NULL || data == NULL){
      return NULL;
   }
   for (size_t entryIndex = 0; entryIndex < multiset->distinctSize; entryIndex++){
      if (multiset->multisetCompareFunc(multiset->entries[entryIndex].data, data)){
         if (multiset->entries[entryIndex].count == 1){
            void *temp = multiset->entries[entryIndex].data;
            multiset->entries[entryIndex].data = NULL;
            multiset->entries[entryIndex].count = 0;

            // shifting the backing entries array in order to maintain the invariant: live entries occupy entries[0] through entries[distinctSize - 1]
            for (size_t currentIndex = entryIndex; currentIndex < multiset->distinctSize - 1; currentIndex++){
               multiset->entries[currentIndex] = multiset->entries[currentIndex + 1];
            }
            multiset->distinctSize--;
            multiset->totalSize -= 1;
            return temp;
         } else if (multiset->entries[entryIndex].count > 1 && multiset->multisetCloneFunc != NULL){
            void *clone = multiset->multisetCloneFunc(multiset->entries[entryIndex].data);
            if (clone == NULL){
               return NULL; // failed clone operation NULL returned
            }
            multiset->entries[entryIndex].count--;
            multiset->totalSize--;
            return clone;
         } else if (multiset->entries[entryIndex].count > 1 && multiset->multisetCloneFunc == NULL){
            multiset->entries[entryIndex].count--;
            multiset->totalSize--;
            // here NULL is returned but mathematical removal has succeeded as no cloning possible no value to return
            // it is caller's responsibility to distinguish these (sorry right now before API update this is the representation)
            return NULL;
         } 
      }
   }
   return NULL;
}


// Remove every logical occurrence of data.
//
// The entire distinct entry is removed regardless of count.
//
// If a matching entry exists:
//
//     totalSize decreases by entry.count
//     distinctSize decreases by 1
//
// The canonical stored pointer itself is returned.
//
// No cloning is needed because no occurrence of that logical
// value remains inside the multiset.
//
// Ownership of the returned canonical pointer transfers to
// the caller.
//
// Returns NULL if no matching value exists or the arguments
// are invalid.
void *multiset_remove_all(Multiset *multiset,void *data){
   if (multiset == NULL || data == NULL){
      return NULL;
   }
   for (size_t entryIndex = 0; entryIndex < multiset->distinctSize; entryIndex++){
      if (multiset->multisetCompareFunc(multiset->entries[entryIndex].data, data)){
         multiset->totalSize -= multiset->entries[entryIndex].count;
         void *temp = multiset->entries[entryIndex].data;
         multiset->entries[entryIndex].data = NULL;
         multiset->entries[entryIndex].count = 0;
         // shifting the backing entries array in order to maintain the invariant: live entries occupy entries[0] through entries[distinctSize - 1]
         for (size_t currentIndex = entryIndex; currentIndex < multiset->distinctSize - 1; currentIndex++){
            multiset->entries[currentIndex] = multiset->entries[currentIndex + 1];
         }
         multiset->distinctSize--;
         return temp;
      }
   }
   return NULL;
}


/* ============================================================
   Observation
   ============================================================ */

// Return the total number of logical occurrences.
//
// Example:
//
//     { A, A, A, B, B, C }
//
// returns:
//
//     6
//
// Returns 0 for a NULL multiset.
size_t multiset_totalSize(const Multiset *multiset){
   return (multiset == NULL) ? 0 : multiset->totalSize;
}


// Return the number of distinct logical values.
//
// Example:
//
//     { A, A, A, B, B, C }
//
// returns:
//
//     3
//
// Returns 0 for a NULL multiset.
size_t multiset_distinctSize(const Multiset *multiset){
   return (multiset == NULL) ? 0 : multiset->distinctSize;
}


// Return the number of MultisetEntry slots currently allocated.
//
// Capacity refers to distinct entries, NOT total occurrences.
//
// Returns 0 for a NULL multiset.
size_t multiset_capacity(const Multiset *multiset){
   return (multiset == NULL) ? 0 : multiset->capacity;
}


// Return whether the multiset contains no logical values.
//
// Empty means:
//
//     distinctSize == 0
//     totalSize    == 0
//
// A NULL multiset is treated as empty according to the
// library's NULL-object convention.
bool multiset_is_empty(const Multiset *multiset){
   return (multiset == NULL) ? true : (multiset->totalSize == 0 && multiset->distinctSize == 0);
}


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove every distinct entry while retaining the backing
// allocation and capacity.
//
// destroyData is called once per canonical stored pointer,
// NOT once per logical occurrence.
//
// Example:
//
//     A x100
//     B x50
//
// totalSize    = 150
// distinctSize = 2
//
// destroyData is called exactly twice.
//
// Clones previously returned to callers are independent objects
// and are no longer owned or tracked by the multiset.
//
// After clearing:
//
//     distinctSize = 0
//     totalSize    = 0
//     capacity     = unchanged
//
// Pass NULL for borrowed, static, or stack-allocated data.
void multiset_clear(Multiset *multiset,void (*destroyData)(void *data)){
   if (multiset == NULL || multiset->entries == NULL){
      return;
   }
   // clear the multiset
   for (size_t entryIndex = 0; entryIndex < multiset->distinctSize; entryIndex++){
      if (destroyData != NULL){
         destroyData(multiset->entries[entryIndex].data);
      }
      multiset->entries[entryIndex].data = NULL;
      multiset->entries[entryIndex].count = 0;
   }

   multiset->distinctSize = 0;
   multiset->totalSize = 0;
}


// Destroy the multiset.
//
// destroyData is called exactly once for every remaining
// canonical stored pointer.
//
// Multiplicity does not cause repeated destruction.
//
// The optional clone function itself owns no data and requires
// no destruction.
//
// After this function returns, the caller's Multiset pointer
// is invalid.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void multiset_destroy(Multiset *multiset,void (*destroyData)(void *data)){
   if (multiset == NULL){
      return;
   } else if (multiset->entries == NULL){
      // destroy the whole multiset struct
      free(multiset);
      return;
   }
   // clear the multiset
   for (size_t entryIndex = 0; entryIndex < multiset->distinctSize; entryIndex++){
      if (destroyData != NULL){
         destroyData(multiset->entries[entryIndex].data);
      }
      multiset->entries[entryIndex].data = NULL;
      multiset->entries[entryIndex].count = 0;
   }

   multiset->distinctSize = 0;
   multiset->totalSize = 0;

   // destroy the whole multiset entries
   free(multiset->entries);
   // destroy the whole multiset struct
   free(multiset);
}


/* ============================================================
   Debugging
   ============================================================ */

// Print every distinct logical value together with its
// multiplicity.
//
// Example:
//
//     [A x 3, B x 2, C x 1]
//
// The canonical stored pointer for each distinct entry is passed
// to print_func exactly once.
//
// This function does not expand multiplicities into separate
// physical objects.
//
// This function does not modify the multiset.
void multiset_print(const Multiset *multiset,void (*print_func)(const void *data)){
   if (multiset == NULL || print_func == NULL){
      return;
   }
   printf("[");
   for (size_t index = 0; index < multiset->distinctSize; index++){
      print_func(multiset->entries[index].data);
      printf(" x %zu",multiset->entries[index].count);
      if(index + 1 < multiset->distinctSize){
         printf(", ");
      }
   }
   printf("]");
   printf("\n");
}