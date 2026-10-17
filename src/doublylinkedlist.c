#include "doublylinkedlist.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/*
    Generic DoublyLinkedList

    A doubly linked list stores values in individually allocated
    nodes connected in logical order through both next and previous
    pointers.

    Each node may therefore be traversed in either direction:

        NULL <- [A] <-> [B] <-> [C] -> NULL
                 ^               ^
                 |               |
                head            tail


    Representation invariants:

    - size == 0 if and only if:
          head == NULL
          tail == NULL

    - If size == 1:
          head == tail

    - If size > 0:
          head != NULL
          tail != NULL

    - head points to the first live node.

    - tail points to the final live node.

    - The first node's previous pointer is NULL.

    - The final node's next pointer is NULL.

    - For every live node with a non-NULL next pointer:

          node->next->previous == node

    - For every live node with a non-NULL previous pointer:

          node->previous->next == node

    - Exactly size live nodes are reachable by repeatedly
      following next from head.

    - Exactly size live nodes are reachable by repeatedly
      following previous from tail.

    - Forward traversal from head eventually reaches NULL
      without revisiting a live node.

    - Backward traversal from tail eventually reaches NULL
      without revisiting a live node.

    - Every live node contains a non-NULL data pointer.


    Storage model:

    - The list stores the supplied data pointer directly.

    - The pointed-to object itself is not copied.

    - The list owns and manages its internal
      DoublyLinkedListNode allocations.

    - Stored data ownership depends on the operation being used.

    - Duplicate logical values are permitted.

    - Access operations return borrowed pointers.

    - Removal operations return the stored pointer and transfer
      responsibility for that pointer to the caller.


    Thread safety:

    - Thread-safety requirements follow the project-wide policy
      documented in the library README.
*/

/* ============================================================
   Representation
   ============================================================ */

// Represents one node in the doubly linked list.
//
// data:
//
//     stores the caller-supplied data pointer.
//
// next:
//
//     points to the subsequent DoublyLinkedListNode
//     in logical order.
//
// previous:
//
//     points to the preceding DoublyLinkedListNode
//     in logical order.
//
// For the first node:
//
//     previous == NULL
//
// For the final node:
//
//     next == NULL
typedef struct DoublyLinkedListNode {
    void *data;
    struct DoublyLinkedListNode *next;
    struct DoublyLinkedListNode *previous;
} DoublyLinkedListNode;


// Represents the complete doubly linked list.
//
// head:
//
//     points to the first live node.
//
// tail:
//
//     points to the final live node.
//
// size:
//
//     number of live nodes currently stored.
struct DoublyLinkedList{
    DoublyLinkedListNode *head;
    DoublyLinkedListNode *tail;
    size_t size;
};

/* ============================================================
   Construction
   ============================================================ */

