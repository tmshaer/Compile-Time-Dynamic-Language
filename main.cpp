#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <charconv>
#include <functional>
#include "string"
#include "parser.h"



template<typename T>
struct TestClass {

    constexpr TestClass() {
        lol<T>();
    }

    constexpr T lol() {
        return 4;
    }

};

// CTAD via class method

// // Here's a deduction guide for our pair class
// // pair objects initialized with arguments of type T and U should deduce to pair<T, U>
// template <typename T, typename U>
// TestClass() -> pair<T, U>;

int main(int argc, char* argv[])
{



    constexpr char example_text[] = "3";
    constexpr auto cres = p.parse(cstring_buffer(example_text)); // notice cstring_buffer and no std::err output
    constexpr bool success = cres.has_value();
    static_assert(success);


    constexpr char example_text2[] = "4";
    constexpr auto cres2 = p.parse(cstring_buffer(example_text2)); // notice cstring_buffer and no std::err output
    constexpr bool success2 = cres2.has_value();
    static_assert(success2);

    std::cout << cres.value() << std::endl;
    std::cout << cres2.value() << std::endl;


    
    //if (success)
    //    std::cout << cres.value() << std::endl;


    return 0;

    // constexpr char example_text[] = "1 + 40";

    // constexpr auto cres = p.parse(cstring_buffer(example_text)); // notice cstring_buffer and no std::err output
    // // constexpr int result = cres.value();
    // // static_assert(result == 5);

    // bool success = cres.has_value();
    // if (success)
    //     std::cout << cres.value().evaluate() << std::endl;
    
    // return 0;




    runValueStackTests();
    runLinkedListTests();

   /*
    x = 6
    y = 2 + x
    z = (y * 10) + 7
    text = "Hello "
    text2 = text + "World!"
    bool = !true
   */

    using code = Execute<TypeStackEmptyNode,
                         Assign<"x", Val<6>>,
                         Assign<"y", Apply<std::plus<>, Val<2>, Var<"x">>>,
                         Assign<"z", Apply<std::plus<>, Apply<std::multiplies<>, Var<"y">, Val<10>>, Val<7>>>,
                         Assign<"text", ValStr<"Hello ">>,
                         Assign<"text2", Apply<std::plus<>, Var<"text">, ValStr<"World!">>>,
                         Assign<"bool", Apply<std::logical_not<>, Val<true>>>,
                         Assign<"ifmodified", Val<false>>,
                         If<Val<1>, 
                            Assign<"temp", Val<10>>,
                            Assign<"ifmodified", Val<true>>>>;


    constexpr typetag test123{code{}};

    using extractedType = decltype(test123)::type;

    static_assert(SymbolTableGetValue<NONE, extractedType::values, "y">::value == 8); // basic evaluation of expression using Apply and ID lookup using Var

    return 0;

    static_assert(SymbolTableGetValue<NONE, code::values, "x">::value == 6); // basic non modified variable init
    static_assert(SymbolTableGetValue<NONE, code::values, "y">::value == 8); // basic evaluation of expression using Apply and ID lookup using Var
    static_assert(SymbolTableGetValue<NONE, code::values, "z">::value == 87); // chained expression test
    constexpr string_literal teststring = "Hello World!";
    static_assert(SymbolTableGetValue<NONE, code::values, "text2">::value == teststring); // string concat test
    static_assert(!SymbolTableGetValue<NONE, code::values, "bool">::value); // bool test
    static_assert(SymbolTableGetValue<NONE, code::values, "temp">::value == NONE); // GOOD we lose values on the temp if scope
    static_assert(SymbolTableGetValue<NONE, code::values, "ifmodified">::value); // GOOD we successfully modified variable in outer scope

    return 0;
}

