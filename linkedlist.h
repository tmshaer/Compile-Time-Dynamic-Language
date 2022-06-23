#include <iostream>
#include "specialtypes.h"

/*
Compile time datastructure used for mapping variable names to values.

First attempt is to create a linkedlist as a "lookup table" with a complexity of O(n)

Must support:
Identifier lookups
Registering new identifiers (and values)
*/

//https://stackoverflow.com/questions/6793259/how-does-one-implement-hash-tables-in-a-functional-language








//Linked list empty node type (used for matching end of linkedlist) 
struct LinkedListEmptyNode {};

//Linked list node
template<string_literal ID, auto Value, typename T = LinkedListEmptyNode>
struct LinkedListNode {};


/*
Gets value in linkedlist by id.
Will be used for lookups
aka
v = storage["x"]
*/

template<typename T>
struct[[deprecated]] test {

};


template<typename T, string_literal LookupID>
struct LinkedListGetValue {
    static const constexpr int value  = -1;

    // error
    //static_assert(!std::is_same_v<T, T>); + deprecated as message
};


template<template <string_literal, auto, typename> typename A, string_literal ID, auto Value, typename T>
struct LinkedListGetValue<A<ID, Value, T>, ID> {
    static const constexpr auto value  = Value;
};

template<template <string_literal, auto, typename> typename A, string_literal ID, auto Value, typename T, string_literal LookupID>
struct LinkedListGetValue<A<ID, Value, T>, LookupID> {
    static const constexpr auto value  = LinkedListGetValue<T, LookupID>::value;
};

/*
Adds new value to end of linked list.
Recreates the LinkedList with an extra value at the end.
Will be used for declaring variable
*/

template<typename T, string_literal NewID, auto NewValue>
struct LinkedListAddValue {
    using newList = T;
};

//edge case for when trying to add to a empty
template<string_literal NewID, auto NewValue>
struct LinkedListAddValue<LinkedListEmptyNode, NewID, NewValue> {
    using newList  = LinkedListNode<NewID, NewValue>;
};

// end case i.e. matched on LinkedListEmptyNode
template<template <string_literal, auto, typename> typename A, string_literal OldID, auto OldValue, string_literal NewID, auto NewValue>
struct LinkedListAddValue<A<OldID, OldValue, LinkedListEmptyNode>, NewID, NewValue> {
    using newList  = LinkedListNode<OldID, OldValue, LinkedListNode<NewID, NewValue>>;
};


template<template <string_literal, auto, typename> typename A, string_literal OldID, auto OldValue, typename T, string_literal NewID, auto NewValue>
struct LinkedListAddValue<A<OldID, OldValue, T>, NewID, NewValue> {
    using extract = LinkedListAddValue<T, NewID, NewValue>::newList;
    using newList  = LinkedListNode<OldID, OldValue, extract>;
};


/*
Change value in list by id
aka
storage["x"] = 2
*/

template<typename T, string_literal LookupID, auto NewValue>
struct LinkedListSetValue {
    using newList  = T;
};


//match case
template<template <string_literal, auto, typename> typename A, string_literal OldID, auto OldValue, typename T, auto NewValue>
struct LinkedListSetValue<A<OldID, OldValue, T>, OldID, NewValue> {
    using newList  = LinkedListNode<OldID, NewValue, T>;
};

template<template <string_literal, auto, typename> typename A, string_literal OldID, auto OldValue, typename T, string_literal LookupID, auto NewValue>
struct LinkedListSetValue<A<OldID, OldValue, T>, LookupID, NewValue> {
    using extract = LinkedListSetValue<T, LookupID, NewValue>::newList;
    using newList  = LinkedListNode<OldID, OldValue, extract>;

};

/*
Either sets value if it exists or adds new one
Helper function that combines set and add functionality into one

*/

template<typename T, string_literal LookupID, auto NewValue>
struct LinkedListSetOrAddValue {
    using newList  = T;
};


//end case couldn't find a value
template<string_literal LookupID, auto NewValue>
struct LinkedListSetOrAddValue<LinkedListEmptyNode, LookupID, NewValue> {
    using newList  = LinkedListNode<LookupID, NewValue>;
};


