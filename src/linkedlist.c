#include "linkedlist.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


/* ============================================================
   Construction
   ============================================================ */

// Construct a valid empty singly linked list.
//
// Initial state:
//
//     head = NULL
//     tail = NULL
//     size = 0
//
// Returns:
//
//     non-NULL -> construction succeeded
//     NULL     -> allocation failed
SinglyLinkedList *singlyLinkedList_create(void){
    SinglyLinkedList *singlyLinkedList = malloc(sizeof(*singlyLinkedList));

    if (singlyLinkedList == NULL){
        return NULL;
    }

    singlyLinkedList->head = NULL;
    singlyLinkedList->tail = NULL;
    singlyLinkedList->size = 0;

    return singlyLinkedList;
}


/* ============================================================
   Insertion
   ============================================================ */

// Insert data at the beginning of the list.
//
// If the list is empty:
//
//     the new node becomes both head and tail.
//
// Otherwise:
//
//     the new node becomes head and links to the previous head.
//
// The supplied data pointer is stored directly.
// The pointed-to object is not copied.
//
// NULL data is invalid.
//
// On success:
//
//     size increases by 1.
//
// Returns:
//
//     true  -> insertion succeeded
//     false -> invalid arguments, allocation failure,
//              or size overflow
//
// Expected complexity:
//
//     O(1)
bool singlyLinkedList_insert_front(SinglyLinkedList *singlyLinkedList,void *data){
    if (singlyLinkedList == NULL || data == NULL){
        return false;
    }
    LinkedListNode *node = malloc(sizeof(*node));
    if (node == NULL){
        return false;
    }
    if (singlyLinkedList->size == 0){
        node->data = data;
        node->next = NULL;
        singlyLinkedList->head = node;
        singlyLinkedList->tail = node;
    } else if (singlyLinkedList->size > 0){
        node->data = data;
        node->next = singlyLinkedList->head;
        singlyLinkedList->head = node;
    }
    singlyLinkedList->size++;
    return true;
}


// Insert data at the end of the list.
//
// If the list is empty:
//
//     the new node becomes both head and tail.
//
// Otherwise:
//
//     the current tail links to the new node and the
//     new node becomes tail.
//
// Because the list stores a tail pointer, no full traversal
// is required.
//
// On success:
//
//     size increases by 1.
//
// Returns:
//
//     true  -> insertion succeeded
//     false -> invalid arguments, allocation failure,
//              or size overflow
//
// Expected complexity:
//
//     O(1)
bool singlyLinkedList_insert_back(SinglyLinkedList *singlyLinkedList,void *data){
    if (singlyLinkedList == NULL || data == NULL){
        return false;
    }
    LinkedListNode *node = malloc(sizeof(*node));
    if (node == NULL){
        return false;
    }
    if (singlyLinkedList->size == 0){
        node->data = data;
        node->next = NULL;
        singlyLinkedList->head = node;
        singlyLinkedList->tail = node;
    } else if (singlyLinkedList->size > 0){
        node->data = data;
        node->next = NULL;
        singlyLinkedList->tail->next = node;
        singlyLinkedList->tail = node;
    }
    singlyLinkedList->size++;
    return true;
}


// Insert data at logical index.
//
// Valid insertion indices:
//
//     0 <= index <= size
//
// Special cases:
//
//     index == 0
//         equivalent to inserting at the front.
//
//     index == size
//         equivalent to inserting at the back.
//
// For a middle insertion, the list is traversed to the node
// immediately preceding index.
//
// Existing nodes retain their original relative order.
//
// On success:
//
//     size increases by 1.
//
// Returns:
//
//     true  -> insertion succeeded
//     false -> invalid arguments, invalid index,
//              allocation failure, or size overflow
//
// Expected complexity:
//
//     O(n)
bool singlyLinkedList_insert_at(SinglyLinkedList *singlyLinkedList,void *data,size_t index){
    if (singlyLinkedList == NULL || data == NULL || index > singlyLinkedList->size){
        return false;
    }
    bool result = false;
    if (index == 0){
        result = singlyLinkedList_insert_front(singlyLinkedList, data);
    } else if (index == singlyLinkedList->size){
        result = singlyLinkedList_insert_back(singlyLinkedList, data);
    } else{
        // For a middle insertion, the list is traversed to the node
        // immediately preceding index.
        LinkedListNode *currentNode = singlyLinkedList->head;
        size_t currentIndex = 0;      
        while (currentNode != NULL && currentIndex + 1 < index){
            currentNode = currentNode->next;
            currentIndex++;
        }
        LinkedListNode *node = malloc(sizeof(*node));
        if (node == NULL){
            return false;
        }
        node->data = data;
        LinkedListNode *nextNodeToTheInsertedNode = currentNode->next;
        currentNode->next = node;
        node->next = nextNodeToTheInsertedNode;
        result = true;
    }
    if (result) {
        singlyLinkedList->size++;
    }
    return result;
}


