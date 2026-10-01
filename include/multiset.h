#ifndef MULTISET_H
#define MULTISET_H

#include <stddef.h>
#include <stdbool.h>


/*
    Generic Multiset / Bag

    A multiset represents logical values together with their
    multiplicities.

    If M is a multiset and x is a value:

        M(x) = number of occurrences of x in M

    Conceptually:

        M : Values -> Natural Number Occurrences


    Example:

        { A, A, A, B, B, C }

        distinctSize = 3
        totalSize    = 6

        multiplicity(A) = 3
        multiplicity(B) = 2
        multiplicity(C) = 1


    Representation invariants:

    - 0 <= distinctSize <= capacity

    - totalSize >= distinctSize

    - If distinctSize == 0:
          totalSize == 0

    - Every live entry occupies:
          entries[0] through entries[distinctSize - 1]

    - Every live entry satisfies:
          entry.data  != NULL
          entry.count > 0

    - No two live entries compare equal according to the
      multiset comparison function.

    - totalSize is equal to the sum of all live entry counts.

    - Slots from entries[distinctSize] through
      entries[capacity - 1] are unused storage and are not
      part of the abstract multiset.


    Storage model:

    - Exactly one canonical data pointer is stored for each
      distinct logical value.

    - Additional equal occurrences increase that entry's count.

    - Additional occurrences do NOT create additional stored
      objects or MultisetEntry objects.

    Example:

        Logical multiset:

            { A, A, A, A }

        Internal representation:

            entry.data  -> one canonical A object
            entry.count = 4

    Therefore multiplicity represents logical occurrences,
    not separately stored physical objects.


    Optional cloning:

    - A MultisetCloneFunc may optionally be supplied when the
      multiset is constructed.

    - The clone function does NOT cause every occurrence to be
      stored separately.

    - The multiset remains compressed as:

          one canonical pointer + multiplicity count

    - The clone function is used only when the implementation
      must materialize an independently owned occurrence for
      the caller.

    In particular, multiset_remove_one() can use the clone
    function when an occurrence is removed but other equal
    occurrences must remain inside the multiset.

    If no clone function is supplied, the multiset behaves as a
    purely mathematical multiset: removing one of several equal
    occurrences changes only the multiplicity.
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
// The comparison function must not modify either value.
typedef bool (*MultisetComparisonFunc)(const void *value1,const void *value2);


// Create an independent owned copy of data.
//
// The returned pointer must represent a logically equivalent
// value according to MultisetComparisonFunc.
//
// The returned object must be independent from the original
// object according to the semantics of the stored type.
//
// Examples:
//
//     int *:
//         allocate a new int and copy its value
//
//     char *:
//         allocate a new string and copy its characters
//
//     complex struct:
//         perform whatever deep copy is required by that type
//
// Returns:
//
//     non-NULL -> cloning succeeded
//     NULL     -> cloning failed
//
// This callback is optional.
//
// If NULL is supplied, the multiset operates in mathematical
// multiplicity mode and does not attempt to materialize owned
// copies for non-final removals.
typedef void *(*MultisetCloneFunc)(const void *data);


/* ============================================================
   Representation
   ============================================================ */

// Represents one distinct logical value and its multiplicity.
typedef struct {
    void *data;      // Canonical stored pointer for this value.
    size_t count;    // Number of logical occurrences.
} MultisetEntry;


// The Multiset manages a dynamic array of distinct entries.
typedef struct {
    MultisetEntry *entries;

    size_t distinctSize;
    size_t totalSize;
    size_t capacity;

    MultisetComparisonFunc multisetCompareFunc;

    // Optional.
    //
    // NULL means that non-final occurrence removal operates
    // purely by changing multiplicity and cannot materialize
    // an independently owned copy.
    MultisetCloneFunc multisetCloneFunc;
} Multiset;


/* ============================================================
   Construction
   ============================================================ */

// Construct a valid empty multiset.
//
// Initial state:
//
//     entries              = NULL
//     distinctSize         = 0
//     totalSize            = 0
//     capacity             = 0
//     multisetCompareFunc  = supplied comparison function
//     multisetCloneFunc    = supplied clone function or NULL
//
// multisetComparisonFunc defines logical equality and is
// required for operations involving values.
//
// multisetCloneFunc is optional.
//
// If multisetCloneFunc != NULL:
//
//     multiset_remove_one() may materialize an independently
//     owned copy when removing one occurrence while other equal
//     occurrences remain.
//
// If multisetCloneFunc == NULL:
//
//     the multiset behaves as a mathematical counting multiset
//     for non-final removals.
//
// Returns NULL if allocation fails.
Multiset *multiset_create(MultisetComparisonFunc multisetComparisonFunc,MultisetCloneFunc multisetCloneFunc);


