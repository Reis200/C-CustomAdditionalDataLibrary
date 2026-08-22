#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <stdio.h>

// or binary set tree as no duplicate values allowed in this implementation
typedef struct BinarySearchTreeNode {
  void *data;
  struct BinarySearchTreeNode *left;
  struct BinarySearchTreeNode *right;
}BinarySearchTreeNode;

typedef struct {
  BinarySearchTreeNode *root;
  size_t size; // O(1) access much faster
} BinarySearchTree;

/*
 COMPARISON FUNCTION:
    < 0 if a < b
      0 if a == b
    > 0 if a > b    
*/
typedef int (*BSTCompareFunc)(const void *a, const void *b);


/*Make / insert*/
BinarySearchTree* bst_insert(BinarySearchTree *tree,void *data,BSTCompareFunc compare);

/*Search*/
void* bst_find(const BinarySearchTree *tree,const void *data,BSTCompareFunc compare);

/*Remove*/
void* bst_remove(BinarySearchTree *tree,const void *data,BSTCompareFunc compare);


/*Information*/

// height(null) = 0 / height(leaf) = 1
int bst_height(const BinarySearchTree *tree);

size_t bst_size(const BinarySearchTree *tree);

/*Traversal*/

void bst_inorder(const BinarySearchTree *tree, void (*visit)(const void *data));

void bst_preorder(const BinarySearchTree *tree, void (*visit)(const void *data));

void bst_postorder(const BinarySearchTree *tree, void (*visit)(const void *data));

/*Cleanup*/
void bst_destroy(const BinarySearchTree *tree, void (*destroyData)(void *data));

#endif