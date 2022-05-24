#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <charconv>





template<int N>
struct Num {
     static const int value = N;
};



template<typename A, typename B>
struct BinOp {
    // static const int value = A::value + B::value;

    static constexpr const int evaluate() {
        return A::value + B::value;
    }
};



int main(int argc, char* argv[])
{

    using expression = BinOp<Num<2>, Num<5>>;

    constexpr int result = expression::evaluate();
    static_assert(result == 7);

    // static_assert(expression::value == 7);

    return 0;
  
}

