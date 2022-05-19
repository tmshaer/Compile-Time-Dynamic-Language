#include <iostream>
#include <vector>
#include <string>


class Node {
public:

    const int a = 2;
private:

};

class Number : public Node {
public:

    constexpr Number(const int _value) : value (_value) {}

    const int value;

private:

};


class BinOp {
public:

private:

};


// https://en.cppreference.com/w/cpp/meta

// https://medium.com/@samathy_barratt/constantly-confusing-c-const-and-constexpr-pointer-behaviour-7eddb041e15a
int main() {

    constexpr const Number n(5);
    static_assert(n.value == 5);

    constexpr int a = 3 + 2;
    
    const static Node node;

    static const constexpr Node* nodepointer = &node;
    
    //const Node* b = new Node();
    static_assert(a == 5);
    
    constexpr const int extract = nodepointer->a;
    static_assert(extract == 2);
    //static_assert(nodepointer->a == 2);
    //static_assert(nodepointer != nullptr);

    //static_assert(a != 5);

}