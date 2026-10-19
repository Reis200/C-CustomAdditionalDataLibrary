#include "hashmap.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// #define c1 = 0x87c37b91114253d5
// #define c2 = 0x4cf5ad432745937f
// #define fmix multiplier A = 0xff51afd7ed558ccd
// #define fmix multiplier B = 0xc4ceb9fe1a85ec53

// One bucket / hashmap entry slot (one way to handle collisions)
typedef struct {
   uint64_t hash;
   void *key;
   void *value;
} HashMapEntry;

struct HashMap{
    HashMapEntry **entries;
    size_t capacity;
    size_t size;
    HashMapHashFunc hashFunc;
    HashMapKeyCompareFunc compareFunc;
};
// Collision resolution
// use linear probbing or Robin hood excellent cache locality + good for this version of the library 

// rotate left by r bits for hashing utility
uint64_t rotl64(uint64_t x, int r){
    return (x << r | (x >> (-r & 63)));
} 

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
// -> Keep old key, replace value
bool hashmap_put(HashMap *doublyLinkedList, void *key, void *data);



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
size_t hashmap_size(const HashMap *hashMap){
    return (hashMap == NULL || hashMap->size == 0) ? 0 : hashMap->size; 
}


// Return whether the list contains no live mappings.
// A NULL hashMap is treated as empty according to the
// library-wide NULL-object convention.
//     O(1)
bool hashmap_is_empty(const HashMap *hashMap){
    return (hashMap == NULL || hashMap->size == 0) ? true : false; 
}


/* ============================================================
   Lifetime Management
   ============================================================ */

void hashmap_clear(const HashMap *hashMap, void (*destroyData)(void *data));


void hashmap_destroy(const HashMap *hashMap,void (*destroyData)(void *data));


/* ============================================================
   Debugging
   ============================================================ */

void hashmap_print(const HashMap *hashMap,void (*print_func)(const void *data));


