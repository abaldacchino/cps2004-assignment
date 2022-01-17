#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <array>
#include <assert.h>
#include <string.h>
#include <stdexcept>

//UINT64_MAX not guaranteed to be defined
#ifndef UINT64_MAX 
#define UINT64_MAX 18446744073709551615
#endif

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

        //Helper functions used in shifts and multiplication
        //get highest n bits in uint64_t
        static uint64_t get_hi(uint64_t x, uint n){
            return x >> (64-n);
        }
        //get lowest n bits in uint64_t
        static uint64_t get_lo(uint64_t x, uint n){
            return (x << (64-n))>>(64-n);
        }
        
    public:
        
        //Default constructor - creates structure with value 0
        myuint(){
        }

        //Unsigned Integer constructor
        myuint(uint num){
            //num should fit in 64 bits since it is of type uint
            words[0] = num;
        }

        /* String constructor (for base 10) 
           Accepts a string representation of an integer
           Behaviour when number too large is undefined
        */
        myuint(std::string s){    
            myuint<bits> ten(10);           //stores value 10 in myuint datatype
            myuint<bits> ten_power_i(1);
            for(int i=0; i<s.size(); i++){
                myuint<bits> buffer(s[i] - '0');   //gets ith digit of string
                this += buffer * ten_power_i;
                ten_power_i *=ten;
            }
        }
        

        /* String constructor (for base 64) 
        Accepts a string of integers, separated by a space. 
        Takes each integer in the string to be a single digit (base 64) of
        myuint, in little endian format (least significant digit first)
        -- Ignores any excess digits 
        
        myuint(std::string s){    
            char *token = strtok(&(s[0]), " ");
            for(word_count_t i=0; i< max_words; i++){
                if(token != NULL){
                    //stoull throws invalid_argument exception in case of token not being an unsigned integer
                    words[i] = std::stoull(token);
                    token = strtok(NULL, " ");
                }
            }
        }
        */


        // Constructor that accepts array of (base 64) digits
        myuint(std::array<word_t, (bits>>6)> input){    
            words=input;
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

        // (compound assignment) addition of rhs to this
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
        friend myuint<bits> operator+(myuint<bits> lhs, const myuint<bits>& rhs){
            lhs+= rhs;
            return lhs;
        }

        // (compound assignment) subtraction of rhs from this
        myuint<bits>& operator-=(const myuint<bits>& rhs){
            words[0] -= rhs.words[0];
            //carry ==1 if words[0] underflows (wraps around), 0 otherwise
            //this way conditionals are avoided
            int carry = words[0] > rhs.words[0];    
            for(word_count_t i=1; i<max_words; i++){
                words[i] -= (rhs.words[i] + carry);
                carry = words[i] > (words[i] + rhs.words[i] + carry);
            }
            return *this;
        }

        // subtraction operator reuses compound assignment
        friend myuint<bits> operator-(myuint<bits> lhs, const myuint<bits>& rhs){
            lhs-= rhs;
            return lhs;
        }

        // (compound assignment) right shift of this by rhs
        myuint<bits>& operator>>=(const uint& rhs){
            // rhs = 64*word_shift + rem
            uint word_shift = (rhs>>6);  //dividing rhs by word length (64)
            uint rem = rhs - (word_shift<<6);    //Finding remainder

            if(rem==0){
                int i=0;
                while(i<max_words-word_shift){
                    words[i] = words[i+word_shift];
                    i++;
                }
                while(i<max_words){
                    words[i]=0;
                    i++;
                }
            }else{
                int i=0;
                //Case where words[i] is entirely filled
                while(i<max_words-(word_shift+1)){
                    /* Performing shift by getting highest bits from words[i+word_shift]
                       and lowest bits from words[i+word_shift+1]
                    */
                    words[i] =  (get_lo(words[i+word_shift+1], rem)<<(64-rem))
                        | (get_hi(words[i+word_shift], 64-rem));
                    i++;
                }
                //Case where words[i] only partially filled
                //Only lowest bits of words[i] filled
                words[i] =  get_hi(words[i+word_shift], 64-rem);

                //Case where words[i] not filled (0)
                while(i<max_words){
                    words[i]=0;
                    i++;
                }
            }
            return *this;
        }

        // right shift operator reuses compound assignment
        friend myuint<bits> operator>>(myuint<bits> lhs, const uint& rhs){
            lhs>>= rhs;
            return lhs;
        }

        // (compound assignment) left shift of this by rhs
        myuint<bits>& operator<<=(const uint& rhs){
            // rhs = 64*word_shift + rem
            uint word_shift = (rhs>>6);  //dividing rhs by word length (64)
            uint rem = rhs - (word_shift<<6);    //Finding remainder

            if(rem==0){
                int i=max_words-1;
                while(i>=word_shift){
                    words[i] = words[i-word_shift];
                    i--;
                }
                while(i>=0){
                    words[i]=0;
                    i--;
                }
            }else{
                //Case where words[i] entirely filled
                int i=max_words-1;
                while(i>=word_shift+1){
                    /* Performing shift by getting lowest bits from words[i-word_shift]
                       and highest bits from words[i-(word_shift)-1]
                    */
                    words[i] =  (get_lo(words[i-word_shift], 64-rem)<<(64-rem))
                        | (get_hi(words[i-word_shift-1], rem));
                    i--;
                }
                //Case where words[i] is only partially filled
                //Only highest bits of words[i] filled
                words[i] =  (get_lo(words[i-word_shift], 64-rem)<<(64-rem));

                //Case where words[i] is not filled (0)
                while(i>=0){
                    words[i]=0;
                    i--;
                }
            }
            return *this;
        }

        // left shift operator reuses compound assignment
        friend myuint<bits> operator<<(myuint<bits> lhs, const uint& rhs){
            lhs<<= rhs;
            return lhs;
        }

        // (compound assignment) multiplication of rhs to lhs
        // Long multiplication used
        myuint<bits>& operator*=(const myuint<bits>& rhs){
            myuint<bits> result;
            uint64_t carry =0;

            /* Method has to further split each word into 32 bit sized integers in
               order to calculate overflow. In a multiplication heavy application
               of bigint, ideally words should be stored in 32 bits immediately
            */
            for(word_count_t i=0; i<max_words; i++){
                for(word_count_t j=0; j<max_words; j++){
                    //Resulting multiplication is multiplied by (64)^index
                    uint index= i+j;

                    //Any overflow is ignored (making calculation mod bits)
                    if(index>=max_words)continue;

                    // Calculating coefficients a1b1(k)^2 + (a1b0+a0b1)k + a0b0
                    // where a1 = highest 32 bits of a, a0 lowest 32 bits of a
                    uint64_t s0 = get_lo(words[i], 32)* get_lo(rhs.words[j], 32);
                    uint64_t s1 = get_lo(words[i], 32)* get_hi(rhs.words[j], 32);
                    uint64_t s2 = get_hi(words[i], 32)* get_lo(rhs.words[j], 32);

                    s1 += s2;

                    myuint<bits> temp;
                    temp.words[index] = s0 + (get_lo(s1, 32) <<32);
                    if(index+1<max_words){
                        uint64_t s3 = get_hi(words[i], 32)* get_hi(rhs.words[j], 32);
                        //Case of overflow of s1+s2
                        s3 += s1<s2;

                        temp.words[index+1] = s1 + get_hi(s2, 32); 
                    }

                    result+=temp;
                }
            }
            *this = result;
            return *this;
        }

        // multiplication operator reuses compound assignment
        friend myuint<bits> operator*(myuint<bits> lhs, const myuint<bits>& rhs){
            lhs *=rhs;
            return lhs;
        }
        

        // (compound assignment) division of lhs by rhs
        // Long division used
        myuint<bits>& operator/=(myuint<bits> rhs){
            myuint<bits> count;
            if(rhs==0) throw std::overflow_error("Divide by zero exception");

            while(this >= rhs){
                count++;
                this -= rhs;
            }
            this =count;
            return *this;
        }

        // multiplication operator reuses compound assignment
        friend myuint<bits> operator/(myuint<bits> lhs, myuint<bits> rhs){
            lhs /=rhs;
            return lhs;
        }

        // modulus function, gives answer to lhs mod rhs
        // Long division used
        myuint<bits>& operator%=(myuint<bits> rhs){
            myuint<bits> count;
            if(rhs==0) throw std::overflow_error("Divide by zero exception");

            while(this >= rhs){
                count++;
                this -= rhs;
            }

            return *this;
        }

        // modulus operator reuses compound assignment
        friend myuint<bits> operator%(myuint<bits> lhs, myuint<bits> rhs){
            lhs %=rhs;
            return lhs;
        }

        //Comparison operators
        inline friend bool operator< (const myuint<bits>& lhs, const myuint<bits>& rhs){ 
            for(word_count_t i= lhs.max_words-1; i>=0; i--){
                if(lhs.words[i] < rhs.words[i])return true;
                if(rhs.words[i] < lhs.words[i])return false;
            }
            return false;    //case they are equal
        }
        inline friend bool operator> (const myuint<bits>& lhs, const myuint<bits>& rhs){ 
            return rhs < lhs; 
        }

        inline friend bool operator<=(const myuint<bits>& lhs, const myuint<bits>& rhs){ 
            return !(lhs > rhs); 
        }
        inline friend bool operator>=(const myuint<bits>& lhs, const myuint<bits>& rhs){ 
            return !(lhs < rhs); 
        }
        inline friend bool operator==(const myuint<bits>& lhs, const myuint<bits>& rhs){ 
            return !(lhs < rhs) && !(rhs < lhs); 
        }
        inline friend bool operator!=(const myuint<bits>& lhs, const myuint<bits>& rhs){ 
            return !(lhs==rhs); 
        }

        //Write to stream (convert to base 10)
        friend std::ostream& operator<<(std::ostream& os, const myuint<bits>& bigint){
            //Outputs representation in base 10
            myuint<bits> ten(10);           //stores value 10 in myuint datatype
            myuint<bits> digit;             //stores nth digit
            myuint<bits> result;

            do{
                digit = result % ten;   //result mod 10 gets the last digit of result
                os << digit.words[0];   //outputs digit (stored in words[0] since value<10)
                result /= ten;  //divides result by 10 (effectively right shifting in base 10)
            }while(!result.is_zero());

            return os;
            /* Outputs representation in base 64
            for(int i=0; i<bigint.max_words; i++){
                os << bigint.words[i] << "×2^" << (i>>6);
                if(i!= bigint.max_words-1){
                    os << " + ";
                }
            }
            return os;
            */
        }

        //Quick function that returns if value is equal to 0
        bool is_zero(){
            for(auto word : words){
                if(word !=0)return false;
            }
            return true;
        }
};

