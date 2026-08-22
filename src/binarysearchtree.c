#include "binarysearchtree.h"

#include <stdio.h>


/*Make / insert*/
BinarySearchTree* bst_insert(BinarySearchTree *tree,void *data,BSTCompareFunc compare){
    if (tree == NULL || compare == NULL){
        return NULL;
    }
    if (tree->root == NULL){
        BinarySearchTreeNode *newNode = malloc(sizeof(*newNode));
        if (newNode == NULL){
            return NULL;
        }
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
        tree->root = newNode;
        tree->size++;
        return tree;
    }

    BinarySearchTreeNode *currentRoot = tree->root;
    BinarySearchTreeNode *parentRoot = NULL;
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
            return tree; 
        }
    }
    BinarySearchTreeNode *newNode = malloc(sizeof(*newNode));

    if (newNode == NULL){
        return NULL;
    }

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    if (comparison < 0){
        // assign it to left tree
        parentRoot->left = newNode;
    } else{
        // assign it to right tree
        parentRoot->right = newNode;
    }

    tree->size++;
    return tree;
}

/*Search*/
void* bst_find(const BinarySearchTree *tree,const void *data,BSTCompareFunc compare){
    if (tree == NULL || compare == NULL){
        return NULL;
    }
    const BinarySearchTreeNode *currentNode = tree->root;
    while (currentNode != NULL){
        int comparison = compare(data,currentNode->data);
        if (comparison < 0){
            // if less than 0 go to left tree
            currentNode = currentNode->left;
        } else if (comparison > 0){
            // if greater than 0 go to right tree
            currentNode = currentNode->right;
        }else if (comparison == 0){
            // found the value
            return currentNode->data; 
        }
    }
    // if not found return NULL
    return NULL;
}

/*Remove*/
void* bst_remove(BinarySearchTree *tree,const void *data,BSTCompareFunc compare){
}


/*Information*/
static inline int bst_max_int(const int first,const int second){
    return (first > second) ? first : second;
}
static int bst_node_height(const BinarySearchTreeNode *node){
    if (node == NULL){
        return 0;
    }
    return 1 + bst_max_int(bst_node_height(node->left),bst_node_height(node->right));
}
// height(null) = 0 / height(leaf) = 1
int bst_height(const BinarySearchTree *tree){
    if (tree == NULL){
        return 0;
    }
    return bst_node_height(tree->root);
}
size_t bst_size(const BinarySearchTree *tree){
    return (tree == NULL) ? 0 : tree->size;
}

/*Traversal*/

void bst_inorder(const BinarySearchTree *tree, void (*visit)(const void *data));

void bst_preorder(const BinarySearchTree *tree, void (*visit)(const void *data));

void bst_postorder(const BinarySearchTree *tree, void (*visit)(const void *data));

/*Cleanup*/
void bst_destroy(const BinarySearchTree *tree, void (*destroyData)(void *data));
