#include "hashmap.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define defaultHashMapCapacity 10

// #define loadFactorMax 0.75
// // float loadFactor = hashMap->size / hashMap->capacity
/*
For chaining, load factor is the average number of entries per bucket. For open addressing it is the fraction of occupied slots,
and performance becomes much more sensitive as the table approaches full. A default maximum of 0.75 is a sound starting
point for chaining and a reasonable educational starting point for many open-addressed designs, though some strategies tune
differently.
*/

// #define c1 0x87c37b91114253d5
// #define c2 0x4cf5ad432745937f
// #define fmix_multiplier_A 0xff51afd7ed558ccd
// #define fmix_multiplier_B 0xc4ceb9fe1a85ec53



// One hashmap entry item 
// typedef struct HashMapEntry{
//    uint64_t hash;
//    void *key;
//    void *value;
//    HashMapEntry *next;
// } HashMapEntry;

// struct HashMap{
//     HashMapEntry **buckets; // each bucket may contain 1 or more HashMapEntry linked through next
//     size_t capacity; // total allocatable number of hash values in all buckets
//     size_t size; // total number of hash values in all buckets
//     size_t resizeThreshold; // can be derived using capacity/4
//     HashMapHashFunc hashFunc;
//     HashMapKeyCompareFunc compareFunc;
// };
// // Collision resolution
// // use linear probbing or Robin hood excellent cache locality + good for this version of the library 

// /* ============================================================
//    Helpers
//    ============================================================ */

// // true success / false failure
// bool hash_allocate(HashMap *hashMap, size_t reservedSize){
//    if (hashMap == NULL || reservedSize == 0){
//       return false;
//    }
//    if (reservedSize > SIZE_MAX / sizeof(*hashMap->buckets)){
//       return false;
//    }
//    hashMap->buckets = malloc(reservedSize * sizeof(*hashMap->buckets));

//    if (hashMap->buckets == NULL){
//       return false;
//    }

//    hashMap->capacity = reservedSize;

//    hashMap->resizeThreshold = hashMap->capacity/4;
//    return true;
// }

// // Used for hashing built-in common types...
// // The caller can wrap these helpers in the HashMapHashFunc signature.
// // Also provide default hashing so API implements hashing for default types but user needs to implement for their self defined types
// uint64_t hash_bytes(void *data, size_t length, int seed);

// uint64_t hash_cstring(char *str, int seed); 

// uint64_t hash_int(int *integer, int seed); 

// uint64_t hash_uint64(uint64_t value);

// uint64_t hash_pointer_identity(void *ptr);

// // rotate left by r bits for hashing utility
// uint64_t rotl64(uint64_t x, int r){
//     return (x << r | (x >> (-r & 63)));
// } 

// /* ============================================================
//    Construction
//    ============================================================ */

// // Construct a valid empty HashMap.
// //
// // Initial state:
// //
// //    HashMapEntry **entries = NULL;
// //    size_t capacity = 0;
// //    size_t size = 0;
// //    HashMapHashFunc hashFunc = hashFunc; !! are obligatory
// //    HashMapKeyCompareFunc compareFunc = compareFunc; !! are obligatory
// //
// // Returns:
// //
// //     non-NULL -> construction succeeded !! also validated correctly
// //     NULL     -> allocation failed
// HashMap *hashmap_create(HashMapHashFunc hashFunc, HashMapKeyCompareFunc compareFunc){
//    if (hashFunc == NULL || compareFunc == NULL){
//       return NULL;
//    }
//    HashMap *hashMap = malloc(sizeof(*hashMap));
//    if (hashMap == NULL){
//       return NULL;
//    }
//    hashMap->buckets = NULL;
//    hashMap->capacity = 0;
//    hashMap->size = 0;
//    hashMap->resizeThreshold = 0;
//    hashMap->hashFunc = hashFunc;
//    hashMap->compareFunc = compareFunc;
// }


// /* ============================================================
//    Insertion
//    ============================================================ */

