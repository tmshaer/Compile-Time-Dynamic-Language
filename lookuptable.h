

/*
Compile time datastructure used for mapping variable names to values.

First attempt is to create a linkedlist as a "lookup table" with a complexity of O(n)

Must support:
Identifier lookups
Registering new identifiers (and values)
*/

//https://stackoverflow.com/questions/6793259/how-does-one-implement-hash-tables-in-a-functional-language



//https://stackoverflow.com/questions/62853609/understanding-user-defined-string-literals-addition-for-c20


template<size_t N>
struct string_literal
{
    std::array<char, N> arr_;

    constexpr string_literal(const char(&in)[N]) : arr_{}   
    {
        std::copy(in, in + N, arr_.begin());
    }
};


struct LinkedListEmptyNode {

};


template<string_literal ID, int Value, typename T = LinkedListEmptyNode>
struct LinkedListNode {
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


// end case i.e. matched on LinkedListEmptyNode
template<template <string_literal, int, typename> typename A, string_literal ID, int Value>
struct LinkedListGetFinalValue<A<ID, Value, LinkedListEmptyNode>> {
    static const int value  = Value;
};


template<template <string_literal, int, typename> typename A, string_literal ID, int Value, typename T>
struct LinkedListGetFinalValue<A<ID, Value, T>> {
    static const int value  = LinkedListGetFinalValue<T>::value;
};

/*
Adds new value to end of linked list.
Recreates the LinkedList with an extra value at the end.
*/

template<typename T, string_literal NewID, int NewValue>
struct LinkedListAddValue {
    using newList = T;
};

// end case i.e. matched on LinkedListEmptyNode
template<template <string_literal, int, typename> typename A, string_literal OldID, int OldValue, string_literal NewID, int NewValue>
struct LinkedListAddValue<A<OldID, OldValue, LinkedListEmptyNode>, NewID, NewValue> {
    using newList  = LinkedListNode<OldID, OldValue, LinkedListNode<NewID, NewValue>>;
};

template<template <string_literal, int, typename> typename A, string_literal OldID, int OldValue, typename T, string_literal NewID, int NewValue>
struct LinkedListAddValue<A<OldID, OldValue, T>, NewID, NewValue> {
    using extract = LinkedListAddValue<T, NewID, NewValue>::newList;
    using newList  = LinkedListNode<OldID, OldValue, extract>;
};


/*
Gets value in linkedlist by id.

Will be used for lookups
*/


template<typename T, string_literal LookupID>
struct LinkedListGetValue {
    static const int value  = -1;
};


template<template <string_literal, int, typename> typename A, string_literal ID, int Value, typename T>
struct LinkedListGetValue<A<ID, Value, T>, ID> {
    static const int value  = Value;
};

template<template <string_literal, int, typename> typename A, string_literal ID, int Value, typename T, string_literal LookupID>
struct LinkedListGetValue<A<ID, Value, T>, LookupID> {
    static const int value  = LinkedListGetValue<T, LookupID>::value;
};
