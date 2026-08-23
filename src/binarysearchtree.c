#include "binarysearchtree.h"

#include <stdlib.h>

/*Make / insert*/
// empty tree initialisation
BinarySearchTree* bst_create(void)
{
    return calloc(1, sizeof(BinarySearchTree));
}

// true  = inserted
// false = duplicate / failure
bool bst_insert(BinarySearchTree *tree,void *data,BSTCompareFunc compare){
    if (tree == NULL || compare == NULL || data == NULL){
        return false;
    }
    if (tree->root == NULL){
        BinarySearchTreeNode *newNode = malloc(sizeof(*newNode));
        if (newNode == NULL){
            return false;
        }
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
        tree->root = newNode;
        tree->size++;
        return true;
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
            return false; 
        }
    }
    BinarySearchTreeNode *newNode = malloc(sizeof(*newNode));

    if (newNode == NULL){
        return false;
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
    return true;
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
/*
 COMPARISON FUNCTION:
    < 0 if a < b
      0 if a == b
    > 0 if a > b    
*/
void* bst_remove(BinarySearchTree *tree,const void *data,BSTCompareFunc compare){
    if (tree == NULL || tree->root == NULL || compare == NULL){
        return NULL;
    }
    BinarySearchTreeNode *currentRoot = tree->root;
    BinarySearchTreeNode *parentRoot = NULL;
    /*Find Node*/
    while (currentRoot != NULL){
        int comparison = compare(data,currentRoot->data);
        if (comparison < 0){
            // check left subtree
            parentRoot = currentRoot;
            currentRoot = currentRoot->left;
        } else if (comparison > 0){
            // check right subtree
            parentRoot = currentRoot;
            currentRoot = currentRoot->right;
        }else {
            // remove if equal
            break;
        }
    }
    // if data is not present at all.
    if (currentRoot == NULL){
        return NULL;
    }    
    /*Ownership data returned to the caller*/
    void *removedData = currentRoot->data;
    
    //the in-order successor: smallest element in the right subtree (self-note)
    /* Case Two children: 
       move the inorder successor's data into the current,
       then remove the successor node instead.
    */
    if (currentRoot->left != NULL && currentRoot->right != NULL){ // if both of child is NULL case
        BinarySearchTreeNode *successorParent = currentRoot;
        BinarySearchTreeNode *successor = currentRoot->right;

        while (successor->left != NULL){
            successorParent = successor;
            successor = successor->left;
        }

        currentRoot->data = successor->data;
        parentRoot = successorParent;
        currentRoot = successor;
    }
    /*
        At this point current node has at most one child.
    */
    BinarySearchTreeNode *childNode = (currentRoot->left != NULL) ? currentRoot->left : currentRoot->right; 

    if (parentRoot == NULL){ // remove the root itself
        tree->root = childNode;
    } else if (parentRoot->left == currentRoot){ // set childNode to parent left if currentNode is that
        parentRoot->left = childNode;
    } else { // set childNode to parent right if currentNode is that
        parentRoot->right = childNode;
    }
    

    free(currentRoot);
    tree->size--;
    return removedData;
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
// visit function is custom action for that unique data type
// tree should not assume that data is printable as an integer or string. Instead, traversal calls a user-supplied function:
// visit(root.data). One caller can print; another can sum values; another can serialise data; another can validate it.
// The traversal algorithm remains unchanged.

static void bst_node_inorder(const BinarySearchTreeNode *root, void (*visit)(const void *data)){
    if (root == NULL){
        return;
    }
    bst_node_inorder(root->left,visit);
    if (visit != NULL){
        visit(root->data);
    }
    bst_node_inorder(root->right, visit);
}
void bst_inorder(const BinarySearchTree *tree, void (*visit)(const void *data)){
    if (tree == NULL || visit == NULL){
        return;
    }
    bst_node_inorder(tree->root,visit);
}

static void bst_node_preorder(const BinarySearchTreeNode *root, void (*visit)(const void *data)){
    if (root == NULL){
        return;
    }
    if (visit != NULL){
        visit(root->data);
    }
    bst_node_preorder(root->left,visit);
    bst_node_preorder(root->right,visit);
}
void bst_preorder(const BinarySearchTree *tree, void (*visit)(const void *data)){
    if (tree == NULL){
        return;
    }
    bst_node_preorder(tree->root,visit);
}

static void bst_node_postorder(const BinarySearchTreeNode *root, void (*visit)(const void *data)){
    if (root == NULL){
        return;
    }
    bst_node_postorder(root->left,visit);
    bst_node_postorder(root->right,visit);
    if (visit != NULL){
        visit(root->data);
    }
}
void bst_postorder(const BinarySearchTree *tree, void (*visit)(const void *data)){
    if (tree == NULL){
        return;
    }
    bst_node_postorder(tree->root,visit);
}

/*Cleanup*/
static void bst_node_destroy(BinarySearchTreeNode *root, void (*destroyData)(void *data)){
    if (root == NULL){
        return;
    }
    bst_node_destroy(root->left, destroyData);
    bst_node_destroy(root->right, destroyData);
    if (destroyData != NULL){
        destroyData(root->data);
    }
    free(root);
}

void bst_destroy(BinarySearchTree *tree, void (*destroyData)(void *data)){
    if (tree == NULL){
        return;
    }
    bst_node_destroy(tree->root,destroyData);
    free(tree);
}