// Reverse the logical order of all nodes in place.
//
// Example:
//
//     A -> B -> C -> NULL
//
// becomes:     A -> B -> C -> D -> E -> F -> G
//
//     C -> B -> A -> NULL
//
// Stored data pointers are not copied or destroyed.
//
// head and tail are updated to represent the reversed list.
//
// size remains unchanged.
//
// Reversing an empty or single-node list is a valid no-op.
//
// Returns:
//
//     true  -> operation succeeded
//     false -> singlyLinkedList is NULL
//
// Expected complexity:
//
//     O(n)
//
// Additional node storage:
//
//     O(1)
bool singlyLinkedList_reverse(SinglyLinkedList *singlyLinkedList){
    if (singlyLinkedList == NULL){
        return false;
    }
    if (singlyLinkedList->size == 0){
        return true;
        // no need to do anything as empty linked list
    } else if (singlyLinkedList->size == 1){
        return true;
        // no need to do anything as single-node linked list (already head == tail)
    } else{ 
        void **temp = malloc(sizeof(**temp) * singlyLinkedList->size);
        LinkedListNode *currentNode = singlyLinkedList->head;
        size_t currentIndex = 0; 
        temp[currentIndex] = currentNode->data; 
        while (currentNode != NULL){
            currentNode = currentNode->next;
            currentIndex++;
            temp[currentIndex] = currentNode->data;
        } 
        currentNode = singlyLinkedList->head;
        while (currentIndex > 0){
            currentNode->data = temp[currentIndex];
            currentNode = currentNode->next;
            currentIndex--;
        }
        // head is still head and tail is tail but they have the data's pointing to in the reverse order
        // so the structure links are actually same but as the data pointers moved around now it is reversed
        free(temp);
    }
}


/* ============================================================
   Access and Lookup
   ============================================================ */

// Borrow the first stored value without removing it.
//
// The returned pointer remains stored inside the list.
//
// Ownership is NOT transferred to the caller.
//
// Returns:
//
//     non-NULL -> first stored data pointer
//     NULL     -> list is NULL or empty
//
// Expected complexity:
//
//     O(1)
const void *singlyLinkedList_get_first(const SinglyLinkedList *singlyLinkedList){
    return (singlyLinkedList == NULL || singlyLinkedList->size == 0) ? NULL : singlyLinkedList->head->data;
}


// Borrow the final stored value without removing it.
//
// Because tail is stored directly, no traversal is required.
//
// The returned pointer remains stored inside the list.
//
// Ownership is NOT transferred to the caller.
//
// Returns:
//
//     non-NULL -> final stored data pointer
//     NULL     -> list is NULL or empty
//
// Expected complexity:
//
//     O(1)
const void *singlyLinkedList_get_last(const SinglyLinkedList *singlyLinkedList){
    return (singlyLinkedList == NULL || singlyLinkedList->size == 0) ? NULL : singlyLinkedList->tail->data;
}


// Return whether the list contains at least one stored value
// logically equal to data.
//
// Equality is determined by singlyLinkedListComparisonFunc.
//
// The supplied data pointer acts only as a lookup key and is
// not stored, modified, removed, or destroyed.
//
// Returns:
//
//     true  -> at least one matching value exists
//     false -> no matching value exists, arguments are invalid,
//              or comparison function is NULL
//
// Expected complexity:
//
//     O(n)
bool singlyLinkedList_contains(const SinglyLinkedList *singlyLinkedList,const void *data,SinglyLinkedListComparisonFunc singlyLinkedListComparisonFunc){
    if (singlyLinkedList == NULL || data == NULL || singlyLinkedListComparisonFunc == NULL){
        return false;
    }

    LinkedListNode *currentNode = singlyLinkedList->head; 
    while (currentNode != NULL){
        if (singlyLinkedListComparisonFunc(currentNode->data, data)){
            return true; // as found 
        }
        currentNode = currentNode->next;
    }
    return false; // not present within the structure
}


/* ============================================================
   Removal
   ============================================================ */