//match case
template<template <string_literal, auto, typename> typename A, string_literal OldID, auto OldValue, typename T, auto NewValue>
struct LinkedListSetOrAddValue<A<OldID, OldValue, T>, OldID, NewValue> {
    using newList  = LinkedListNode<OldID, NewValue, T>;
};

template<template <string_literal, auto, typename> typename A, string_literal OldID, auto OldValue, typename T, string_literal LookupID, auto NewValue>
struct LinkedListSetOrAddValue<A<OldID, OldValue, T>, LookupID, NewValue> {
    using extract = LinkedListSetOrAddValue<T, LookupID, NewValue>::newList;
    using newList  = LinkedListNode<OldID, OldValue, extract>;

};


void runLinkedListTests() {
    using storage0 = LinkedListEmptyNode;
    using storage1 = LinkedListNode<"a", 3>;
    using storageN = LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>;

    // get tests

    //0 cases
    static_assert(LinkedListGetValue<storage0, "a">::value  == -1); //invalid case

    //1 cases
    static_assert(LinkedListGetValue<storage1, "a">::value  == 3);
    static_assert(LinkedListGetValue<storage1, "b">::value  == -1); //invalid case

    // n cases
    static_assert(LinkedListGetValue<storageN, "a">::value  == 3);
    static_assert(LinkedListGetValue<storageN, "b">::value  == 4);
    static_assert(LinkedListGetValue<storageN, "c">::value  == 9);
    static_assert(LinkedListGetValue<storageN, "d">::value  == -1); //invalid case
    
    // set tests

    //0 cases
    static_assert(std::is_same_v<storage0, LinkedListSetValue<storage0, "a", 40>::newList>); //invalid case

    //1 cases
    static_assert(std::is_same_v<LinkedListNode<"a", 40>, LinkedListSetValue<storage1, "a", 40>::newList>); 
    static_assert(std::is_same_v<storage1, LinkedListSetValue<storage1, "b", 40>::newList>); //invalid case

    //n cases
    static_assert(std::is_same_v<LinkedListNode<"a", 40, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>, LinkedListSetValue<storageN, "a", 40>::newList>); 
    static_assert(std::is_same_v<LinkedListNode<"a", 3, LinkedListNode<"b", 40, LinkedListNode<"c", 9>>>, LinkedListSetValue<storageN, "b", 40>::newList>); 
    static_assert(std::is_same_v<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 40>>>, LinkedListSetValue<storageN, "c", 40>::newList>); 
    static_assert(std::is_same_v<storageN, LinkedListSetValue<storageN, "d", 40>::newList>);  //invalid case


    // add tests

    //0 cases
    static_assert(std::is_same_v<LinkedListNode<"a", 40>, LinkedListAddValue<storage0, "a", 40>::newList>);

    //1 cases
    static_assert(std::is_same_v<LinkedListNode<"a", 3, LinkedListNode<"b", 4>>, LinkedListAddValue<storage1, "b", 4>::newList>); 

    //n cases
    static_assert(std::is_same_v<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 9, LinkedListNode<"d", 12>>>>, LinkedListAddValue<storageN, "d", 12>::newList>); 


    // set or add tests

    //0 cases
    static_assert(std::is_same_v<LinkedListNode<"a", 40>, LinkedListSetOrAddValue<storage0, "a", 40>::newList>); 

    //1 cases
    static_assert(std::is_same_v<LinkedListNode<"a", 40>, LinkedListSetOrAddValue<storage1, "a", 40>::newList>); 
    static_assert(std::is_same_v<LinkedListNode<"a", 3, LinkedListNode<"b", 40>>, LinkedListSetOrAddValue<storage1, "b", 40>::newList>); //add case

    //n cases
    static_assert(std::is_same_v<LinkedListNode<"a", 40, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>, LinkedListSetOrAddValue<storageN, "a", 40>::newList>); 
    static_assert(std::is_same_v<LinkedListNode<"a", 3, LinkedListNode<"b", 40, LinkedListNode<"c", 9>>>, LinkedListSetOrAddValue<storageN, "b", 40>::newList>); 
    static_assert(std::is_same_v<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 40>>>, LinkedListSetOrAddValue<storageN, "c", 40>::newList>); 
    static_assert(std::is_same_v<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 9, LinkedListNode<"d", 40>>>>, LinkedListSetOrAddValue<storageN, "d", 40>::newList>);  //invalid case

}
