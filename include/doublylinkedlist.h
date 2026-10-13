#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <stddef.h>
#include <stdbool.h>


/*
    Generic DoublyLinkedList

    A doubly linked list stores values in individually allocated
    nodes connected in logical order through next pointers. Additional to the normal singly linked
    list it holds previous pointer for reverse traversal + some other uses.

    Example: 

        head
         |
         v
        [A] -> [B] -> [C] -> NULL
 Null <-    <-     <-   ^
                        |
                       tail


    Representation invariants:

    - size == 0 if and only if head == NULL.

    - If size == 0:
          head == NULL
          tail == NULL

    - If size == 1:
          head == tail

    - If size > 0:
          head points to the first live node
          tail points to the final live node

    - The final node's next pointer is NULL.

    - Exactly size live nodes are reachable by repeatedly
      following next from head.

    - No live node appears twice during traversal.

    - This representation contains no cycle.

    - Every live node contains a non-NULL data pointer.


    Storage model:

    - The list stores the supplied data pointer directly.

    - The pointed-to object itself is not copied.

    - The list owns and manages its internal LinkedListNode
      allocations.

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
   Value Policies
   ============================================================ */

// Compare two logical values.
//
// Convention:
//
//     true  -> value1 and value2 are logically equal
//     false -> value1 and value2 are logically different
//
// The caller defines what equality means for the stored type.
//
// The comparison function:
//
//     - must not modify either argument
//     - should return consistent results for the same values
//
// It is used only by operations that require logical-value
// comparison, such as contains and remove_value.
typedef bool (*DoublyLinkedListComparisonFunc)(
    const void *value1,
    const void *value2
);


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
//     refers to the subsequent DoublyLinkedListNode in logical order.
//
// previous:
//
//     refers to the previous DoublyLinkedListNode in logical order.
//
// For the first node:
//
//     previous == NULL
//
// For the final node:
//
//     next == NULL
typedef struct DoublyLinkedListNode{
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
typedef struct {
    DoublyLinkedListNode *head;
    DoublyLinkedListNode *tail;
    size_t size;
} DoublyLinkedList;


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
DoublyLinkedList *doublyLinkedList_create(void);


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
bool doublyLinkedList_insert_front(DoublyLinkedList *doublyLinkedList,void *data);


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
bool doublyLinkedList_insert_back(DoublyLinkedList *doublyLinkedList,void *data);


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
bool doublyLinkedList_insert_at(DoublyLinkedList *doublyLinkedList,void *data,size_t index);


// Reverse the logical order of all nodes in place.
//
// Example:
//
//     A -> B -> C -> NULL
//
// becomes:
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
//     false -> doublyLinkedList is NULL
//
// Expected complexity:
//
//     O(n)
//
// Additional node storage:
//
//     O(1)
bool doublyLinkedList_reverse(DoublyLinkedList *doublyLinkedList);


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
const void *doublyLinkedList_get_first(const DoublyLinkedList *doublyLinkedList);


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
const void *doublyLinkedList_get_last(const DoublyLinkedList *doublyLinkedList);


// Return whether the list contains at least one stored value
// logically equal to data.
//
// Equality is determined by doublyLinkedListComparisonFunc.
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
bool doublyLinkedList_contains(const DoublyLinkedList *doublyLinkedList,const void *data,DoublyLinkedListComparisonFunc doublyLinkedListComparisonFunc);


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
void *doublyLinkedList_remove_first(DoublyLinkedList *doublyLinkedList);


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
void *doublyLinkedList_remove_last(DoublyLinkedList *doublyLinkedList);


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
// The removed DoublyLinkedList is released internally.
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
void *doublyLinkedList_remove_at(DoublyLinkedList *doublyLinkedList,size_t index);


// Remove the first stored value logically equal to data.
//
// Equality is determined by doublyLinkedListComparisonFunc.
//
// If multiple equal values exist, only the first matching node
// encountered from head is removed.
//
// The removed DoublyLinkedList is released internally.
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
void *doublyLinkedList_remove_value(DoublyLinkedList *doublyLinkedList,const void *data,DoublyLinkedListComparisonFunc doublyLinkedListComparisonFunc);


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
size_t doublyLinkedList_size(const DoublyLinkedList *doublyLinkedList);


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
bool doublyLinkedList_is_empty(const DoublyLinkedList *doublyLinkedList);


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
void doublyLinkedList_clear(DoublyLinkedList *doublyLinkedList,void (*destroyData)(void *data));


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
void doublyLinkedList_destroy(DoublyLinkedList *doublyLinkedList,void (*destroyData)(void *data));


/* ============================================================
   Debugging
   ============================================================ */

// Print every stored value in logical order from head to tail.
//
// Conceptual rendering:
//
//     [Node: A -> Data: B -> Data: C]
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
void doublyLinkedList_print(const DoublyLinkedList *doublyLinkedList,void (*print_func)(const void *data));


#endif