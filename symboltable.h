#include "linkedlist.h"
#include "valuestack.h"
#include "typestack.h"

/*

Symbol table is a stack of linked lists.
The top node of the stack is the current scope.

Enter scope.
When entering a scope, we push new stack node containing a linkedlist onto the stack.

Leave scope.
When leaving a scope, we pop the stack.

egg
Referencing a variable.
Starting from top of stack, we search each linkedlist for the id

Assigning a variable.
Starting from top of stack, we search each linkedlist for a id and update it if we find it.
If we can't find the id, then go back to top of stack and declare variable on that (current scope basically)
*/


/*
Case when match
*/
template<auto CurrentValue, typename StackNode, string_literal LookupID>
struct SymbolTableGetValue {
    static const constexpr auto value  = CurrentValue;
};


/*
Case when not in current scope and there is a enclosing scope
*/
template<template <typename, typename> typename StackNode, typename CurrentLinkedList, typename NextStackNode, string_literal LookupID>
struct SymbolTableGetValue<NONE, StackNode<CurrentLinkedList, NextStackNode>, LookupID> {
    static const constexpr auto value  = SymbolTableGetValue<LinkedListGetValue<CurrentLinkedList, LookupID>::value, NextStackNode, LookupID>::value;
};


/*
Case when not in current scope and there is no enclosing scope
*/
template<template <typename, typename> typename StackNode, typename CurrentLinkedList,  string_literal LookupID>
struct SymbolTableGetValue<NONE, StackNode<CurrentLinkedList, TypeStackEmptyNode>, LookupID> {
    static const constexpr auto value  = LinkedListGetValue<CurrentLinkedList, LookupID>::value;
};

/*
Case when no scope have been declared yet
*/
template<auto Value,  string_literal LookupID>
struct SymbolTableGetValue<Value, TypeStackEmptyNode, LookupID> {
    static const constexpr auto value  = NONE;
};


/*
Base case for symboltable get or add value
*/
template<auto CurrentValue, typename NextStackNode, typename OriginalStackNode, string_literal LookupID, auto NewValue>
struct SymbolTableSetOrAddValue {
    using newStack  = OriginalStackNode;
};


/*
Found lookup id case.
Return new stack node
*/
template<auto Value,
         template <typename, typename> typename CurrentStackNode, typename CurrentLinkedList, typename NextStackNode,
         typename OriginalStackNode,
         string_literal LookupID,
         auto NewValue>
struct SymbolTableSetOrAddValue<Value, CurrentStackNode<CurrentLinkedList, NextStackNode>, OriginalStackNode, LookupID, NewValue> {
    using newLinkedList = LinkedListSetValue<CurrentLinkedList, LookupID, NewValue>::newList;
    using newStack  = TypeStackNode<newLinkedList, NextStackNode>;
};

/*
Recursive case find next one 
*/
template<template <typename, typename> typename CurrentStackNode, typename CurrentLinkedList, typename NextStackNode,
         typename OriginalStackNode,
         string_literal LookupID,
         auto NewValue>
struct SymbolTableSetOrAddValue<NONE, CurrentStackNode<CurrentLinkedList, NextStackNode>, OriginalStackNode, LookupID, NewValue> {
    static auto valueLookup = LinkedListGetValue<CurrentLinkedList, LookupID>::value;
    using newNextStackNode  = SymbolTableSetOrAddValue<valueLookup,  NextStackNode, OriginalStackNode, LookupID, NewValue>::newStack;
    using newStack = TypeStackNode<CurrentLinkedList, newNextStackNode>;
};


/*
Case couldn't find identifier
*/
template<template <typename, typename> typename OriginalStackNode, typename CurrentLinkedList, typename NextStackNode,
         string_literal LookupID,
         auto NewValue>
struct SymbolTableSetOrAddValue<NONE, TypeStackEmptyNode, OriginalStackNode<CurrentLinkedList, NextStackNode>, LookupID, NewValue> {
    using newLinkedList = LinkedListSetValue<CurrentLinkedList, LookupID, NewValue>::newList;
    using newStack = TypeStackNode<newLinkedList, NextStackNode>;
};


/*
Edge case supplied empty list
*/
template<string_literal LookupID,
         auto NewValue>
struct SymbolTableSetOrAddValue<NONE, TypeStackNode<LinkedListEmptyNode>, TypeStackNode<LinkedListEmptyNode>, LookupID, NewValue> {
    using newStack = TypeStackNode<LinkedListNode<LookupID, NewValue>>;
};


/*
Edge case nothing passed
*/
template<string_literal LookupID,
         auto NewValue>
struct SymbolTableSetOrAddValue<NONE, TypeStackEmptyNode, TypeStackEmptyNode, LookupID, NewValue> {
    using newStack = TypeStackNode<LinkedListNode<LookupID, NewValue>>;
};




