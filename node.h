/*
Contains all the valid AST nodes for our language.
*/


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