#ifndef BINARYSEARCHTREE_H

#define BINARYSEARCHTREE_H

typedef struct BinarySearchTree {
    char *key;  // the key: a string
    void *value; //the value: a generic pointer
    struct BinarySearchTree *left;
    struct BinarySearchTree *right; // the left and right subtrees
};

BinarySearchTree* bst_insert(
    BinarySearchTree *root,
    const char *key,
    void *value
);

void bst_get(BinarySearchTree *root, const char *key);

BinarySearchTree* bst_remove(BinarySearchTree *root, const char *key);

void bst_destroy(BinarySearchTree *root);



#endif