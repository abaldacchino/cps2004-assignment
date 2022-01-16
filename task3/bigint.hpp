#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <array>
#include <assert.h>
#include <string.h>

#define UINT64_MAX 18446744073709551615
typedef uint64_t word_t;
//maximum 11 words hence no of words can be stored in 8 bits
typedef uint_least8_t word_count_t;  

template <int bits>
class myuint{
    // asserts that bits has to have a value in the form of 2^n
    static_assert(((bits & (bits-1)) ==0)&& bits!=0, "Bits must be in the form 2^n");
    // specification asks for integers up to 2048 bits
    static_assert(bits>=1 && bits<=2048, "Range of bit number is from 1 to 2048");

    private:
        const word_count_t max_words = bits>>6;
        // stores individual words (of size 64 bits) of the integer
        // with least significant word first
        std::array<word_t, (bits>>6)> words{};
        
    public:
        
        //Default constructor - creates structure with value 0
        myuint(){
        }

        //Unsigned Integer constructor
        myuint(uint num){
            //num should fit in 64 bits since it is of type uint
            words[0] = num;
        }

        /* String constructor - accepts a string of integers, separated by 
        a space. Takes each integer in the string to be a single digit in
        myuint, in little endian format (least significant digit first)
        -- Ignores any excess digits 
        */
        myuint(std::string s){    
            char *token = strtok(s, " ");
            for(word_count_t i=0; i< max_words; i++){
                if(token != NULL){
                    //stoull throws invalid_argument exception in case of token not being an unsigned integer
                    words[i] = std::stoull(token);
                    token = strtok(NULL, " ");
                }
            }
        }

        // Copy assignment
        myuint<bits>& operator=(const myuint<bits>& other){
            // Guard self assignment
            if (this == &other){
                return *this;
            }
            //Copy array (std::array has already defined copy operator)
            other.words = words;
            return *this;
        }

        // Move assignment
        myuint<bits>& operator=(myuint<bits>&& other) noexcept{
            // Guard self assignment
            if (this == &other){
                return *this;
            }

            words = std::exchange(other.words, nullptr); // leave other in valid state
            return *this;
        }


        // prefix increment
        myuint<bits>& operator++(){
            words[0]++; //Adds 1 to least significant digit
            //Accounts for carry (case of overflow)
            for(word_count_t i=1; i<max_words; i++){
                if(words[i-1]==0){
                    words[i]++;
                }else{
                    break;
                }
            }
            return *this; // return new value by reference
        }
        
        // postfix increment
        myuint<bits> operator++(int){
            myuint<bits> old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }    
        
        // prefix decrement
        myuint<bits>& operator--(){
            words[0]--; //Removes 1 to least significant digit
            //Accounts for carry (case of underflow)
            for(word_count_t i=1; i<max_words; i++){
                if(words[i-1]==UINT64_MAX){
                    words[i]--;
                }else{
                    break;
                }
            }
            return *this; // return new value by reference
        }
        
        // postfix decrement
        myuint<bits> operator--(int){
            myuint<bits> old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }

        // (compound assignment) addition of rhs to lhs
        myuint<bits>& operator+=(const myuint<bits>& rhs){
            words[0] += rhs.words[0];
            //carry ==1 if words[0] overflows (wraps around), 0 otherwise
            //this way conditionals are avoided
            int carry = words[0] < rhs.words[0];    
            for(word_count_t i=1; i<max_words; i++){
                words[i] += (rhs.words[i] + carry);
                carry = words[i] < (rhs.words[i] + carry);
            }
            return *this;
        }

        // addition operator reuses compound assignment
        myuint<bits> operator+(myuint<bits> lhs, const myuint<bits>& rhs){
            lhs+= rhs;
            return lhs;
        }

        // (compound assignment) subtraction of rhs from lhs
        myuint<bits>& operator-=(const myuint<bits>& rhs){
            words[0] -= rhs.words[0];
            //carry ==1 if words[0] underflows (wraps around), 0 otherwise
            //this way conditionals are avoided
            int carry = words[0] > rhs.words[0];    
            for(word_count_t i=1; i<max_words; i++){
                words[i] -= (rhs.words[i] + carry);
                carry = words[i] > (rhs.words[i] + carry);
            }
            return *this;
        }

        // subtraction operator reuses compound assignment
        myuint<bits> operator-(myuint<bits> lhs, const myuint<bits>& rhs){
            lhs-= rhs;
            return lhs;
        }


        //Write to stream 
        template<int bits>
        std::ostream& operator<<(std::ostream& os, const myuint<bits>& bigint){
            for(int i=0; i<bigint.max_words; i++){
                os << word << "×2^" << (i>>6);
                if(i!= max_words-1){
                    os << " + ";
                }
            }
            return os;
        }

};
