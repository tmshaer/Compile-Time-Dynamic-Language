#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <charconv>
#include <functional>





template<int N>
struct Num {
    //  static const int value = N;
    // using value = N;

     static constexpr const int interpret() {
        return N;
    }
};



template<typename A, typename B>
struct BinOpPlus {
    // static const int value = A::value + B::value;
    // using value = Num<A::value + B::value>;

    static constexpr const int interpret() {
        return A::interpret() + B::interpret();
    }
};



struct LinkedListEmptyNode {

};


// template<typename T>
// struct LinkedListNode {
// };


template<int V, typename T = LinkedListEmptyNode>
struct LinkedListNode {
    using nextType = T;
    static const int value = V;
};


template<typename T, typename A>
struct TestType {

};


template<typename T>
struct LinkedListWalker {
    static const int value  = 8;
};

/*
Has next node
*/
template<template <typename, typename ...> typename A, typename T, typename ...Args>
struct LinkedListWalker<A<T, Args...>> {
    static const int value  = 7;
};




//https://stackoverflow.com/questions/6793259/how-does-one-implement-hash-tables-in-a-functional-language


int main(int argc, char* argv[])
{

    using test = LinkedListNode<2, LinkedListNode<3, LinkedListNode<-1>>>;

    constexpr int result2 = LinkedListWalker<TestType<int, int>>::value;
    

    std::cout << result2 << std::endl;
    // static_assert(result2 == 3);


    return 0;

    // using expression = BinOpPlus<Num<2>, BinOpPlus<Num<2>, Num<5>>>;
    using expression = BinOpPlus<Num<2>, Num<7>>;

    constexpr int result = expression::interpret();
    static_assert(result == 9);

    // static_assert(expression::value == 7);

    return 0;
  
}

