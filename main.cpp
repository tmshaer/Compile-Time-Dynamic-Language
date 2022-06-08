#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <charconv>
#include <functional>
#include "lookuptable.h"
#include "node.h"
#include "genericlookuptable.h"
#include "string"





int main(int argc, char* argv[])
{



    using storage = LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"hey!", 9>>>;


    std::cout << LinkedListGetValue<storage, "a">::value << std::endl;
    std::cout << LinkedListGetValue<LinkedListSetValue<storage, "a", 40>::newList, "a">::value << std::endl;
    std::cout << LinkedListGetValue<LinkedListAddValue<storage, "new", 90>::newList, "new">::value << std::endl;



    return 0;

    // using expression = BinOpPlus<Num<2>, BinOpPlus<Num<2>, Num<5>>>;
    using expression = BinOpPlus<Num<2>, Num<7>>;

    constexpr int result = expression::interpret();
    static_assert(result == 9);

    // static_assert(expression::value == 7);

    return 0;
  
}

