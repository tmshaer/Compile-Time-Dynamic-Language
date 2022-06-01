

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
    using nextType = T;
    static const int value = V;
};


template<typename T, typename A>
struct TestType {

};


template<typename T, auto A>
struct TestType2 {

};

template<typename T>
struct LinkedListWalker {
    static const int value  = 8;
};


template<template <int, typename> typename A, int C>
struct LinkedListWalker<A<C, LinkedListEmptyNode>> {
    static const int value  = C;
};

// int non type template template specilisation
template<template <int, typename> typename A, int C, typename T>
struct LinkedListWalker<A<C, T>> {
    static const int value  = LinkedListWalker<T>::value;
};


