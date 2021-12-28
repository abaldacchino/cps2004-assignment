#pragma once
#include <iostream>
#include <cmath>
#include <array>
#include <assert.h>

template <int bits>
class Storage{
    // asserts that bits has to have a value in the form of 2^n
    static_assert((bits & (bits-1)) ==0, "Bits must be in the form 2^n");

    private:
        // stores individual words (of size 64 bits) of the integer
        // with least significant word first
        std::array<uint64_t, bits/64> words;

    public:
        Storage(std::string);
        Storage(uint);
        void print();
};

template<int bits>
Storage<bits>::Storage(std::string s){
    // Insert code here to convert from string to integer
    std::cout << "This is my constructor" << s << std::endl;
}

template<int bits>
Storage<bits>::Storage(uint num){
    //Since num is of size integer, will definitely fit in 64 bits
    words[0] = num;
    std::cout << words[0] << std::endl;
}

template<int bits>
void print(){
    std::cout << "a" << std::endl;
}