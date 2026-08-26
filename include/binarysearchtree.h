#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <stddef.h>
#include <stdbool.h>

/*
  bst_insert(tree, data): tree stores the pointer

bst_find(tree,data,compare): returns borrowed pointer / tree still contains/owns the node

bst_remove(...): node is destroyed, stored data pointer is returned to caller, caller becomes responsible for it

bst_destroy(tree, destroyData): all remaining nodes destroyed, destroyData(data) called for every remaining value
*/

// or binary set tree as no duplicate values allowed in this implementation
// AVL = Adelson-Velsky and Landis tree -> self-balancing binary search tree
typedef struct {
  void *data;
  struct BinarySearchTreeNode *left;
  struct BinarySearchTreeNode *right;
  int height; // root height is 0 then 1,2,3 etc.
}AVLTreeNode;

typedef struct {
  AVLTreeNode *root;
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
// empty tree initialisation
BinarySearchTree* bst_create(void);

// or binary set tree as no duplicate values allowed in this implementation
// true  = inserted
// false = duplicate / failure
bool bst_insert(BinarySearchTree *tree,void *data,BSTCompareFunc compare);

/*Search*/
void* bst_find(const BinarySearchTree *tree,const void *data,BSTCompareFunc compare);

/*Remove*/
// The BST owns the node, but not necessarily the object referenced by node->data.
// bst_remove() frees the node and returns the stored void * to the caller.
void* bst_remove(BinarySearchTree *tree,const void *data,BSTCompareFunc compare);


/*Information*/

// height(null) = 0 / height(leaf) = 1
int bst_height(const BinarySearchTree *tree);

size_t bst_size(const BinarySearchTree *tree);

/*Traversal*/
// visit function is custom action for that unique data type
// tree should not assume that data is printable as an integer or string. Instead, traversal calls a user-supplied function:
// visit(root.data). One caller can print; another can sum values; another can serialise data; another can validate it.
// The traversal algorithm remains unchanged.
void bst_inorder(const BinarySearchTree *tree, void (*visit)(const void *data));

void bst_preorder(const BinarySearchTree *tree, void (*visit)(const void *data));

void bst_postorder(const BinarySearchTree *tree, void (*visit)(const void *data));

/*Cleanup*/
// bst_destroy(tree, destroyData) optionally destroys all values still inside the tree.
void bst_destroy(BinarySearchTree *tree, void (*destroyData)(void *data));

#endif