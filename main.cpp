#include <iostream>
#include <vector>
#include <string>
#include "ctpg.hpp"
#include <iostream>
#include "nodes.cpp"
#include <charconv>


using namespace ctpg;
using namespace ctpg::buffers;

//Nonterminal symbols (short: nonterms) are essentially all non atomic symbols in the grammar. In C++ language these are things like: expression, class definition, function declaration etc.
// int is value type
constexpr nterm<int> list("list");


constexpr nterm<BinOp> binop("binop");


//Terminal symbols (short: terms) are symbols used in grammar definition that are atomic blocks. Examples of the terms from a C++ language are: identifier, '+' operator, various keywords etc.
constexpr char number_pattern[] = "[1-9][0-9]*";
constexpr regex_term<number_pattern> number("number");


constexpr int to_int(std::string_view sv)
{
    int sum = 0;
    for (auto c : sv) { sum *= 10; sum += c - '0'; }
    return sum;
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
constexpr parser p(
    binop,
    terms('+', number),
    nterms(list, binop),
    rules(
        // list(number) >=
        //     to_int,
        // list(list, ',', number)
        //     >= [](int sum, char, const auto& n){ return 5.23; }
         binop(number, '+', number)
             >= [](const auto& n, char, const auto& n2){ return BinOp(to_int(n), to_int(n2)); }
    )
);


// https://en.cppreference.com/w/cpp/meta

// https://medium.com/@samathy_barratt/constantly-confusing-c-const-and-constexpr-pointer-behaviour-7eddb041e15a

// https://github.com/peter-winter/ctpg



int main(int argc, char* argv[])
{

    constexpr char example_text[] = "1 + 40";

    constexpr auto cres = p.parse(cstring_buffer(example_text)); // notice cstring_buffer and no std::err output
    // constexpr int result = cres.value();
    // static_assert(result == 5);

    bool success = cres.has_value();
    if (success)
        std::cout << cres.value().evaluate() << std::endl;
    
    return 0;

}