// Construct an empty multiset with capacity reserved for at least
// reservedSize distinct values.
//
// Capacity refers to distinct values, NOT total occurrences.
//
// Example:
//
//     one value with count == 10,000
//
// still occupies only one MultisetEntry slot.
//
// multisetCloneFunc may be NULL.
//
// Returns NULL if allocation fails.
Multiset *multiset_reserve(size_t reservedSize,MultisetComparisonFunc multisetComparisonFunc,MultisetCloneFunc multisetCloneFunc);


/* ============================================================
   Insertion
   ============================================================ */

// Add one logical occurrence of data.
//
// If no equal entry currently exists:
//
//     - a new MultisetEntry is created
//     - data becomes its canonical stored pointer
//     - count becomes 1
//     - distinctSize increases by 1
//     - totalSize increases by 1
//
// If an equal entry already exists:
//
//     - no additional MultisetEntry is created
//     - the existing canonical pointer remains stored
//     - count increases by 1
//     - totalSize increases by 1
//     - distinctSize remains unchanged
//
// IMPORTANT:
//
// Equality does not imply pointer identity.
//
// Two different pointers may compare logically equal:
//
//     pointer A -> "hello"
//     pointer B -> "hello"
//
// If A is already the canonical stored pointer, adding B does
// not cause B itself to be stored.
//
// The ownership policy for such a redundant incoming pointer
// must therefore remain clear to the caller.
//
// The clone callback is NOT used during insertion.
//
// It exists to materialize owned occurrences when required by
// removal operations.
//
// Returns:
//
//     true  -> insertion succeeded
//     false -> invalid arguments, allocation failure,
//              multiplicity overflow, or capacity overflow
bool multiset_add(Multiset *multiset,void *data);


/* ============================================================
   Access and Lookup
   ============================================================ */

// Return the multiplicity of data.
//
// Returns:
//
//     0   -> no equal logical value exists
//     > 0 -> number of logical occurrences
//
// The supplied data pointer acts only as a lookup key.
size_t multiset_lookup(const Multiset *multiset,void *data);


// Return whether at least one logically equal value exists.
//
// Equivalent conceptually to:
//
//     multiset_lookup(multiset, data) > 0
bool multiset_contains(const Multiset *multiset,void *data);


/* ============================================================
   Removal
   ============================================================ */

// Remove exactly one logical occurrence.
//
// Three important cases exist:
//
// ------------------------------------------------------------
// Case 1: count == 1
// ------------------------------------------------------------
//
// This is the final occurrence.
//
// The entire distinct entry is removed:
//
//     count disappears
//     distinctSize--
//     totalSize--
//
// The canonical stored pointer itself is returned.
//
// Ownership of that pointer transfers to the caller.
//
// No cloning is necessary because the multiset no longer needs
// a canonical representative for that value.
//
//
// ------------------------------------------------------------
// Case 2: count > 1 and multisetCloneFunc != NULL
// ------------------------------------------------------------
//
// Other equal occurrences must remain represented by the
// canonical stored pointer.
//
// Therefore:
//
//     - the canonical pointer remains stored
//     - an independent clone of the canonical value is created
//     - count--
//     - totalSize--
//     - the clone is returned to the caller
//
// The returned clone represents the removed logical occurrence.
//
// Ownership of the clone belongs to the caller.
//
// From the public API's perspective, the caller has therefore
// removed one occurrence and received an independently owned
// object representing that occurrence.
//
// Internally, however, the multiset still stores only:
//
//     one canonical pointer + remaining multiplicity
//
//
// ------------------------------------------------------------
// Case 3: count > 1 and multisetCloneFunc == NULL
// ------------------------------------------------------------
//
// The multiset behaves as a mathematical counting multiset.
//
// Only multiplicity changes:
//
//     count--
//     totalSize--
//
// The canonical pointer remains stored.
//
// NULL is returned because there is no separately materialized
// object corresponding to the removed occurrence.
//
// The logical removal still succeeds.
//
//
// ------------------------------------------------------------
//
// Example:
//
//     canonical A
//     count = 3
//
// With a clone callback:
//
//     remove one:
//
//         canonical A remains stored
//         count becomes 2
//         clone(A) is returned
//
//     remove one:
//
//         canonical A remains stored
//         count becomes 1
//         clone(A) is returned
//
//     remove final:
//
//         canonical A leaves the multiset
//         canonical A itself is returned
//
//
// Without a clone callback:
//
//     remove one:
//
//         count becomes 2
//         NULL returned
//
//     remove one:
//
//         count becomes 1
//         NULL returned
//
//     remove final:
//
//         canonical A itself is returned
//
//
// IMPORTANT:
//
// If cloning is required but the clone function fails and
// returns NULL, the implementation should leave the multiset
// unchanged.
//
// This preserves operation atomicity:
//
//     either cloning succeeds and one occurrence is removed,
//     or nothing changes.
//
// NOTE:
//
// Because NULL can also represent successful mathematical-mode
// removal, absence, or clone failure, callers needing to
// distinguish these outcomes may eventually require a richer
// status-returning API.
void *multiset_remove_one(Multiset *multiset,void *data);


