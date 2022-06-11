#include "lookuptable.h"




/*
Contains all the valid AST nodes for our language.
*/


template<int N>
struct Num {
     static const constexpr int value = N;
};



template<typename A, typename B>
struct BinOpPlus {
    static const constexpr int value = A::value + B::value;
};




template<string_literal VarName, typename Value, typename SymbolTable = LinkedListEmptyNode>
struct Assign {
    using UpdatedSymbolTable = LinkedListAddValue<SymbolTable, VarName, Value::value>::newList;
};



// variadic recursion ... pass symbol table as variable to each one


template <typename SymbolTable, typename... Statements>
struct Execute {
    using values = SymbolTable;
};


template <typename SymbolTable, template <string_literal, typename> typename T, string_literal VarName, typename Value, typename... Statements>
struct Execute<SymbolTable, T<VarName, Value>, Statements...> {

    using TempTable = Assign<VarName, Value, SymbolTable>::UpdatedSymbolTable;
    using values = Execute<TempTable, Statements...>::values;
};



