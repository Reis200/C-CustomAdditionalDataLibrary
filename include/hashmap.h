#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdbool.h>

/*
    Generic HashMap

    Representation Invariants and Design Choices:
    
    - Keys are unique according to the caller-supplied equality function.
    - A put with an equal existing key updates or replaces that mapping rather than adding a second logical key.
    - NULL keys are rejected. This keeps NULL available as a failure/absence signal and matches the rest of your library style.
    - Decide separately whether NULL values are rejected. Rejecting them makes get returning NULL unambiguous. Allowing them
      requires status-returning lookup APIs.
    - Lookup returns a borrowed value pointer; removal returns the stored key/value ownership according to a documented policy.
    - The map owns its internal nodes/entries/table storage, not automatically the pointed-to key/value objects.
    - Hash and equality callbacks must remain valid for the entire lifetime of the map.
    
        Universal invariants:
    - size is exactly the number of live key-value mappings.
    - Every live key is non-NULL as NULL keys are forbidden in this implementation.
    - Every live value is non-NULL as NULL values are forbidden in this implementation.
    - No two live entries contain logically equal keys.
    - For every live entry, cached_hash == hashFunc(entry->key), assuming the key has not been mutated.
    - For every pair of logically equal keys, the hash callback returns the same hash.
    - capacity is either zero in the explicitly unallocated state or satisfies the table representation rules.
    - If capacity is a power of two, capacity >= a documented minimum and (capacity & (capacity - 1)) == 0.
    - The load threshold is consistent with capacity and the selected maximum load factor.
    - The structure contains every entry exactly once in the location implied by its hash and collision strategy.

    Thread safety:

    - Thread-safety requirements follow the project-wide policy
      documented in the library README.
*/

/* ============================================================
   Value Policies
   ============================================================ */

// Exact Hash implementation for that user decided data   
typedef uint64_t (*HashMapHashFunc)(const void *key);

// Compare two logical keys.
//
// Convention:
//
//     Hash/equality consistency law
//     if (equals(a, b) == true)
//        then hash(a) MUST equal hash(b)
       
//     The reverse is NOT required:
//     if (hash(a) == hash(b))
//        a and b may still be different keys.
//
// The caller defines what logical equality means for the
// stored type.
//
// The comparison function:
//
//     - must not modify either argument
//     - should behave consistently with the convention
typedef bool (*HashMapKeyCompareFunc)(
    const void *key1,
    const void *key2
);


/* ============================================================
   Representation
   ============================================================ */


typedef struct {
    void *keys;
    void *values;
    size_t capacity;
    size_t size;
} HashMap;


/* ============================================================
   Construction
   ============================================================ */

// Construct a valid empty HashMap.
HashMap *hashmap_create(void);


/* ============================================================
   Insertion
   ============================================================ */

// Updates the HashMap with the specified key-value pairs
// if key value pair already exists only changes the value does not make a duplicate key
bool hashmap_set(HashMap *doublyLinkedList, void *key, void *data);



/* ============================================================
   Access and Lookup
   ============================================================ */

// Returns the value associated with the specified key
const void *hashmap_get_value(const HashMap *hashMap, const void *key);

// Returns a list containing the dictionary's keys
const void *hashmap_keys(const HashMap *hashMap);

// Returns a list of all the values in the dictionary
const void *hashmap_values(const HashMap *hashMap);


bool hashmap_contains_key(const HashMap *hashMap,const void *key,HashMapKeyCompareFunc hashMapKeyCompareFunc);


/* ============================================================
   Removal
   ============================================================ */

// Removes the element with the specified key
void *hashmap_pop(const HashMap *hashMap,void *key,HashMapKeyCompareFunc hashMapKeyCompareFunc);


// Remove the last inserted key value pair
//
// Expected complexity:
//
//     O(n)
void *hashmap_pop_item(const HashMap *hashMap, HashMapKeyCompareFunc hashMapKeyCompareFunc);


/* ============================================================
   Observation
   ============================================================ */

// Return the number of live mappings currently stored.
//
// Returns 0 for a NULL HashMap.
//
// Expected complexity:
//
//     O(1)
size_t hashmap_size(const HashMap *hashMap);


// Return whether the list contains no live mappings.
//     O(1)
bool hashmap_is_empty(const HashMap *hashMap);


/* ============================================================
   Lifetime Management
   ============================================================ */

void hashmap_clear(const HashMap *hashMap, void (*destroyData)(void *data));


void hashmap_destroy(const HashMap *hashMap,void (*destroyData)(void *data));


/* ============================================================
   Debugging
   ============================================================ */

void hashmap_print(const HashMap *hashMap,void (*print_func)(const void *data));


#endif