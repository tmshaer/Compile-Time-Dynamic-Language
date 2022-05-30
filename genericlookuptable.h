/*

Same as look up table but not specific to any type
*/



template<typename T>
struct LinkedListGenericWalker {
    static const int value  = 8;
};

//https://stackoverflow.com/questions/22486386/partially-specializing-a-non-type-template-parameter-of-dependent-type

// "template template" aka dependent types
// https://stackoverflow.com/questions/27372492/c-template-parameter-type-inference
// accept a nttp of any type

template<template <typename, auto> typename A, typename T, auto C>
struct LinkedListGenericWalker<A<T, C>> {
    static const decltype(C) value  = C;
};
