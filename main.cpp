#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <charconv>
#include <functional>
#include "node.h"
#include "genericlookuptable.h"
#include "string"





int main(int argc, char* argv[])
{


    /*
    TODO.
    Write tests for linkedlistgetvalue, linkedlistsetvalue, linkedlistaddvalue
    such that they work on a empty lists, 1 element lists, n element lists

    Make it so that you don't have to explicitly pass in a linkedlist item to Execute

    Make a function to either update a linkedlist if value exists else just append new one 
    (because no syntactic difference between assign and vardecl)    
    */

using storage = LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"hey!", 9>>>;


    std::cout << LinkedListGetValue<storage, "a">::value << std::endl;
    std::cout << LinkedListGetValue<LinkedListSetValue<storage, "a", 40>::newList, "a">::value << std::endl;
    std::cout << LinkedListGetValue<LinkedListAddValue<storage, "new", 90>::newList, "new">::value << std::endl;


    using code = Execute<storage,
                         Assign<"x", Num<6>>,
                         Assign<"y", BinOpPlus<Num<2>, Num<7>>>,
                         Assign<"z", BinOpPlus<BinOpPlus<Num<2>, Num<10>>, Num<7>>>>;


    std::cout << LinkedListGetValue<code::values, "x">::value << std::endl;
    std::cout << LinkedListGetValue<code::values, "y">::value << std::endl;
    std::cout << LinkedListGetValue<code::values, "z">::value << std::endl;



    return 0;

    // using expression = BinOpPlus<Num<2>, BinOpPlus<Num<2>, Num<5>>>;
    using expression = BinOpPlus<Num<2>, Num<7>>;

    constexpr int result = expression::value;
    static_assert(result == 9);

    // static_assert(expression::value == 7);

    return 0;
  
}

