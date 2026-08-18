#ifndef BINARYSEARCHTREE_H

#define BINARYSEARCHTREE_H

typedef struct BinarySearchTree{
    char *key;  // the key: a string
    void *value; //the value: a generic pointer
    struct BinarySearchTree *left;
    struct BinarySearchTree *right; // the left and right subtrees
} BinarySearchTree;

void bst_printkv(const BinarySearchTree *root);

BinarySearchTree* bst_free_value(BinarySearchTree *root, const char *key);

void* bst_get(const BinarySearchTree *root, const char *key);

BinarySearchTree* bst_remove(BinarySearchTree *root, const char *key);

void bst_destroy(BinarySearchTree *root);



#endif