void runSymbolTableTests() {
    using storage0 = TypeStackEmptyNode;
    using storage10 = TypeStackNode<LinkedListEmptyNode>;
    using storage11 = TypeStackNode<LinkedListNode<"a", 3>>;
    using storage1N = TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>>;


    using storageNN = TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>,
                      TypeStackNode<LinkedListNode<"b", 4>, 
                      TypeStackNode<LinkedListNode<"c", 9>>>>;


    using storageNNEmptyFirst = TypeStackNode<LinkedListEmptyNode,
                      TypeStackNode<LinkedListNode<"b", 4>, 
                      TypeStackNode<LinkedListNode<"c", 9>>>>;


    using storageNNEmptyMiddle = TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>,
                      TypeStackNode<LinkedListEmptyNode, 
                      TypeStackNode<LinkedListNode<"c", 9>>>>;


    using storageNNEmptyEnd = TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>,
                      TypeStackNode<LinkedListNode<"b", 4>, 
                      TypeStackNode<LinkedListEmptyNode>>>;


    // no scopes edge case
    static_assert(SymbolTableGetValue<NONE, storage0, "a">::value == NONE); // invalid case

    // one scope with no values
    static_assert(SymbolTableGetValue<NONE, storage10, "a">::value == NONE); // invalid case

    // one scope with one value
    static_assert(SymbolTableGetValue<NONE, storage11, "a">::value == 3);
    static_assert(SymbolTableGetValue<NONE, storage11, "b">::value == NONE); // invalid case

    // one scope with multiple values
    static_assert(SymbolTableGetValue<NONE, storage1N, "a">::value == 3);
    static_assert(SymbolTableGetValue<NONE, storage1N, "b">::value == 4);
    static_assert(SymbolTableGetValue<NONE, storage1N, "c">::value == 9);
    static_assert(SymbolTableGetValue<NONE, storage1N, "d">::value == NONE); // invalid case

    // multiple scopes None empty
    static_assert(SymbolTableGetValue<NONE, storageNN, "a">::value == 3);
    static_assert(SymbolTableGetValue<NONE, storageNN, "aa">::value == 3);
    static_assert(SymbolTableGetValue<NONE, storageNN, "b">::value == 4);
    static_assert(SymbolTableGetValue<NONE, storageNN, "c">::value == 9);
    static_assert(SymbolTableGetValue<NONE, storageNN, "d">::value == NONE);


    // multiple scopes First empty
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyFirst, "a">::value == NONE);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyFirst, "aa">::value == NONE);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyFirst, "b">::value == 4);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyFirst, "c">::value == 9);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyFirst, "d">::value == NONE);


    // multiple scopes middle empty
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyMiddle, "a">::value == 3);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyMiddle, "aa">::value == 3);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyMiddle, "b">::value == NONE);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyMiddle, "c">::value == 9);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyMiddle, "d">::value == NONE);


    // multiple scopes end empty
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyEnd, "a">::value == 3);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyEnd, "aa">::value == 3);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyEnd, "b">::value == 4);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyEnd, "c">::value == NONE);
    static_assert(SymbolTableGetValue<NONE, storageNNEmptyEnd, "d">::value == NONE);

    /*
    using storage0 = TypeStackEmptyNode;
    using storage10 = TypeStackNode<LinkedListEmptyNode>;
    using storage11 = TypeStackNode<LinkedListNode<"a", 3>>;
    using storage1N = TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>>;


    using storageNN = TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>,
                      TypeStackNode<LinkedListNode<"b", 4>, 
                      TypeStackNode<LinkedListNode<"c", 9>>>>;


    using storageNNEmptyFirst = TypeStackNode<LinkedListEmptyNode,
                      TypeStackNode<LinkedListNode<"b", 4>, 
                      TypeStackNode<LinkedListNode<"c", 9>>>>;


    using storageNNEmptyMiddle = TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>,
                      TypeStackNode<LinkedListEmptyNode, 
                      TypeStackNode<LinkedListNode<"c", 9>>>>;


    using storageNNEmptyEnd = TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>,
                      TypeStackNode<LinkedListNode<"b", 4>, 
                      TypeStackNode<LinkedListEmptyNode>>>;
    */


    // no scopes edge case
    static_assert(std::is_same_v<SymbolTableSetOrAddValue<NONE, storage0, storage0, "a", 5>::newStack, TypeStackNode<LinkedListNode<"a", 5>>>);

    // one scope with no values

    static_assert(std::is_same_v<SymbolTableSetOrAddValue<NONE, storage10, storage10, "a", 5>::newStack, TypeStackNode<LinkedListNode<"a", 5>>>);


    // static_assert(SymbolTableGetValue<NONE, storage10, "a">::value == NONE); // invalid case

    // one scope with one value
    // static_assert(SymbolTableGetValue<NONE, storage11, "a">::value == 3);
    // static_assert(SymbolTableGetValue<NONE, storage11, "b">::value == NONE); // invalid case

    // one scope with multiple values
    // static_assert(SymbolTableGetValue<NONE, storage1N, "a">::value == 3);
    // static_assert(SymbolTableGetValue<NONE, storage1N, "b">::value == 4);
    // static_assert(SymbolTableGetValue<NONE, storage1N, "c">::value == 9);
    // static_assert(SymbolTableGetValue<NONE, storage1N, "d">::value == NONE); // invalid case

    // multiple scopes None empty
    // static_assert(SymbolTableGetValue<NONE, storageNN, "a">::value == 3);
    // static_assert(SymbolTableGetValue<NONE, storageNN, "aa">::value == 3);
    // static_assert(SymbolTableGetValue<NONE, storageNN, "b">::value == 4);
    // static_assert(SymbolTableGetValue<NONE, storageNN, "c">::value == 9);
    // static_assert(SymbolTableGetValue<NONE, storageNN, "d">::value == NONE);


    // multiple scopes First empty
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyFirst, "a">::value == NONE);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyFirst, "aa">::value == NONE);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyFirst, "b">::value == 4);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyFirst, "c">::value == 9);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyFirst, "d">::value == NONE);


    // multiple scopes middle empty
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyMiddle, "a">::value == 3);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyMiddle, "aa">::value == 3);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyMiddle, "b">::value == NONE);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyMiddle, "c">::value == 9);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyMiddle, "d">::value == NONE);


    // multiple scopes end empty
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyEnd, "a">::value == 3);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyEnd, "aa">::value == 3);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyEnd, "b">::value == 4);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyEnd, "c">::value == NONE);
    // static_assert(SymbolTableGetValue<NONE, storageNNEmptyEnd, "d">::value == NONE);

}