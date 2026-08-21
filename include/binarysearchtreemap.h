#ifndef BINARYSEARCHTREEMAP_H

#define BINARYSEARCHTREEMAP_H

#include <stdio.h>

// allows duplicates binary search tree map
typedef struct BinarySearchTreeMap{
    char *key;  // the key: a string
    void *value; //the value: a generic pointer
    struct BinarySearchTreeMap *left;
    struct BinarySearchTreeMap *right; // the left and right subtrees
} BinarySearchTreeMap;


/* Insert / update */
BinarySearchTreeMap *bstmap_insert(BinarySearchTreeMap *root,const char *key,void *value);

/* Lookup */
void *bstmap_get(const BinarySearchTreeMap *root,const char *key);

/* Check for key */
int bstmap_contains(const BinarySearchTreeMap *root,const char *key);

/* Remove entry */
BinarySearchTreeMap *bstmap_remove(BinarySearchTreeMap *root,const char *key);

/* Number of key-value pairs */
size_t bstmap_size(const BinarySearchTreeMap *root);

/* Print key-value pairs */
void bstmap_printkv(const BinarySearchTreeMap *root);

/* Destroy entire map */
void bstmap_destroy(BinarySearchTreeMap *root);

#endif