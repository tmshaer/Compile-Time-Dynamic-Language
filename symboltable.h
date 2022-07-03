#include "linkedlist.h"
#include "valuestack.h"
#include "typestack.h"

/*

Symbol table is a stack of linked lists.
The top node of the stack is the current scope.

Enter scope.
When entering a scope, we push new stack node containing a linkedlist onto the stack.

Leave scope.
When leaving a scope, we pop the stack.


Referencing a variable.
Starting from top of stack, we search each linkedlist for the id

Assigning a variable.
Starting from top of stack, we search each linkedlist for a id and update it if we find it.
If we can't find the id, then go back to top of stack and declare variable on that (current scope basically)

*/


void runSymbolTableTests() {
    // using storage0 = StackEmptyNode;
    //using storage1 = StackNode<LinkedListEmptyNode>;
    //using storageN = StackNode<3, StackNode<4, StackNode<9>>>;

}