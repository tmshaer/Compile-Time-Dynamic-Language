// push

// pop

// walk through

// similar to linked list but without id + pop support



//Linked list empty node type (used for matching end of linkedlist) 
struct ValueStackEmptyNode {};

//ValueStack node
template<auto Value, typename T = ValueStackEmptyNode>
struct ValueStackNode {};



/*
Pushing to ValueStack
*/

template<typename T, auto NewValue>
struct ValueStackPushNode {
    using newList = T;
};

//edge case for when trying to add to a empty
template<auto NewValue>
struct ValueStackPushNode<ValueStackEmptyNode, NewValue> {
    using newList  = ValueStackNode<NewValue>;
};


template<template <auto, typename> typename A, auto OldValue, typename T, auto NewValue>
struct ValueStackPushNode<A<OldValue, T>, NewValue> {
    using newList  = ValueStackNode<NewValue, A<OldValue, T>>;
};

/*
Pop node
*/

template<typename T>
struct ValueStackPopNode {
    using newList = T;
};

//edge case for when trying to add to a empty
template<>
struct ValueStackPopNode<ValueStackEmptyNode> {
    using newList  = ValueStackEmptyNode;
};


template<template <auto, typename> typename A, auto OldValue, typename T>
struct ValueStackPopNode<A<OldValue, T>> {
    using newList  = T;
};


void runValueStackTests() {
    using storage0 = ValueStackEmptyNode;
    using storage1 = ValueStackNode<3>;
    using storageN = ValueStackNode<3, ValueStackNode<4, ValueStackNode<9>>>;

    // push tests

    //0 cases
     static_assert(std::is_same_v<ValueStackNode<40>, ValueStackPushNode<storage0, 40>::newList>);

    // //1 cases
     static_assert(std::is_same_v<ValueStackNode<4, ValueStackNode<3>>, ValueStackPushNode<storage1, 4>::newList>); 

    // //n cases
    static_assert(std::is_same_v<ValueStackNode<12, ValueStackNode<3, ValueStackNode<4, ValueStackNode<9>>>>, ValueStackPushNode<storageN, 12>::newList>); 

    // // pop tests

    // //0 cases
     static_assert(std::is_same_v<ValueStackEmptyNode, ValueStackPopNode<storage0>::newList>);

    //  //1 cases
    static_assert(std::is_same_v<ValueStackEmptyNode, ValueStackPopNode<storage1>::newList>); 

    // //n cases
    static_assert(std::is_same_v<ValueStackNode<4, ValueStackNode<9>>, ValueStackPopNode<storageN>::newList>); 
}