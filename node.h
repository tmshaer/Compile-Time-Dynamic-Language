#include "linkedlist.h"
#include "stack.h"




/*
Contains all the valid AST nodes for our language.
*/

template<auto N>
struct Val {

    template<typename SymbolTable>
     static const constexpr auto getValue() {
        return N;
    }
};

template<string_literal N>
struct ValStr {

    template<typename SymbolTable>
     static const constexpr auto getValue() {
        return N;
    }
};


template<int N>
struct ValNum {

    template<typename SymbolTable>
     static const constexpr auto getValue() {
        return N;
    }
};


template<string_literal VarName>
struct Var {

    template<typename SymbolTable>
     static const constexpr auto getValue() {
        return LinkedListGetValue<SymbolTable, VarName>::value;
    }
};


template<typename Func, typename...Args>
struct Apply {

    template<typename SymbolTable>
    static const constexpr auto getValue() {
        return Func{}((Args::template getValue<SymbolTable>())...);
    }
};

template<bool Condition, typename...Statements>
struct If {

    template<typename SymbolTable>
    static const constexpr auto getValue() {
        return 3;
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


// match assign node
template <typename SymbolTable, template <string_literal, typename> typename AssignNode, string_literal VarName, typename Value, typename... Statements>
struct Execute<SymbolTable, AssignNode<VarName, Value>, Statements...> {

    using TempTable = Assign<VarName, Value, SymbolTable>::UpdatedSymbolTable;
    using values = Execute<TempTable, Statements...>::values;
};

// match assign node
// template <typename SymbolTable, template <string_literal, typename> typename AssignNode, string_literal VarName, typename Value, typename... Statements>
// struct Execute<SymbolTable, AssignNode<VarName, Value>, Statements...> {

//     using TempTable = Assign<VarName, Value, SymbolTable>::UpdatedSymbolTable;
//     using values = Execute<TempTable, Statements...>::values;
// };


