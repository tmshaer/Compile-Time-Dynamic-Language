

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


template<int V, typename T = LinkedListEmptyNode>
struct LinkedListNode {
    static const int value = V;
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


template<template <int, typename> typename A, int C>
struct LinkedListGetFinalValue<A<C, LinkedListEmptyNode>> {
    static const int value  = C;
};

// int non type template template specilisation
template<template <int, typename> typename A, int C, typename T>
struct LinkedListGetFinalValue<A<C, T>> {
    static const int value  = LinkedListGetFinalValue<T>::value;
};

/*
Adds new value to end of linked list.
Recreates the LinkedList with an extra value at the end.
*/

template<typename T, int NewValue>
struct LinkedListAddValue {
    using newList = T;
};

template<template <int, typename> typename A, int C, int NewValue>
struct LinkedListAddValue<A<C, LinkedListEmptyNode>, NewValue> {
    using newList  = LinkedListNode<C, LinkedListNode<NewValue>>;
};

template<template <int, typename> typename A, int C, typename T, int NewValue>
struct LinkedListAddValue<A<C, T>, NewValue> {
    using extract = LinkedListAddValue<T, NewValue>::newList;
    using newList  = LinkedListNode<C, extract>;
};

