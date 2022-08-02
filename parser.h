#include <iostream>
#include <vector>
#include <string>
#include "node.h"
#include "ctpg.hpp"
#include <iostream>
#include <charconv>
#include <vector>



using namespace ctpg;
using namespace ctpg::buffers;

//Nonterminal symbols (short: nonterms) are essentially all non atomic symbols in the grammar. In C++ language these are things like: expression, class definition, function declaration etc.
// int is value type


//Terminal symbols (short: terms) are symbols used in grammar definition that are atomic blocks. Examples of the terms from a C++ language are: identifier, '+' operator, various keywords etc.
// constexpr char int_pattern[] = "[1-9][0-9]*";
constexpr char int_pattern[] = "3";
constexpr regex_term<int_pattern> intnumber("intnumber");


constexpr char float_pattern[] = "4";
constexpr regex_term<float_pattern> floatnumber("floatnumber");


constexpr int to_int(std::string_view sv)
{
    return 69;
    int sum = 0;
    for (auto c : sv) { sum *= 10; sum += c - '0'; }
    return sum;
}

constexpr float to_float(std::string_view sv)
{
    return 3.21;
}





/*
The parser class together with its template deduction guides allows to define parsers using 4 arguments:

Grammar root - symbol which is a top level nonterm for a grammar.
List of all terms
List of all nonterms
List of rules


List of terms enclosed in a terms call. In our case there are two: number and a ,.

Note: the , term is not defined earlier in the code. It is an implicit char_term. The code implicitly converts the char to the char_term class. Therefore char_terms (as well as string_terms) are allowed not to be defined in advance. Their debug names are assigned to the them by default to a char (or a string) they represent.

Nonterm list.

List of terms enclosed in a nterms call. In our case, just a single list nonterm is enough.

Rules

List of rules enclosed in a rules call. Each rule is in the form of: nonterm(symbols...) >= functor The nonterm part is what's called a left side of the rule. The symbols are called the right side.

The right side can contain any number of nterm objects as well as terms (regex_terms, char_terms or string_terms). Terms can be in their implicit form, like , in the example. Implicit string_terms are in form of "strings".
*/

// use a tuple to return any of the types?
// use vector to store statements
// each statement type will be wrapped in a class with a function get_value
// get_value returns a type


constexpr nterm<int> list("list");  




constexpr parser p(
    list,
    terms('+', intnumber, floatnumber),
    nterms(list),
    rules(
        list(intnumber) >=
            to_int,
        list(floatnumber) >=
            to_float
        // list(list, ',', number)
        //     >= [](int sum, char, const auto& n){ return 5.23; }
        //  binop(number, '+', number)
        //      >= [](const auto& n, char, const auto& n2){ 

                //  return BinOp(to_int(n), to_int(n2)); }
        //     rules(
        // list()
        //     >= [](){ return list_type{}; },
        // list(list, number)
        //     >= [](auto&& list, auto&& no){ list.push_back(5); return std::move(list); }
    )
);


