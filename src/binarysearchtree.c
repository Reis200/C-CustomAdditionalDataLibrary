#include "binarysearchtree.h"

#include <stdio.h>

/*Make / insert*/
BinarySearchTree* bst_insert(BinarySearchTree *root,void *data,BSTCompareFunc compare){
    if (root == NULL){
        BinarySearchTree *newRoot = malloc(sizeof(BinarySearchTree));
        if (newRoot == NULL){
            return NULL;
        }
        newRoot->data = data;
        newRoot->left = NULL;
        newRoot->right = NULL;
        return newRoot;
    }

    BinarySearchTree *currentRoot = root;
    BinarySearchTree *parentRoot = NULL;
    int comparison = 0;
    while (currentRoot != NULL){
        parentRoot = currentRoot;
        comparison = compare(data,currentRoot->data);
        if (comparison < 0){
            // values less than the current root data are shifted towards left
            currentRoot = currentRoot->left;
        } else if (comparison > 0){
            // values greater than the current root data are shifted towards right
            currentRoot = currentRoot->right;
        }else {
            // duplicate policy: no duplicate allowed (set like tree) 
            break; 
        }
    }
    BinarySearchTree *newRoot = malloc(sizeof(BinarySearchTree));

    if (newRoot == NULL){
        return NULL;
    }

    newRoot->data = data;
    newRoot->left = NULL;
    newRoot->right = NULL;

    if (comparison < 0){
        // assign it to left tree
        parentRoot->left = newRoot;
    } else{
        // assign it to right tree
        parentRoot->right = newRoot;
    }

    return root;
}

/*Search*/
void* bst_find(const BinarySearchTree *root,const void *data,BSTCompareFunc compare){
    const BinarySearchTree *currentRoot = root;
    while (currentRoot != NULL){
        int comparison = compare(data,currentRoot->data);
        if (comparison < 0){
            // if less than 0 go to left tree
            currentRoot = currentRoot->left;
        } else if (comparison > 0){
            // if greater than 0 go to right tree
            currentRoot = currentRoot->right;
        }else if (comparison == 0){
            // found the value
            return currentRoot->data; 
        }
    }
    // if not found return NULL
    return NULL;
}

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
