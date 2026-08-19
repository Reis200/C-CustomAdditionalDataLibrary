#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

typedef struct BinarySearchTree {
    void *data;

    struct BinarySearchTree *left;
    struct BinarySearchTree *right;
}BinarySearchTree;

/*
 COMPARISON FUNCTION:
    < 0 if a < b
      0 if a == b
    > 0 if a > b    
*/
typedef int (*BSTCompareFunc)(const void *a, const void *b);


/*Make / insert*/
BinarySearchTree* bst_insert(BinarySearchTree *root,void *data,BSTCompareFunc compare);

/*Search*/
void* bst_find(BinarySearchTree *root,const void *data,BSTCompareFunc compare);

/*Remove*/
void* bst_remove(BinarySearchTree *root,const void *data,BSTCompareFunc compare);


/*Information*/
void* bst_size(const BinarySearchTree *root);

void* bst_height(const BinarySearchTree *root);

/*Traversal*/

void bst_inorder(const BinarySearchTree *root, void (*visit)(const void *data));

void bst_preorder(const BinarySearchTree *root, void (*visit)(const void *data));

void bst_postorder(const BinarySearchTree *root, void (*visit)(const void *data));

/*Cleanup*/
void bst_destroy(const BinarySearchTree *root, void (*destroyData)(void *data));

#endif