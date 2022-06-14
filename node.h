#include "lookuptable.h"




/*
Contains all the valid AST nodes for our language.
*/


template<int N>
struct Num {

    template<typename SymbolTable>
     static const constexpr int getValue() {
        return N;
    }
};


template<string_literal VarName>
struct Var {

    template<typename SymbolTable>
     static const constexpr int getValue() {
        return LinkedListGetValue<SymbolTable, VarName>::value;
    }
};


template<typename A, typename B>
struct BinOpPlus {

    template<typename SymbolTable>
    static const constexpr int getValue() {
        return A::template getValue<SymbolTable>() + B::template getValue<SymbolTable>();
    }
};




template<string_literal VarName, typename Value, typename SymbolTable = LinkedListEmptyNode>
struct Assign {
    using UpdatedSymbolTable = LinkedListSetOrAddValue<SymbolTable, VarName, Value::template getValue<SymbolTable>()>::newList;
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



