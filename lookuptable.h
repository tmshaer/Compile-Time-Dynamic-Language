

/*
Compile time datastructure used for mapping variable names to values.

First attempt is to create a linkedlist as a "lookup table" with a complexity of O(n)

Must support:
Identifier lookups
Registering new identifiers (and values)
*/

//https://stackoverflow.com/questions/6793259/how-does-one-implement-hash-tables-in-a-functional-language


struct LinkedListEmptyNode {

};


template<int ID, int Value, typename T = LinkedListEmptyNode>
struct LinkedListNode {
};


/*
Returns final value in linked list
Walks along it and uses pattern matching to find the final node
which will have its next node as LinkedListEmptyNode.
*/

template<typename T>
struct LinkedListGetFinalValue {
    static const int value  = -1;
};


// end case i.e. matched on LinkedListEmptyNode
template<template <int, int, typename> typename A, int ID, int Value>
struct LinkedListGetFinalValue<A<ID, Value, LinkedListEmptyNode>> {
    static const int value  = Value;
};


template<template <int, int, typename> typename A, int ID, int Value, typename T>
struct LinkedListGetFinalValue<A<ID, Value, T>> {
    static const int value  = LinkedListGetFinalValue<T>::value;
};

/*
Adds new value to end of linked list.
Recreates the LinkedList with an extra value at the end.
*/

template<typename T, int NewID, int NewValue>
struct LinkedListAddValue {
    using newList = T;
};

// end case i.e. matched on LinkedListEmptyNode
template<template <int, int, typename> typename A, int OldID, int OldValue, int NewID, int NewValue>
struct LinkedListAddValue<A<OldID, OldValue, LinkedListEmptyNode>, NewID, NewValue> {
    using newList  = LinkedListNode<OldID, OldValue, LinkedListNode<NewID, NewValue>>;
};

template<template <int, int, typename> typename A, int OldID, int OldValue, typename T, int NewID, int NewValue>
struct LinkedListAddValue<A<OldID, OldValue, T>, NewID, NewValue> {
    using extract = LinkedListAddValue<T, NewID, NewValue>::newList;
    using newList  = LinkedListNode<OldID, OldValue, extract>;
};


/*
Gets value in linkedlist by id
*/

template<typename T, int LookupID>
struct LinkedListGetValue {
    static const int value  = -1;
};


// end case i.e. matched on LinkedListEmptyNode
template<template <int, int, typename> typename A, int ID, int Value, int LookupID>
struct LinkedListGetValue<A<ID, Value, LinkedListEmptyNode>, LookupID> {
    static const int value  = -1;
};


// template<template <int, int, typename> typename A, int ID, int Value, typename T, int LookupID>
// struct LinkedListGetValue<A<LookupID, Value, T>, LookupID> {
//     static const int value  = Value;
// };


template<template <int, int, typename> typename A, int ID, int Value, typename T, int LookupID>
struct LinkedListGetValue<A<ID, Value, T>, LookupID> {
    static const int value  = LinkedListGetValue<T, LookupID>::value;
};
