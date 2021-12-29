#pragma once
#include <iostream>
#include <cmath>
#include <array>
#include <assert.h>

template <int bits>
class myuint{
    // asserts that bits has to have a value in the form of 2^n
    static_assert(((bits & (bits-1)) ==0)&& bits!=0, "Bits must be in the form 2^n");
    // structure accepts
    static_assert(bits>=1 && bits<=2048, "Range of bit number is from 1 to 2048");

    private:
        // stores individual words (of size 64 bits) of the integer
        // with least significant word first
        std::array<uint64_t, bits/64> words{};

    public:
        myuint();
        myuint(uint);
        myuint(std::string);
        std::string to_string();
};

//Default constructor - creates structure with value 0
template<int bits>
myuint<bits>::myuint(){
}

//Unsigned Integer constructor
template<int bits>
myuint<bits>::myuint(uint num){
    //num should fit in 64 bits since it is of type uint
    words[0] = num;
}

//String constructor
template<int bits>
myuint<bits>::myuint(std::string s){
    // Insert code here to convert from string to integer
    std::cout << "This is my string constructor" << s << std::endl;
}

//Displays 
template<int bits>
std::string to_string(){
    std::string out = "";
    for(auto word:myuint<bits>::words){
        out += std::to_string(word);
    }
    return out;
}