// Construct a valid empty doubly linked list.
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
DoublyLinkedList *doublyLinkedList_create(void){
    DoublyLinkedList *doublyLinkedList = malloc(sizeof(*doublyLinkedList));
    if (doublyLinkedList == NULL){
        return NULL;
    }
    doublyLinkedList->head = NULL;
    doublyLinkedList->tail = NULL;
    doublyLinkedList->size = 0;
    return doublyLinkedList;
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
//     the new node becomes head
//     the new node's next pointer refers to the previous head
//     the previous head's previous pointer refers to the new node
//
// The new head's previous pointer is NULL.
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
bool doublyLinkedList_insert_front(DoublyLinkedList *doublyLinkedList,void *data){
    if (doublyLinkedList == NULL || data == NULL){
        return false;
    }
    DoublyLinkedListNode *node = malloc(sizeof(*node));
    if (node == NULL){
        return false;
    }
    if (doublyLinkedList->size >= SIZE_MAX){
        free(node);
        return false; // check for overflows
    }
    node->data = data;
    node->previous = NULL;
    if (doublyLinkedList->size == 0){
        node->next = NULL;
        doublyLinkedList->head = node;
        doublyLinkedList->tail = node;
    } else{
        node->next = doublyLinkedList->head;
        doublyLinkedList->head->previous = node;
        doublyLinkedList->head = node;
    }
    doublyLinkedList->size++;
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
//     the current tail's next pointer refers to the new node
//     the new node's previous pointer refers to the old tail
//     the new node becomes tail
//
// The new tail's next pointer is NULL.
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
bool doublyLinkedList_insert_back(DoublyLinkedList *doublyLinkedList,void *data){
    if (doublyLinkedList == NULL || data == NULL){
        return false;
    }
    DoublyLinkedListNode *node = malloc(sizeof(*node));
    if (node == NULL){
        return false;
    }
    if (doublyLinkedList->size >= SIZE_MAX){
        free(node);
        return false; // check for overflows
    }
    node->data = data;
    node->next = NULL;
    if (doublyLinkedList->size == 0){
        node->previous = NULL;
        doublyLinkedList->head = node;
        doublyLinkedList->tail = node;
    } else{
        node->previous = doublyLinkedList->tail;
        doublyLinkedList->tail->next = node;
        doublyLinkedList->tail = node;
    }
    doublyLinkedList->size++;
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
// For a middle insertion, the new node is linked between the
// nodes immediately before and after the insertion position.
//
// Both next and previous relationships must remain consistent.
//
// An implementation may traverse from head or tail depending
// on which endpoint is closer to the requested index.
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
//     O(n) worst case
bool doublyLinkedList_insert_at(DoublyLinkedList *doublyLinkedList,void *data,size_t index){
    if (doublyLinkedList == NULL || data == NULL || index > doublyLinkedList->size){
        return false;
    }
    bool result = false;
    if (index == 0){
        result = doublyLinkedList_insert_front(doublyLinkedList,data);
        //doublyLinkedList->size++; already done
    } else if (index == doublyLinkedList->size){
        result = doublyLinkedList_insert_back(doublyLinkedList,data);
        //doublyLinkedList->size++; already done
    } else{
        DoublyLinkedListNode *node = malloc(sizeof(*node));
        if (node == NULL){
            return false;
        }
        if (doublyLinkedList->size >= SIZE_MAX){
            free(node);
            return false; // check for overflows
        } 
        node->data = data;
        DoublyLinkedListNode *currentNode;
        if (index <= (doublyLinkedList->size / 2)){ // near the beginning
            currentNode = doublyLinkedList->head;
            for (size_t currentNodeIndex = 0; currentNodeIndex + 1 < index; currentNodeIndex++){
                currentNode = currentNode->next;
            }
        } else { // near the end
            currentNode = doublyLinkedList->tail;
            for (size_t currentNodeIndex = doublyLinkedList->size-1; currentNodeIndex + 1 > index; currentNodeIndex--){
                currentNode = currentNode->previous;                       
            }
        }
        DoublyLinkedListNode *nextNodeToTheInsertedNode = currentNode->next;
        nextNodeToTheInsertedNode->previous = node;
        currentNode->next = node;
        node->previous = currentNode;
        node->next = nextNodeToTheInsertedNode;
        result = true;
        doublyLinkedList->size++;
    }
    
    return result;
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
const void *doublyLinkedList_get_first(const DoublyLinkedList *doublyLinkedList){
    return (doublyLinkedList == NULL || doublyLinkedList->size == 0) ? NULL : doublyLinkedList->head->data;
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
const void *doublyLinkedList_get_last(const DoublyLinkedList *doublyLinkedList){
    return (doublyLinkedList == NULL || doublyLinkedList->size == 0) ? NULL : doublyLinkedList->tail->data;
}


// Return whether the list contains at least one stored value
// logically equal to data.
//
// Equality is determined by doublyLinkedListComparisonFunc.
//
// The supplied data pointer acts only as a lookup key and is
// not stored, modified, removed, or destroyed.
//
// Search proceeds logically from head toward tail.
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
bool doublyLinkedList_contains(const DoublyLinkedList *doublyLinkedList,const void *data,DoublyLinkedListComparisonFunc doublyLinkedListComparisonFunc){
    if (doublyLinkedList == NULL || data == NULL || doublyLinkedListComparisonFunc == NULL){
        return false;
    }

    DoublyLinkedListNode *currentNode = doublyLinkedList->head; 
    while (currentNode != NULL){
        if (doublyLinkedListComparisonFunc(currentNode->data, data)){
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
// The DoublyLinkedListNode itself is released internally.
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
//     head advances to the next live node
//     the new head's previous pointer becomes NULL
//
// Returns:
//
//     non-NULL -> removed stored data pointer
//     NULL     -> list is NULL or empty
//
// Expected complexity:
//
//     O(1)
void *doublyLinkedList_remove_first(DoublyLinkedList *doublyLinkedList){
    if (doublyLinkedList == NULL || doublyLinkedList->size == 0){
        return NULL;
    }
    DoublyLinkedListNode *headNode = doublyLinkedList->head;
    void *data = headNode->data;
    if (doublyLinkedList->size > 1){
        doublyLinkedList->head = headNode->next;
        doublyLinkedList->head->previous = NULL;
        doublyLinkedList->size -= 1;
    } else{
        doublyLinkedList->head = NULL;
        doublyLinkedList->tail = NULL;
        doublyLinkedList->size = 0;
    }
    free(headNode); // free the DoublyLinkedListNode
    return data;
}


// Remove the final node from the list.
//
// The DoublyLinkedListNode itself is released internally.
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
//     tail moves directly to the previous node
//     the new tail's next pointer becomes NULL
//
// Because each node stores its previous node, the predecessor
// of tail is available directly and no traversal is required.
//
// Returns:
//
//     non-NULL -> removed stored data pointer
//     NULL     -> list is NULL or empty
//
// Expected complexity:
//
//     O(1)
void *doublyLinkedList_remove_last(DoublyLinkedList *doublyLinkedList){
    if (doublyLinkedList == NULL || doublyLinkedList->size == 0){
        return NULL;
    }
    DoublyLinkedListNode *tailNode = doublyLinkedList->tail;
    void *data = tailNode->data;
    if (doublyLinkedList->size > 1){
        doublyLinkedList->tail = tailNode->previous;
        doublyLinkedList->tail->next = NULL;
        doublyLinkedList->size -= 1;
    } else{
        doublyLinkedList->head = NULL;
        doublyLinkedList->tail = NULL;
        doublyLinkedList->size = 0;
    }
    free(tailNode); // free the DoublyLinkedListNode
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
// For a middle removal, the surrounding nodes are linked
// directly to one another:
//
//     removed->previous->next = removed->next
//     removed->next->previous = removed->previous
//
// The removed DoublyLinkedListNode is released internally.
//
// The stored data pointer is returned and responsibility for
// that pointer transfers to the caller.
//
// An implementation may traverse from head or tail depending
// on which endpoint is closer to the requested index.
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
//     O(n) worst case
void *doublyLinkedList_remove_at(DoublyLinkedList *doublyLinkedList,size_t index){
    if (doublyLinkedList == NULL || doublyLinkedList->size == 0 || index >= doublyLinkedList->size){
        return NULL;
    }
    void *data;
    if (index == 0){
        data = doublyLinkedList_remove_first(doublyLinkedList);
    } else if (index == doublyLinkedList->size-1){
        data = doublyLinkedList_remove_last(doublyLinkedList);
    } else{
        DoublyLinkedListNode *currentNode;
        if (index < (doublyLinkedList->size / 2)){ // near the beginning
            currentNode = doublyLinkedList->head;
            for (size_t currentNodeIndex = 0; currentNodeIndex < index; currentNodeIndex++){
                currentNode = currentNode->next;
            }
        } else { // near the end
            currentNode = doublyLinkedList->tail;
            for (size_t currentNodeIndex = doublyLinkedList->size-1; currentNodeIndex > index; currentNodeIndex--){
                currentNode = currentNode->previous;                       
            }
        }
        
        data = currentNode->data;
        currentNode->previous->next = currentNode->next;
        currentNode->next->previous = currentNode->previous;
        free(currentNode); // free the DoublyLinkedListNode
        doublyLinkedList->size--;
    }
    return data;
}


// Remove the first stored value logically equal to data.
//
// Equality is determined by doublyLinkedListComparisonFunc.
//
// If multiple logically equal values exist, only the first
// matching node encountered from head is removed.
//
// The removed node's neighbours are reconnected so that both
// next and previous relationships remain valid.
//
// The removed DoublyLinkedListNode is released internally.
//
// The exact stored data pointer belonging to that node is
// returned and responsibility transfers to the caller.
//
// The supplied data argument acts only as the lookup key and
// is not itself stored, modified, or destroyed.
//
// Returns:
//
//     non-NULL -> matching value was removed
//     NULL     -> no matching value exists or arguments are invalid
//
// Expected complexity:
//
//     O(n)
void *doublyLinkedList_remove_value(DoublyLinkedList *doublyLinkedList,const void *data,DoublyLinkedListComparisonFunc doublyLinkedListComparisonFunc){
    if (doublyLinkedList == NULL || doublyLinkedList->size == 0 || data == NULL || doublyLinkedListComparisonFunc == NULL){
        return NULL;
    }
    void *returnData = NULL;
    DoublyLinkedListNode *currentNode = doublyLinkedList->head;      
    for (size_t currentNodeIndex = 0; currentNodeIndex < doublyLinkedList->size; currentNodeIndex++){
        if (doublyLinkedListComparisonFunc(currentNode->data, data)){
            if (currentNodeIndex  == 0){
                // update head
                if (doublyLinkedList->size == 1){
                    doublyLinkedList->head = NULL;
                    doublyLinkedList->tail = NULL;
                } else{
                    doublyLinkedList->head = doublyLinkedList->head->next;
                    doublyLinkedList->head->previous = NULL;
                }
            }
            else if (currentNodeIndex == doublyLinkedList->size - 1){
                // update tail 
                doublyLinkedList->tail = currentNode->previous;
                doublyLinkedList->tail->next = NULL;
            } else{
                currentNode->previous->next = currentNode->next;  
                currentNode->next->previous = currentNode->previous;
            }
            returnData = currentNode->data; 
            free(currentNode); // free the DoublyLinkedListNode
            doublyLinkedList->size--;
            break;
        }
        currentNode = currentNode->next;
    }
    return returnData;
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
size_t doublyLinkedList_size(const DoublyLinkedList *doublyLinkedList){
    return (doublyLinkedList == NULL) ? 0 : doublyLinkedList->size;
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
bool doublyLinkedList_is_empty(const DoublyLinkedList *doublyLinkedList){
    return (doublyLinkedList == NULL || doublyLinkedList->size == 0 || doublyLinkedList->head == NULL || doublyLinkedList->tail == NULL) ? true : false;
}


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove every live node while retaining the DoublyLinkedList
// container itself.
//
// If destroyData is non-NULL:
//
//     destroyData is called exactly once for every live stored
//     data pointer before its node is released.
//
// If destroyData is NULL:
//
//     only the internal DoublyLinkedListNode objects are released.
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
void doublyLinkedList_clear(DoublyLinkedList *doublyLinkedList,void (*destroyData)(void *data)){
    if (doublyLinkedList == NULL || doublyLinkedList->size == 0){
        return;
    }
    // clear the DoublyLinkedList
    DoublyLinkedListNode *currentNode = doublyLinkedList->head;
    DoublyLinkedListNode *removedNode = currentNode; // loop iterating pointer
    while (currentNode != NULL){
        if (destroyData != NULL){
            destroyData(removedNode->data);
        }
        removedNode->data = NULL;
        currentNode = currentNode->next;
        free(removedNode); // free the underlying DoublyLinkedListNode
        removedNode = currentNode;
    }
    doublyLinkedList->size = 0;
    doublyLinkedList->head = NULL;
    doublyLinkedList->tail = NULL;
}


// Destroy the entire doubly linked list.
//
// Every remaining DoublyLinkedListNode is released.
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
// After all nodes are released, the DoublyLinkedList container
// itself is freed.
//
// After this function returns, the caller's DoublyLinkedList
// pointer is invalid and must not be dereferenced.
//
// Pass NULL for borrowed, static, stack-allocated, or otherwise
// externally managed data.
void doublyLinkedList_destroy(DoublyLinkedList *doublyLinkedList,void (*destroyData)(void *data)){
    if (doublyLinkedList == NULL || doublyLinkedList->size == 0){
        free(doublyLinkedList);
        return;
    }
    // clear the DoublyLinkedList
    DoublyLinkedListNode *currentNode = doublyLinkedList->head;
    DoublyLinkedListNode *removedNode = currentNode; // loop iterating pointer
    while (currentNode != NULL){
        if (destroyData != NULL){
            destroyData(removedNode->data);
        }
        removedNode->data = NULL;
        currentNode = currentNode->next;
        free(removedNode); // free the underlying DoublyLinkedListNode
        removedNode = currentNode;
    }
    // free / destroy the DoublyLinkedList structure
    free(doublyLinkedList);
}


/* ============================================================
   Debugging
   ============================================================ */

// Print every stored value in logical order from head to tail.
//
// Conceptual rendering:
//
//     [Data: A <-> Data: B <-> Data: C]
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
void doublyLinkedList_print(const DoublyLinkedList *doublyLinkedList,void (*print_func)(const void *data)){
    if (doublyLinkedList == NULL || print_func == NULL){
        return;
    }
    printf("[");
    DoublyLinkedListNode *currentNode = doublyLinkedList->head;
    for (size_t index = 0; index < doublyLinkedList->size; index++){
        printf("Data: ");
        print_func(currentNode->data);
        if(index + 1 < doublyLinkedList->size){
            printf(" <-> ");
        }
        currentNode = currentNode->next;
    }
    printf("]");
    printf("\n");
}
