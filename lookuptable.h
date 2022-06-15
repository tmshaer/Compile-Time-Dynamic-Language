#include <iostream>

/*
Compile time datastructure used for mapping variable names to values.

First attempt is to create a linkedlist as a "lookup table" with a complexity of O(n)

Must support:
Identifier lookups
Registering new identifiers (and values)
*/

//https://stackoverflow.com/questions/6793259/how-does-one-implement-hash-tables-in-a-functional-language




/*
Compatible string literal compile time type (used for ids)

https://stackoverflow.com/questions/62853609/understanding-user-defined-string-literals-addition-for-c20
*/
template<size_t N>
struct string_literal
{
    std::array<char, N> arr_;

    constexpr string_literal(const char(&in)[N]) : arr_{}   
    {
        std::copy(in, in + N, arr_.begin());
    }

    friend std::ostream& operator<<(std::ostream& os, const string_literal& dt)
    {
        //std::string lol(arr_;
        os << std::string(dt.arr_.data());
        return os;
    }

};



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


template<typename T, string_literal LookupID>
struct LinkedListGetValue {
    static const constexpr int value  = -1;
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