#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <charconv>
#include <functional>
#include "lookuptable.h"
#include "node.h"
#include "genericlookuptable.h"







int main(int argc, char* argv[])
{

    using test = LinkedListNode<2, LinkedListNode<3, LinkedListNode<7>>>;


    std::cout << LinkedListGetFinalValue<test>::value << std::endl;
    std::cout << LinkedListGetFinalValue<LinkedListAddValue<test, 5>::newList>::value << std::endl;

    // constexpr auto result2 = LinkedListGetFinalValue<test>::value;
    

    // std::cout << result2 << std::endl;
    // static_assert(result2 == 3);


    return 0;

    // using expression = BinOpPlus<Num<2>, BinOpPlus<Num<2>, Num<5>>>;
    using expression = BinOpPlus<Num<2>, Num<7>>;

    constexpr int result = expression::interpret();
    static_assert(result == 9);

    // static_assert(expression::value == 7);

    return 0;
  
}