// Remove every logical occurrence of data.
//
// The entire distinct entry is removed regardless of count.
//
// If a matching entry exists:
//
//     totalSize decreases by entry.count
//     distinctSize decreases by 1
//
// The canonical stored pointer itself is returned.
//
// No cloning is needed because no occurrence of that logical
// value remains inside the multiset.
//
// Ownership of the returned canonical pointer transfers to
// the caller.
//
// Returns NULL if no matching value exists or the arguments
// are invalid.
void *multiset_remove_all(Multiset *multiset,void *data);


/* ============================================================
   Observation
   ============================================================ */

// Return the total number of logical occurrences.
//
// Example:
//
//     { A, A, A, B, B, C }
//
// returns:
//
//     6
//
// Returns 0 for a NULL multiset.
size_t multiset_totalSize(const Multiset *multiset);


// Return the number of distinct logical values.
//
// Example:
//
//     { A, A, A, B, B, C }
//
// returns:
//
//     3
//
// Returns 0 for a NULL multiset.
size_t multiset_distinctSize(const Multiset *multiset);


// Return the number of MultisetEntry slots currently allocated.
//
// Capacity refers to distinct entries, NOT total occurrences.
//
// Returns 0 for a NULL multiset.
size_t multiset_capacity(const Multiset *multiset);


// Return whether the multiset contains no logical values.
//
// Empty means:
//
//     distinctSize == 0
//     totalSize    == 0
//
// A NULL multiset may be treated as empty according to the
// library's NULL-object convention.
bool multiset_is_empty(const Multiset *multiset);


/* ============================================================
   Lifetime Management
   ============================================================ */

// Remove every distinct entry while retaining the backing
// allocation and capacity.
//
// destroyData is called once per canonical stored pointer,
// NOT once per logical occurrence.
//
// Example:
//
//     A x100
//     B x50
//
// totalSize    = 150
// distinctSize = 2
//
// destroyData is called exactly twice.
//
// Clones previously returned to callers are independent objects
// and are no longer owned or tracked by the multiset.
//
// After clearing:
//
//     distinctSize = 0
//     totalSize    = 0
//     capacity     = unchanged
//
// Pass NULL for borrowed, static, or stack-allocated data.
void multiset_clear(Multiset *multiset,void (*destroyData)(void *data));


// Destroy the multiset.
//
// destroyData is called exactly once for every remaining
// canonical stored pointer.
//
// Multiplicity does not cause repeated destruction.
//
// The optional clone function itself owns no data and requires
// no destruction.
//
// After this function returns, the caller's Multiset pointer
// is invalid.
//
// Pass NULL for borrowed, static, or stack-allocated data.
void multiset_destroy(Multiset *multiset,void (*destroyData)(void *data));


/* ============================================================
   Debugging
   ============================================================ */

// Print every distinct logical value together with its
// multiplicity.
//
// Example:
//
//     { A x3, B x2, C x1 }
//
// The canonical stored pointer for each distinct entry is passed
// to print_func exactly once.
//
// This function does not expand multiplicities into separate
// physical objects.
//
// This function does not modify the multiset.
void multiset_print(const Multiset *multiset,void (*print_func)(const void *data));

#endif