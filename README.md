# Compile Time Dynamic Language


###### Aim of project: 
Implement a dynamically typed language in C++ that is evaluated 100% at compile time.

###### Example Usage

   // psudo code for below example here
   /*
    x = 6
    y = 2 + x
    z = (y * 10) + 7
    text = "Hello "
    text2 = text + "World!"
    bool = !true
    ifmodified = false
    if 1:
       temp = 10
       ifmodified = true
   */

    // code
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

    // tests
    static_assert(SymbolTableGetValue<NONE, code::values, "x">::value == 6); // basic non modified variable init
    static_assert(SymbolTableGetValue<NONE, code::values, "y">::value == 8); // basic evaluation of expression using Apply and ID lookup using Var
    static_assert(SymbolTableGetValue<NONE, code::values, "z">::value == 87); // chained expression test
    constexpr string_literal teststring = "Hello World!";
    static_assert(SymbolTableGetValue<NONE, code::values, "text2">::value == teststring); // string concat test
    static_assert(!SymbolTableGetValue<NONE, code::values, "bool">::value); // bool test
    static_assert(SymbolTableGetValue<NONE, code::values, "temp">::value == NONE); // GOOD we lose values on the temp if scope
    static_assert(SymbolTableGetValue<NONE, code::values, "ifmodified">::value); // GOOD we successfully modified variable in outer scope


###### Approach
The language will be implemented as a tree-walk interpreter which makes use of nodes that perform operations.  

###### Compiler
GCC-11 /std:c++20

###### Milestones:
1. Implement basic chainable expressions such as binary ops for float/number types ✔️
2. Implement variables using lookup table. ✔️
3. Implement boolean and string types. ✔️
4. Implement basic error messages for type related errors to expressions.
5. Implement basic control flow through if statements. ✔️
6. Implement for loops.
7. Implement methods.
8. Have language parsed through string literal instead of type declarations.
