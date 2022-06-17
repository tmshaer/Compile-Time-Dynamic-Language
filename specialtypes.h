#include <iostream>

/*
Compatible string literal compile time type (used for ids)

https://stackoverflow.com/questions/62853609/understanding-user-defined-string-literals-addition-for-c20

*/


#include <array>
#include <algorithm>



template<size_t N>
struct string_literal
{
    std::array<char, N> arr_;

    constexpr string_literal(const char(&in)[N]) : arr_{}   
    {
        std::copy(in, in + N, arr_.begin());
    }

    constexpr string_literal(std::array<char, N> a) : arr_{a}   
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const string_literal& dt)
    {
        os << std::string(dt.arr_.data());
        return os;
    }



};

template<template <size_t> typename String1, size_t StringSize1,
        template <size_t> typename String2, size_t StringSize2>
constexpr bool operator==(const String1<StringSize1>& a, const String2<StringSize2>& b) {
        return a.arr_ == b.arr_;
    }

template<template <size_t> typename String1, size_t StringSize1,
        template <size_t> typename String2, size_t StringSize2>
constexpr string_literal<(StringSize1 + StringSize2)-1>  operator+(const String1<StringSize1>& a, const String2<StringSize2>& b) {

        std::array<char, StringSize1 + StringSize2-1> temp;

        std::copy_n(a.arr_.begin(), StringSize1-1 , std::begin(temp));
        std::copy_n(b.arr_.begin(), StringSize2 ,std::begin(temp) + StringSize1  - 1);


        string_literal<(StringSize1 + StringSize2)-1> temp2 = temp;

        return temp2;
    }