#include "linkedlist.h"
#include "valuestack.h"
#include "typestack.h"
#include <type_traits>
#include <typeinfo>

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






template<typename T>
struct typetag {

    constexpr typetag() {}
    constexpr typetag(T l) {}

    using type = T;
};

/*
SymbolTableSetValue

Will replace value on the symbol table (given a id and val)
starting with the current scope.

If FAILS (can't find ID) returns the Input

*/


/*
Base case for symboltable get or add value
*/
template<typename NextStackNode, typename OriginalStackNode, string_literal LookupID, auto NewValue>
struct SymbolTableSetValue {
    using newStack  = OriginalStackNode;
};



/*
Recursive case find next one 
*/
template<template <typename, typename> typename CurrentStackNode, typename CurrentLinkedList, typename NextStackNode,
         typename OriginalStackNode,
         string_literal LookupID,
         auto NewValue>
struct SymbolTableSetValue<CurrentStackNode<CurrentLinkedList, NextStackNode>, OriginalStackNode, LookupID, NewValue> {
    
    
    static constexpr auto valueLookup = LinkedListGetValue<CurrentLinkedList, LookupID>::value;
    


    using newStack = typename decltype([]() {
        //                                                  DON'T FORGET CONST!
        if constexpr (std::is_same_v<decltype(valueLookup), const NoneValue>) {
            // no match, continue recursive descent
            return typetag<TypeStackNode<CurrentLinkedList, typename SymbolTableSetValue<NextStackNode, OriginalStackNode, LookupID, NewValue>::newStack>>{};
        } else {
            // match found | return updated linked list and stack node
            return typetag<TypeStackNode<typename LinkedListSetValue<CurrentLinkedList, LookupID, NewValue>::newList, NextStackNode>>{};
        }

    }())::type;

};


/*
Edge case supplied empty list
*/
template<string_literal LookupID,
         auto NewValue>
struct SymbolTableSetValue<TypeStackNode<LinkedListEmptyNode>, TypeStackNode<LinkedListEmptyNode>, LookupID, NewValue> {
    using newStack = TypeStackNode<LinkedListEmptyNode>;
};


/*
Edge case given a empty stack node
*/
template<typename T, string_literal LookupID,
         auto NewValue>
struct SymbolTableSetValue<TypeStackEmptyNode, T, LookupID, NewValue> {
    using newStack = TypeStackEmptyNode;
};


template<typename T, string_literal LookupID, auto NewValue>
struct SymbolTableDeclareOrAssignVariable {
    using newStack = T;
};


/*
Edge case if given a empty stack node just return basic result
*/
template<string_literal LookupID, auto NewValue>
struct SymbolTableDeclareOrAssignVariable<TypeStackEmptyNode, LookupID, NewValue> {
    using newStack = TypeStackNode<LinkedListNode<LookupID, NewValue>>;
};

/*
Edge case if given a empty stack node with empty linkedlist return basic result
*/
template<template<typename UNUSED, typename UNUSED2> typename T, string_literal LookupID, auto NewValue>
struct SymbolTableDeclareOrAssignVariable<T<LinkedListEmptyNode, TypeStackEmptyNode>, LookupID, NewValue> {
    
    using newStack = TypeStackNode<LinkedListNode<LookupID, NewValue>>;
};


/*
Main logic for SymbolTableDeclareOrAssignVariable

First attempts to lookup identifier on symbol table
If fails, updates current top stack node's linkedlist
Else uses SymbolTableSetValue to update symbol table where appropriate.

*/
template<template<typename, typename> typename T, typename CurrentLinkedList, typename NextStackNode, string_literal LookupID, auto NewValue>
struct SymbolTableDeclareOrAssignVariable<T<CurrentLinkedList, NextStackNode>, LookupID, NewValue> {
    
    static constexpr auto valueLookup = SymbolTableGetValue<NONE, T<CurrentLinkedList, NextStackNode>, LookupID>::value;

    using newStack = typename decltype([]() {
        //                                                  DON'T FORGET CONST!
        if constexpr (std::is_same_v<decltype(valueLookup), const NoneValue>) {
            // no match, declare on current stacknode
            return typetag<TypeStackNode<typename LinkedListAddValue<CurrentLinkedList, LookupID, NewValue>::newList, NextStackNode>>{};
        } else {
            // match found use SymbolTableSetValue to set value
            
            return typetag<typename SymbolTableSetValue<T<CurrentLinkedList, NextStackNode>, T<CurrentLinkedList, NextStackNode>, LookupID, NewValue>::newStack>{};
        }
    }())::type;
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


    using storageNNEmpty = TypeStackNode<LinkedListEmptyNode,
                           TypeStackNode<LinkedListEmptyNode, 
                           TypeStackNode<LinkedListEmptyNode>>>;


    /*
    SymbolTableGetValue tests
    */

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


    // multiple scopes empty
    static_assert(SymbolTableGetValue<NONE, storageNNEmpty, "a">::value == NONE);


    /*
    SymbolTableSetValue tests
    */


    // no scopes edge case
    static_assert(std::is_same_v<SymbolTableSetValue<storage0, storage0, "a", 5>::newStack, storage0>); // invalid case

    // one scope with no values
    static_assert(std::is_same_v<SymbolTableSetValue<storage10, storage10, "a", 5>::newStack, storage10>);


    // one scope with one value
    static_assert(std::is_same_v<SymbolTableSetValue<storage11, storage11, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10>>>); 
    static_assert(std::is_same_v<SymbolTableSetValue<storage11, storage11, "b", 10>::newStack, storage11>); // invalid case


    // one scope with multiple values