// Remove the first node from the list.
//
// The LinkedListNode itself is released internally.
//
// The stored data pointer is returned and responsibility for
// that pointer transfers to the caller.
//
// If the removed node was the only node:
//
//     head = NULL
//     tail = NULL
//     size = 0
//
// Otherwise:
//
//     head advances to the next live node.
//
// Returns:
//
//     non-NULL -> removed stored data pointer
//     NULL     -> list is NULL or empty
//
// Expected complexity:
//
//     O(1)
void *singlyLinkedList_remove_first(SinglyLinkedList *singlyLinkedList){
    if (singlyLinkedList = NULL || singlyLinkedList->size == 0){
        return NULL;
    }
    LinkedListNode *headNode = singlyLinkedList->head;
    if (singlyLinkedList->size > 1){
        singlyLinkedList->head = headNode->next;
    } else{
        singlyLinkedList->head = NULL;
        singlyLinkedList->tail = NULL;
        singlyLinkedList->size = 0;
    }
    void *data = headNode->data;
    free(headNode); // free the LinkedListNode
    return data;
}


// Remove the final node from the list.
//
// The LinkedListNode itself is released internally.
//
// The stored data pointer is returned and responsibility for
// that pointer transfers to the caller.
//
// Because this is a singly linked list, the node immediately
// preceding tail must normally be found by traversal.
//
// If the removed node was the only node:
//
//     head = NULL
//     tail = NULL
//     size = 0
//
// Otherwise:
//
//     tail moves to the previous node
//     tail->next becomes NULL
//
// Returns:
//
//     non-NULL -> removed stored data pointer
//     NULL     -> list is NULL or empty
//
// Expected complexity:
//
//     O(n)
void *singlyLinkedList_remove_last(SinglyLinkedList *singlyLinkedList){
    if (singlyLinkedList = NULL || singlyLinkedList->size == 0){
        return NULL;
    }
    LinkedListNode *tailNode = singlyLinkedList->tail;
    void *data = tailNode->data;
    if (singlyLinkedList->size > 1){
        LinkedListNode *currentNode = singlyLinkedList->head;      
        while (currentNode != NULL){
            currentNode = currentNode->next;
            if (currentNode->next->next == NULL){
                singlyLinkedList->tail = currentNode->next;
                break;
            }
        }
    } else{
        singlyLinkedList->head = NULL;
        singlyLinkedList->tail = NULL;
        singlyLinkedList->size = 0;
    }
    free(tailNode); // free the LinkedListNode
    return data;
}


// Remove the node at logical index.
//
// Valid removal indices:
//
//     0 <= index < size
//
// Special cases:
//
//     index == 0
//         removes head.
//
//     index == size - 1
//         removes tail.
//
// The removed LinkedListNode is released internally.
//
// The stored data pointer is returned and responsibility for
// that pointer transfers to the caller.
//
// On success:
//
//     size decreases by 1.
//
// Returns:
//
//     non-NULL -> removed stored data pointer
//     NULL     -> invalid list, empty list, or invalid index
//
// Expected complexity:
//
//     O(n)
void *singlyLinkedList_remove_at(SinglyLinkedList *singlyLinkedList,size_t index){
    if (singlyLinkedList = NULL || singlyLinkedList->size == 0 || index >= singlyLinkedList->size){
        return NULL;
    }
    void *data;
    if (index == 0){
        data = singlyLinkedList_remove_head(singlyLinkedList);
    } else if (index == singlyLinkedList->size-1){
        data = singlyLinkedList_remove_last(singlyLinkedList);
    } else{
        LinkedListNode *previousNode;
        LinkedListNode *currentNode = singlyLinkedList->head;      
        for (size_t currentNodeIndex = 0; currentNodeIndex < index; currentNodeIndex++){
            previousNode = currentNode;
            currentNode = currentNode->next;
        }
        data = currentNode->data;
        previousNode->next = currentNode->next;
        free(currentNode); // free the LinkedListNode
    }
    return data;
}


// Remove the first stored value logically equal to data.
//
// Equality is determined by singlyLinkedListComparisonFunc.
//
// If multiple equal values exist, only the first matching node
// encountered from head is removed.
//
// The removed LinkedListNode is released internally.
//
// The exact stored data pointer belonging to that node is returned
// and responsibility transfers to the caller.
//
// The supplied data argument acts only as the lookup key and is
// not itself stored or destroyed by this operation.
//
// Returns:
//
//     non-NULL -> matching value was removed
//     NULL     -> no matching value exists or arguments are invalid
//
// Expected complexity:
//
//     O(n)
void *singlyLinkedList_remove_value(SinglyLinkedList *singlyLinkedList,const void *data,SinglyLinkedListComparisonFunc singlyLinkedListComparisonFunc){
    if (singlyLinkedList = NULL || singlyLinkedList->size == 0 || data == NULL){
        return NULL;
    }
    void *data = NULL;
    LinkedListNode *previousNode;
    LinkedListNode *currentNode = singlyLinkedList->head;      
    for (size_t currentNodeIndex = 0; currentNodeIndex < singlyLinkedList->size; currentNodeIndex++){
        previousNode = currentNode;
        currentNode = currentNode->next;
        if (singlyLinkedListComparisonFunc(currentNode->data, data)){
            data = currentNode->data;
            previousNode->next = currentNode->next; 
            free(currentNode); // free the LinkedListNode
            break;
        }
    }
    return data;
}


