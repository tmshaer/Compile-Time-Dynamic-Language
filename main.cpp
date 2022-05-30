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

    using test = LinkedListNode<2, LinkedListNode<3>>;

    constexpr auto result2 = LinkedListGenericWalker<TestType2<int, 'a'>>::value;
    

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

