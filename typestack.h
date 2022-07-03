#include <iostream>


// push

// pop

// walk through

// similar to linked list but without id + pop support



//Linked list empty node type (used for matching end of linkedlist) 
struct TypeStackEmptyNode {};

//TypeStack node
template<typename Value, typename T = TypeStackEmptyNode>
struct TypeStackNode {};



/*
Pushing to TypeStack
*/

template<typename T, typename NewValue>
struct TypeStackPushNode {
    using newList = T;
};

//edge case for when trying to add to a empty
template<typename NewValue>
struct TypeStackPushNode<TypeStackEmptyNode, NewValue> {
    using newList  = TypeStackNode<NewValue>;
};


template<template <typename, typename> typename A, typename OldValue, typename T, typename NewValue>
struct TypeStackPushNode<A<OldValue, T>, NewValue> {
    using newList  = TypeStackNode<NewValue, A<OldValue, T>>;
};

/*
Pop node
*/

template<typename T>
struct TypeStackPopNode {
    using newList = T;
};

// //edge case for when trying to add to a empty
template<>
struct TypeStackPopNode<TypeStackEmptyNode> {
    using newList  = TypeStackEmptyNode;
};


template<template <typename, typename> typename A, typename OldValue, typename T>
struct TypeStackPopNode<A<OldValue, T>> {
    using newList  = T;
};


void runTypeStackTests() {
    using storage0 = TypeStackEmptyNode;
    using storage1 = TypeStackNode<int>;
    using storageN = TypeStackNode<int, TypeStackNode<float, TypeStackNode<double>>>;

    // push tests

    //0 cases
    static_assert(std::is_same_v<TypeStackNode<int>, TypeStackPushNode<storage0, int>::newList>);

    // //1 cases
    static_assert(std::is_same_v<TypeStackNode<float, TypeStackNode<int>>, TypeStackPushNode<storage1, float>::newList>); 

    // //n cases
    static_assert(std::is_same_v<TypeStackNode<std::string, TypeStackNode<int, TypeStackNode<float, TypeStackNode<double>>>>, TypeStackPushNode<storageN, std::string>::newList>); 

    // // pop tests

    // //0 cases
    static_assert(std::is_same_v<TypeStackEmptyNode, TypeStackPopNode<storage0>::newList>);

    //  //1 cases
    static_assert(std::is_same_v<TypeStackEmptyNode, TypeStackPopNode<storage1>::newList>); 

    // //n cases
    static_assert(std::is_same_v<TypeStackNode<float, TypeStackNode<double>>, TypeStackPopNode<storageN>::newList>); 
}