/* ============================================================
   Observation
   ============================================================ */

// Return the number of live nodes currently stored.
//
// Returns 0 for a NULL list.
//
// Expected complexity:
//
//     O(1)
size_t singlyLinkedList_size(const SinglyLinkedList *singlyLinkedList){
    return (singlyLinkedList == NULL) ? 0 : singlyLinkedList->size;
}


// Return whether the list contains no live nodes.
//
// Empty means:
//
//     size == 0
//     head == NULL
//     tail == NULL
//
// A NULL list is treated as empty according to the
// library-wide NULL-object convention.
//
// Expected complexity:
//
//     O(1)
bool singlyLinkedList_is_empty(const SinglyLinkedList *singlyLinkedList){
    return (singlyLinkedList == NULL || singlyLinkedList->size == 0) ? true : false;
}


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove every live node while retaining the SinglyLinkedList
// container itself.
//
// If destroyData is non-NULL:
//
//     destroyData is called exactly once for every live stored
//     data pointer before its node is released.
//
// If destroyData is NULL:
//
//     only the internal LinkedListNode objects are released.
//
//     Stored data pointers are not destroyed.
//
// After clearing:
//
//     head = NULL
//     tail = NULL
//     size = 0
//
// Pass NULL for borrowed, static, stack-allocated, or otherwise
// externally managed data.
void singlyLinkedList_clear(SinglyLinkedList *singlyLinkedList,void (*destroyData)(void *data)){
    if (singlyLinkedList = NULL || singlyLinkedList->size == 0){
        return;
    }
    // clear the SinglyLinkedList
    LinkedListNode *currentNode = singlyLinkedList->head;
    LinkedListNode *removedNode = currentNode; // loop iterating pointer
    while (currentNode != NULL){
        if (destroyData != NULL){
            destroyData(removedNode->data);
        }
        removedNode->data = NULL;
        currentNode = currentNode->next;
        free(removedNode); // free the underlying LinkedListNode
        removedNode = currentNode;
    }
    singlyLinkedList->size = 0;
    singlyLinkedList->head = NULL;
    singlyLinkedList->tail = NULL;
}


// Destroy the entire singly linked list.
//
// Every remaining LinkedListNode is released.
//
// If destroyData is non-NULL:
//
//     destroyData is called exactly once for every remaining
//     stored data pointer before its node is released.
//
// If destroyData is NULL:
//
//     stored data pointers are left untouched.
//
// After all nodes are released, the SinglyLinkedList container
// itself is freed.
//
// After this function returns, the caller's SinglyLinkedList
// pointer is invalid and must not be dereferenced.
//
// Pass NULL for borrowed, static, stack-allocated, or otherwise
// externally managed data.
void singlyLinkedList_destroy(SinglyLinkedList *singlyLinkedList,void (*destroyData)(void *data)){
    if (singlyLinkedList = NULL || singlyLinkedList->size == 0){
        return;
    }
    // clear the SinglyLinkedList
    LinkedListNode *currentNode = singlyLinkedList->head;
    LinkedListNode *removedNode = currentNode; // loop iterating pointer
    while (currentNode != NULL){
        if (destroyData != NULL){
            destroyData(removedNode->data);
        }
        removedNode->data = NULL;
        currentNode = currentNode->next;
        free(removedNode); // free the underlying LinkedListNode
        removedNode = currentNode;
    }
    // free / destroy the SinglyLinkedList structure
    free(singlyLinkedList);
}


/* ============================================================
   Debugging
   ============================================================ */

// Print every stored value in logical order from head to tail.
//
// Conceptual rendering:
//
//     [A -> B -> -> C]
//
// The caller supplies print_func to define how one stored value
// should be printed.
//
// Each live stored data pointer is passed to print_func exactly
// once.
//
// This function does not modify the list.
//
// Expected complexity:
//
//     O(n)
void singlyLinkedList_print(const SinglyLinkedList *singlyLinkedList,void (*print_func)(const void *data)){
    if (singlyLinkedList == NULL || print_func == NULL){
        return;
    }
    printf("[");
    LinkedListNode *currentNode = singlyLinkedList->head;
    for (size_t index = 0; index < singlyLinkedList->size; index++){
        printf("Node: ");
        print_func(currentNode->data);
        if(index + 1 < singlyLinkedList->size){
            printf("-> ");
        }
        currentNode = currentNode->next;
    }
    printf("]");
    printf("\n");
}
