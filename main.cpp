#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <charconv>
#include <functional>
#include "node.h"
#include "string"


int main(int argc, char* argv[])
{

    runValueStackTests();
    runLinkedListTests();
    /*
    TODO.
    Make it so that you don't have to explicitly pass in a linkedlist item to Execute.
    Add scoping.

    How to do scoping:

    For each scope pass a new symbol table on to it.
    If an attempt is made to modify a variable, first check existing scope's. -> can be done through some kind of stack data structure.
    If not found on exsting scope, then check local scope and if still not found then declare it then.

    Variables do same approach.


    */


   /*
    x = 6
    y = 2 + x
    z = (y * 10) + 7
    text = "Hello "
    text2 = text + "World!"
    bool = !true
   */

    using code = Execute<LinkedListEmptyNode,
                         Assign<"x", Val<6>>,
                         Assign<"y", Apply<std::plus<>, Val<2>, Var<"x">>>,
                         Assign<"z", Apply<std::plus<>, Apply<std::multiplies<>, Var<"y">, Val<10>>, Val<7>>>,
                         Assign<"text", ValStr<"Hello ">>,
                         Assign<"text2", Apply<std::plus<>, Var<"text">, ValStr<"World!">>>,
                         Assign<"bool", Apply<std::logical_not<>, Val<true>>>>;
                         //If<Val<true>, 
                            //Assign<"g", Val<5>>>>;

    static_assert(LinkedListGetValue<code::values, "x">::value == 6);
    static_assert(LinkedListGetValue<code::values, "y">::value == 8);
    static_assert(LinkedListGetValue<code::values, "z">::value == 87);
    constexpr string_literal teststring = "Hello World!";
    static_assert(LinkedListGetValue<code::values, "text2">::value == teststring);
    static_assert(!LinkedListGetValue<code::values, "bool">::value);




    return 0;



    return 0;
  
}