    static_assert(std::is_same_v<SymbolTableSetValue<storage1N, storage1N, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableSetValue<storage1N, storage1N, "b", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"b", 10, LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableSetValue<storage1N, storage1N, "c", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 10>>>>>); 
    static_assert(std::is_same_v<SymbolTableSetValue<storage1N, storage1N, "d", 10>::newStack, storage1N>); // invalid case 


    // multiple scopes None empty
    static_assert(std::is_same_v<SymbolTableSetValue<storageNN, storageNN, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNN, storageNN, "aa", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 10>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNN, storageNN, "b", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListNode<"b", 10>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNN, storageNN, "c", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 10>>>>>); 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNN, storageNN, "d", 10>::newStack, storageNN>); // invalid case

    // multiple scopes First empty
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyFirst, storageNNEmptyFirst, "a", 10>::newStack, storageNNEmptyFirst>); // invalid case 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyFirst, storageNNEmptyFirst, "aa", 10>::newStack, storageNNEmptyFirst>); // invalid case 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyFirst, storageNNEmptyFirst, "b", 10>::newStack, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"b", 10>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyFirst, storageNNEmptyFirst, "c", 10>::newStack, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 10>>>>>); 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyFirst, storageNNEmptyFirst, "d", 10>::newStack, storageNNEmptyFirst>); // invalid case 


    // multiple scopes middle empty
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyMiddle, storageNNEmptyMiddle, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"c", 9>>>>>); // invalid case 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyMiddle, storageNNEmptyMiddle, "aa", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 10>>, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"c", 9>>>>>); // invalid case 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyMiddle, storageNNEmptyMiddle, "b", 10>::newStack, storageNNEmptyMiddle>); // invalid case
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyMiddle, storageNNEmptyMiddle, "c", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"c", 10>>>>>); // invalid case 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyMiddle, storageNNEmptyMiddle, "d", 10>::newStack, storageNNEmptyMiddle>); // invalid case


    // multiple scopes end empty
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyEnd, storageNNEmptyEnd, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListEmptyNode>>>>); // invalid case 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyEnd, storageNNEmptyEnd, "aa", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 10>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListEmptyNode>>>>); // invalid case 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyEnd, storageNNEmptyEnd, "b", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListNode<"b", 10>, TypeStackNode<LinkedListEmptyNode>>>>); // invalid case 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyEnd, storageNNEmptyEnd, "c", 10>::newStack, storageNNEmptyEnd>); // invalid case 
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmptyEnd, storageNNEmptyEnd, "d", 10>::newStack, storageNNEmptyEnd>); // invalid case

    // multiple scopes empty
    static_assert(std::is_same_v<SymbolTableSetValue<storageNNEmpty, storageNNEmpty, "a", 10>::newStack, storageNNEmpty>); // invalid case 



    /*
    SymbolTableDeclareOrAssignVariable tests
    */

    // no scopes edge case
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storage0, "a", 5>::newStack, TypeStackNode<LinkedListNode<"a", 5>>>); // add case


    // one scope with no values
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storage10, "a", 5>::newStack, TypeStackNode<LinkedListNode<"a", 5>>>); // add case


    // one scope with one value
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storage11, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storage11, "b", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"b", 10>>>>); // add case


     // one scope with multiple values
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storage1N, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storage1N, "b", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"b", 10, LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storage1N, "c", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 10>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storage1N, "d", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 9, LinkedListNode<"d", 10>>>>>>); // add case 


    // multiple scopes None empty
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNN, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNN, "aa", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 10>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNN, "b", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListNode<"b", 10>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNN, "c", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 10>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNN, "d", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3, LinkedListNode<"d", 10>>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 9>>>>>); // ADD case

    // multiple scopes First empty
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyFirst, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyFirst, "aa", 10>::newStack, TypeStackNode<LinkedListNode<"aa", 10>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyFirst, "b", 10>::newStack, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"b", 10>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyFirst, "c", 10>::newStack, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 10>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyFirst, "d", 10>::newStack, TypeStackNode<LinkedListNode<"d", 10>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListNode<"c", 9>>>>>); 


    // multiple scopes middle empty
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyMiddle, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyMiddle, "aa", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 10>>, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyMiddle, "b", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3,  LinkedListNode<"b", 10>>>, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"c", 9>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyMiddle, "c", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"c", 10>>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyMiddle, "d", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3,  LinkedListNode<"d", 10>>>, TypeStackNode<LinkedListEmptyNode, TypeStackNode<LinkedListNode<"c", 9>>>>>); 


    // multiple scopes end empty
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyEnd, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListEmptyNode>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyEnd, "aa", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 10>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListEmptyNode>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyEnd, "b", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3>>, TypeStackNode<LinkedListNode<"b", 10>, TypeStackNode<LinkedListEmptyNode>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyEnd, "c", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3, LinkedListNode<"c", 10>>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListEmptyNode>>>>); 
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmptyEnd, "d", 10>::newStack, TypeStackNode<LinkedListNode<"a", 3, LinkedListNode<"aa", 3, LinkedListNode<"d", 10>>>, TypeStackNode<LinkedListNode<"b", 4>, TypeStackNode<LinkedListEmptyNode>>>>); 

    // multiple scopes empty
    static_assert(std::is_same_v<SymbolTableDeclareOrAssignVariable<storageNNEmpty, "a", 10>::newStack, TypeStackNode<LinkedListNode<"a", 10>,
                                                                                                            TypeStackNode<LinkedListEmptyNode, 
                                                                                                            TypeStackNode<LinkedListEmptyNode>>>>); 


}