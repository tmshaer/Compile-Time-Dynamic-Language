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



void runSymbolTableTests() {
    using storage0 = TypeStackEmptyNode;
    using storage10 = TypeStackNode<LinkedListEmptyNode>;
    using storage11 = TypeStackNode<LinkedListNode<"a", 3>>;
    using storage1N = TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>>;

    using storageNN = TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>,
                      TypeStackNode<LinkedListNode<"b", 4>, 
                      TypeStackNode<LinkedListNode<"c", 9>>>>;

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

    // multiple scopes
    static_assert(SymbolTableGetValue<NONE, storageNN, "a">::value == 3);
    static_assert(SymbolTableGetValue<NONE, storageNN, "aa">::value == 3);
    static_assert(SymbolTableGetValue<NONE, storageNN, "b">::value == 4);
    static_assert(SymbolTableGetValue<NONE, storageNN, "c">::value == 9);
    static_assert(SymbolTableGetValue<NONE, storageNN, "d">::value == NONE);





}