// // Updates the HashMap with the specified key-value pairs
// // if key value pair already exists only changes the value does not make a duplicate key
// // -> Keep old key, replace value
// bool hashmap_put(HashMap *hashMap, void *key, void *data){
//    // if (hashMap == NULL || key == NULL || data == NULL){
//    //    return false;
//    // }
//    // bool result = false;
//    // HashMapEntry **temp;
//    // if (hashMap->size >= hashMap->resizeThreshold){
//    //    if (hashMap->capacity == 0){
//    //       result = hash_allocate(hashMap, defaultHashMapCapacity);
//    //    } else if (hash_allocate(hashMap, hashMap->capacity * 2) == false){
//    //       return false;
//    //    } else{ // resize successful now transfer values
//    //       // come back to this
//    //    }
//    // }
//    // HashMapEntry *newEntry = malloc(sizeof(*newEntry));
//    // if (newEntry == NULL){
//    //    return false;
//    // }
//    // uint64_t hashValue = hashMap->hashFunc(key);
//    // size_t bucketIndex = hashValue % hashMap->capacity;
//    // // if (hashMap->buckets[bucketIndex] == NULL){

//    // // } else if (){

//    // // }
// }



// /* ============================================================
//    Access and Lookup
//    ============================================================ */

// // Returns the value associated with the specified key
// const void *hashmap_lookup(const HashMap *hashMap, const void *key){
//    // if (hashMap == NULL || key == NULL){
//    //    return NULL;
//    // }
//    // HashMapEntry *current;
//    // size_t bucketIndex = hashMap->hashFunc(key) % hashMap->capacity;
//    // if (hashMap->buckets[bucketIndex] != NULL){
//    //    current = hashMap->buckets[bucketIndex];
//    //    while (hashMap->buckets[bucketIndex]->key != key && current->next != NULL){
//    //       current = current->next;
//    //    }
//    //    if (current->key == key){
//    //       // return as found
//    //       return current->value;
//    //    } else{
//    //       // not found within the current bucket chain of entries
//    //       return NULL;
//    //    }
//    // }
// }

// // Returns a list containing the dictionary's keys
// const void *hashmap_keys(const HashMap *hashMap){
//    // if (hashMap == NULL){
//    //    return NULL;
//    // }
//    // void *all_keys = malloc(sizeof(*all_keys) * hashMap->size);
//    // size_t keyIndex = 0;
//    // // for (size_t currentBucketIndex = 0; currentBucketIndex < hashMap->size; currentBucketIndex++){
//    // //    all_keys = hashMap->buckets[currentBucketIndex]->key;
//    // //    all_keys 
//    // // } 
// }

// // Returns a list of all the values in the dictionary
// const void *hashmap_values(const HashMap *hashMap);


// bool hashmap_contains_key(const HashMap *hashMap,const void *key,HashMapKeyCompareFunc hashMapKeyCompareFunc);


// /* ============================================================
//    Removal
//    ============================================================ */

// // Removes the element with the specified key
// void *hashmap_pop(const HashMap *hashMap,void *key,HashMapKeyCompareFunc hashMapKeyCompareFunc);


// // Remove the last inserted key value pair
// //
// // Expected complexity:
// //
// //     O(n)
// void *hashmap_pop_item(const HashMap *hashMap, HashMapKeyCompareFunc hashMapKeyCompareFunc);


// /* ============================================================
//    Observation
//    ============================================================ */

// // Return the number of live mappings currently stored.
// //
// // Returns 0 for a NULL HashMap.
// //
// // Expected complexity:
// //
// //     O(1)
// size_t hashmap_size(const HashMap *hashMap){
//     return (hashMap == NULL || hashMap->size == 0) ? 0 : hashMap->size; 
// }


// // Return whether the list contains no live mappings.
// // A NULL hashMap is treated as empty according to the
// // library-wide NULL-object convention.
// //     O(1)
// bool hashmap_is_empty(const HashMap *hashMap){
//     return (hashMap == NULL || hashMap->size == 0) ? true : false; 
// }


// /* ============================================================
//    Lifetime Management
//    ============================================================ */

// void hashmap_clear(const HashMap *hashMap, void (*destroyData)(void *data));


// void hashmap_destroy(const HashMap *hashMap,void (*destroyData)(void *data));


// /* ============================================================
//    Debugging
//    ============================================================ */

// void hashmap_print(const HashMap *hashMap,void (*print_func)(const void *data));


