// push

// pop

// walk through

// similar to linked list but without id + pop support



//Linked list empty node type (used for matching end of linkedlist) 
struct StackEmptyNode {};

//Linked list node
template<auto Value, typename T = StackEmptyNode>
struct StackNode {};


/*
Pushing to stack
*/

template<typename T, auto NewValue>
struct StackPushNode {
    using newList = T;
};

//edge case for when trying to add to a empty
template<auto NewValue>
struct StackPushNode<StackEmptyNode, NewValue> {
    using newList  = StackNode<NewValue>;
};


template<template <auto, typename> typename A, auto OldValue, typename T, auto NewValue>
struct StackPushNode<A<OldValue, T>, NewValue> {
    using newList  = StackNode<NewValue, A<OldValue, T>>;
};

/*
Pop node
*/

template<typename T>
struct StackPopNode {
    using newList = T;
};

//edge case for when trying to add to a empty
template<>
struct StackPopNode<StackEmptyNode> {
    using newList  = StackEmptyNode;
};


template<template <auto, typename> typename A, auto OldValue, typename T>
struct StackPopNode<A<OldValue, T>> {
    using newList  = T;
};


void runStackTests() {
    using storage0 = StackEmptyNode;
    using storage1 = StackNode<3>;
    using storageN = StackNode<3, StackNode<4, StackNode<9>>>;

    // push tests

    //0 cases
     static_assert(std::is_same_v<StackNode<40>, StackPushNode<storage0, 40>::newList>);

    //1 cases
     static_assert(std::is_same_v<StackNode<4, StackNode<3>>, StackPushNode<storage1, 4>::newList>); 

    //n cases
    static_assert(std::is_same_v<StackNode<12, StackNode<3, StackNode<4, StackNode<9>>>>, StackPushNode<storageN, 12>::newList>); 

    // pop tests

    //0 cases
     static_assert(std::is_same_v<StackEmptyNode, StackPopNode<storage0>::newList>);

     //1 cases
    static_assert(std::is_same_v<StackEmptyNode, StackPopNode<storage1>::newList>); 

    //n cases
    static_assert(std::is_same_v<StackNode<4, StackNode<9>>, StackPopNode<storageN>::newList>); 


}