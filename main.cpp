#include <iostream>
#include <vector>
#include <string>
#include "ctpg.hpp"
#include <iostream>
#include <charconv>


using namespace ctpg;
using namespace ctpg::buffers;

constexpr nterm<int> list("list");

constexpr char number_pattern[] = "[1-9][0-9]*";
constexpr regex_term<number_pattern> number("number");


constexpr int to_int(std::string_view sv)
{
    int sum = 0;
    for (auto c : sv) { sum *= 10; sum += c - '0'; }
    return sum;
}


constexpr parser p(
    list,
    terms(',', number),
    nterms(list),
    rules(
        list(number) >=
            to_int,
        list(list, ',', number)
            >= [](int sum, char, const auto& n){ return sum + to_int(n); }
    )
);

class Node {
public:

    const int a = 2;
private:

};

class Number : public Node {
public:

    constexpr Number(const int _value) : value (_value) {}

    const int value;

private:

};


class BinOp {
public:

private:

};


// https://en.cppreference.com/w/cpp/meta

// https://medium.com/@samathy_barratt/constantly-confusing-c-const-and-constexpr-pointer-behaviour-7eddb041e15a





int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        constexpr char example_text[] = "1, 20, 3";

        constexpr auto cres = p.parse(cstring_buffer(example_text)); // notice cstring_buffer and no std::err output
        std::cout << cres.value() << std::endl;
        return 0;
    }

    auto res = p.parse(string_buffer(argv[1]), std::cerr);
    bool success = res.has_value();
    if (success)
        std::cout << res.value() << std::endl;
    return success ? 0 : -1;
}


// int main() {

//     constexpr const Number n(5);
//     static_assert(n.value == 5);

//     constexpr int a = 3 + 2;
    
//     const static Node node;

//     static const constexpr Node* nodepointer = &node;
    
//     //const Node* b = new Node();
//     static_assert(a == 5);
//     std::cout << "hello" << std::endl;
//     //constexpr const int extract = nodepointer->a;
//     //static_assert(extract == 2);
//     //static_assert(nodepointer->a == 2);
//     //static_assert(nodepointer != nullptr);

//     //static_assert(a != 5);

// }


