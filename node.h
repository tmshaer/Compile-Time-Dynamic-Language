#include "symboltable.h"

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
        return SymbolTableGetValue<NONE, SymbolTable, VarName>::value;
    }
};


template<typename Func, typename...Args>
struct Apply {

    template<typename SymbolTable>
    static const constexpr auto getValue() {
        return Func{}((Args::template getValue<SymbolTable>())...);
    }
};





// variadic recursion ... pass symbol table as variable to each one


template <typename SymbolTable, typename... Statements>
struct Execute {
    using values = SymbolTable;
};

template<typename Condition, typename SymbolTable = TypeStackEmptyNode, typename...Statements>
struct If {
    

    using UpdatedSymbolTable = typename decltype([]() {
        
        static const constexpr auto conditionValue = Condition::template getValue<SymbolTable>();

        //                                                  DON'T FORGET CONST!
         if constexpr (conditionValue) {
            using newSymbolTable = TypeStackNode<LinkedListEmptyNode, SymbolTable>; // declare new empty scope on temp symbol table
            using updatedSymbolTable = Execute<newSymbolTable, Statements...>::values; // execute if statement code on temp symbol table
            using poppedSymbolTable = TypeStackPopNode<updatedSymbolTable>::newList; // pop off the new latest stack
            return typetag<poppedSymbolTable>{};
        } else {
            // if not true just ignore and return old symbol table
            return typetag<SymbolTable>{};
         }
    }())::type;

};




template<string_literal VarName, typename Value, typename SymbolTable = TypeStackEmptyNode>
struct Assign {
    using UpdatedSymbolTable = SymbolTableDeclareOrAssignVariable<SymbolTable, VarName, Value::template getValue<SymbolTable>()>::newStack;
};


// match assign node
template <typename SymbolTable, 
          template <string_literal, typename> typename AssignNode, string_literal VarName, typename Value,
          typename... Statements>
struct Execute<SymbolTable, AssignNode<VarName, Value>, Statements...> {

    using TempTable = Assign<VarName, Value, SymbolTable>::UpdatedSymbolTable;
    using values = Execute<TempTable, Statements...>::values;
};


// match if node
template <typename SymbolTable, 
          template <typename, typename> typename IfNode, typename Condition, typename... IfStatements,
          typename... Statements>
struct Execute<SymbolTable, IfNode<Condition, IfStatements...>, Statements...> {

    using TempTable = IfNode<Condition, SymbolTable, IfStatements...>::UpdatedSymbolTable;
    using values = Execute<TempTable, Statements...>::values;
};
