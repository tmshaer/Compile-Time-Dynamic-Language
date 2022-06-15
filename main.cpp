#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <charconv>
#include <functional>
#include "node.h"
#include "string"

void runTests() {
    using storage0 = LinkedListEmptyNode;
    using storage1 = LinkedListNode<"a", 3>;
    using storageN = LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>;

    // get tests

    //0 cases
    static_assert(LinkedListGetValue<storage0, "a">::value  == -1); //invalid case

    //1 cases
    static_assert(LinkedListGetValue<storage1, "a">::value  == 3);
    static_assert(LinkedListGetValue<storage1, "b">::value  == -1); //invalid case

    // n cases
    static_assert(LinkedListGetValue<storageN, "a">::value  == 3);
    static_assert(LinkedListGetValue<storageN, "b">::value  == 4);
    static_assert(LinkedListGetValue<storageN, "c">::value  == 9);
    static_assert(LinkedListGetValue<storageN, "d">::value  == -1); //invalid case
    
    // set tests

    //0 cases
    static_assert(std::is_same<storage0, LinkedListSetValue<storage0, "a", 40>::newList>::value); //invalid case

    //1 cases
    static_assert(std::is_same<LinkedListNode<"a", 40>, LinkedListSetValue<storage1, "a", 40>::newList>::value); 
    static_assert(std::is_same<storage1, LinkedListSetValue<storage1, "b", 40>::newList>::value); //invalid case

    //n cases
    static_assert(std::is_same<LinkedListNode<"a", 40, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>, LinkedListSetValue<storageN, "a", 40>::newList>::value); 
    static_assert(std::is_same<LinkedListNode<"a", 3, LinkedListNode<"b", 40, LinkedListNode<"c", 9>>>, LinkedListSetValue<storageN, "b", 40>::newList>::value); 
    static_assert(std::is_same<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 40>>>, LinkedListSetValue<storageN, "c", 40>::newList>::value); 
    static_assert(std::is_same<storageN, LinkedListSetValue<storageN, "d", 40>::newList>::value);  //invalid case


    // add tests

    //0 cases
    static_assert(std::is_same<LinkedListNode<"a", 40>, LinkedListAddValue<storage0, "a", 40>::newList>::value);

    //1 cases
    static_assert(std::is_same<LinkedListNode<"a", 3, LinkedListNode<"b", 4>>, LinkedListAddValue<storage1, "b", 4>::newList>::value); 

    //n cases
    static_assert(std::is_same<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 9, LinkedListNode<"d", 12>>>>, LinkedListAddValue<storageN, "d", 12>::newList>::value); 


    // set or add tests

    //0 cases
    static_assert(std::is_same<LinkedListNode<"a", 40>, LinkedListSetOrAddValue<storage0, "a", 40>::newList>::value); 

    //1 cases
    static_assert(std::is_same<LinkedListNode<"a", 40>, LinkedListSetOrAddValue<storage1, "a", 40>::newList>::value); 
    static_assert(std::is_same<LinkedListNode<"a", 3, LinkedListNode<"b", 40>>, LinkedListSetOrAddValue<storage1, "b", 40>::newList>::value); //add case

    //n cases
    static_assert(std::is_same<LinkedListNode<"a", 40, LinkedListNode<"b", 4, LinkedListNode<"c", 9>>>, LinkedListSetOrAddValue<storageN, "a", 40>::newList>::value); 
    static_assert(std::is_same<LinkedListNode<"a", 3, LinkedListNode<"b", 40, LinkedListNode<"c", 9>>>, LinkedListSetOrAddValue<storageN, "b", 40>::newList>::value); 
    static_assert(std::is_same<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 40>>>, LinkedListSetOrAddValue<storageN, "c", 40>::newList>::value); 
    static_assert(std::is_same<LinkedListNode<"a", 3, LinkedListNode<"b", 4, LinkedListNode<"c", 9, LinkedListNode<"d", 40>>>>, LinkedListSetOrAddValue<storageN, "d", 40>::newList>::value);  //invalid case

}



int main(int argc, char* argv[])
{

    runTests();
    /*
    TODO.
    Make it so that you don't have to explicitly pass in a linkedlist item to Execute
    */



    using code = Execute<LinkedListEmptyNode,
                         Assign<"x", ValNum<6>>,
                         Assign<"y", BinOpPlus<ValNum<2>, Var<"x">>>,
                         Assign<"z", BinOpPlus<BinOpPlus<Var<"y">, ValNum<10>>, ValNum<7>>>,
                         Assign<"a", ValStr<"Hello World!">>>;

    static_assert(LinkedListGetValue<code::values, "x">::value == 6);
    static_assert(LinkedListGetValue<code::values, "y">::value == 8);
    static_assert(LinkedListGetValue<code::values, "z">::value == 25);
    std::cout << LinkedListGetValue<code::values, "a">::value << std::endl;
    //static_assert(LinkedListGetValue<code::values, "z">::value == 25);

    // std::cout << LinkedListGetValue<code::values, "x">::value << std::endl;
    // std::cout << LinkedListGetValue<code::values, "y">::value << std::endl;
    // std::cout << LinkedListGetValue<code::values, "z">::value << std::endl;



    return 0;



    return 0;
  
}

