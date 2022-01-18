#pragma once
#include <iostream>
#include <strin
#include <cmath>
#include <array>
#include <assert.h>
#include <string.h>
#include <stdexcept>
#include <algorithm>

//UINT64_MAX not guaranteed to be defined
#ifndef UINT64_MAX 
#define UINT64_MAX 18446744073709551615
#endif

typedef uint64_t word_t;

template <int bits>
class myuint{
    // asserts that bits has to have a value in the form of 2^n
    static_assert(((bits & (bits-1)) ==0)&& bits!=0, "Bits must be in the form 2^n");
    // specification asks for integers up to 2048 bits
    static_assert(bits>=1 && bits<=2048, "Range of bit number is from 1 to 2048");

    private:
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
        myuint(uint64_t num){
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
            for(int i=s.size()-1; i>=0; i--){
                myuint<bits> buffer(s[i] - '0');   //gets ith digit of string
                *this += buffer * ten_power_i;
                ten_power_i *=ten;
            }
        }

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
            words = other.words;
            return *this;
        }


        // prefix increment
        myuint<bits>& operator++(){
            words[0]++; //Adds 1 to least significant digit
            //Accounts for carry (case of overflow)
            for(int i=1; i<words.size(); i++){
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
            for(int i=1; i<words.size(); i++){
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
            for(int i=1; i<words.size(); i++){
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
            int carry = words[0] > words[0] + rhs.words[0];    
            for(int i=1; i<words.size(); i++){
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
            if(rhs==0)return *this;

            // rhs = 64*word_shift + rem
            uint word_shift = (rhs>>6);  //dividing rhs by word length (64)
            uint rem = rhs - (word_shift<<6);    //Finding remainder

            if(rem==0){
                int i=0;
                while(i<words.size()-word_shift){
                    words[i] = words[i+word_shift];
                    i++;
                }
                while(i<words.size()){
                    words[i]=0;
                    i++;
                }
            }else{
                int i=0;
                //Case where words[i] is entirely filled
                while(i<words.size()-(word_shift+1)){
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
                i++;

                //Case where words[i] not filled (0)
                while(i<words.size()){
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
            if(rhs==0)return *this;

            // rhs = 64*word_shift + rem
            uint word_shift = (rhs>>6);  //dividing rhs by word length (64)
            uint rem = rhs - (word_shift<<6);    //Finding remainder

            if(rem==0){
                int i=words.size()-1;
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
                int i=words.size()-1;
                while(i>=word_shift+1){
                    /* Performing shift by getting lowest bits from words[i-word_shift]
                       and highest bits from words[i-(word_shift)-1]
                    */
                    words[i] =  (get_lo(words[i-word_shift], 64-rem)<<rem)
                        | (get_hi(words[i-word_shift-1], rem));
                    i--;
                }
                //Case where words[i] is only partially filled
                //Only highest bits of words[i] filled
                words[i] =  (get_lo(words[i-word_shift], 64-rem)<<rem);
                i--;

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
            for(int i=0; i<words.size(); i++){
                for(int j=0; j<words.size(); j++){
                    //Resulting multiplication is multiplied by (64)^index
                    int index= i+j;

                    //Any overflow is ignored (making calculation mod bits)
                    if(index>=words.size())continue;

                    // Calculating coefficients a1b1(k)^2 + (a1b0+a0b1)k + a0b0
                    // where a1 = highest 32 bits of a, a0 lowest 32 bits of a
                    uint64_t s0 = get_lo(words[i], 32)* get_lo(rhs.words[j], 32);
                    uint64_t s1 = get_lo(words[i], 32)* get_hi(rhs.words[j], 32);
                    uint64_t s2 = get_hi(words[i], 32)* get_lo(rhs.words[j], 32);

                    s1 += s2;

                    myuint<bits> temp(0);
                    temp.words[index] = s0 + (get_lo(s1, 32) <<32);
                    if((index+1)<words.size()){
                        uint64_t s3 = get_hi(words[i], 32)* get_hi(rhs.words[j], 32);
                        //Case of overflow of s1+s2 (need to increment s3 by 2^32)
                        s3 += ((uint64_t)(s1<s2))<<32;
                        temp.words[index+1] = s3 + get_hi(s1, 32); 
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

        // Function returns result of division and stores remainder in lhs
        // Useful for division/remainder operators 
        // Especially useful when user needs both division and remainder
        myuint<bits> div_rem(myuint<bits> &lhs, myuint<bits> rhs){
            if(rhs==0) throw std::overflow_error("Divide by zero exception");

            myuint<bits> count;
            myuint<bits> one(1);

            int power = (lhs.get_msd() - rhs.get_msd())*64+63;

            //Need to safeguard for overflow here is rhs is very large
            //Note division might be very inefficient in specific this case
            if(rhs.get_msd() == words.size()-1)power==0;   
            
            while(lhs >= rhs){
                myuint<bits> rhs_power = rhs << (power);  //rhs left shifted by power
                myuint<bits> add_count = one << (power);
                while(lhs >= rhs_power){
                    lhs -= rhs_power;
                    count+= add_count;  //increment count by add_count (1^power)
                    
                }
                power--;
            }

            return count;
        }
        

        // (compound assignment) division of lhs by rhs
        // Long division used
        myuint<bits>& operator/=(myuint<bits> rhs){
            *this = div_rem(*this, rhs);
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
            div_rem(*this, rhs);
            return *this;
        }

        // modulus operator reuses compound assignment
        friend myuint<bits> operator%(myuint<bits> lhs, myuint<bits> rhs){
            lhs %=rhs;
            return lhs;
        }

        //Comparison operators
        inline friend bool operator< (const myuint<bits>& lhs, const myuint<bits>& rhs){ 
            for(int i= lhs.words.size()-1; i>=0; i--){
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
            os << bigint.to_string();
            return os;
        }

        //Quick function that returns if value is equal to 0
        bool is_zero(){
            for(auto word : words){
                if(word !=0)return false;
            }
            return true;
        }

        //Returns most significant digit (ie largest i where words[i] !=0)
        //Returns -1 if all digits 0
        int get_msd(){
            for(int i= words.size()-1; i>=0; i--){
                if(words[i] !=0)return i;
            }
            return -1;
        }

        //Gets base 10 representation of uint
        std::string to_string() const {
            //Outputs representation in base 10
            myuint<bits> ten(10);           //stores value 10 in myuint datatype
            myuint<bits> digit;             //stores nth digit
            myuint<bits> result = *this;
            std::string base_10 = "";   //empty string to hold base 10 representation

            do{
                digit = result % ten;   //result mod 10 gets the last digit of result
                base_10 = std::to_string(digit.words[0]) + base_10;   //adds digit to string
                result /= ten;  //divides result by 10 (effectively right shifting in base 10)
            }while(!result.is_zero());

            return base_10;
        }

};

/*********************************************************************************************************************
 * From now on, default templates are specified 
 * 1/2/4/8 bit integers take 8 bits
 * 8/16/32/64 bit integers take their respective size bits
 *********************************************************************************************************************/


template <>
class myuint<1>{

    private:
        uint8_t num =0;
    public:
        //Default constructor - creates structure with value 0
        myuint(){
        }
        //Unsigned Integer constructor
        myuint(uint8_t num){
            this->num = num;
        }
        /* String constructor (for base 10) 
           Accepts a string representation of an integer
           Behaviour when number too large is undefined
        */
        myuint(std::string s){    
            num = std::stoul(s);
        }
        // Copy assignment
        myuint<1>& operator=(const myuint<1>& other){
            // Guard self assignment
            if (this == &other){
                return *this;
            }
            num = other.num;
            return *this;
        }
        // prefix increment
        myuint<1>& operator++(){
            num++;
            return *this; // return new value by reference
        }
        // postfix increment
        myuint<1> operator++(int){
            myuint<1> old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }   
        // prefix decrement
        myuint<1>& operator--(){
            num--;
            return *this; // return new value by reference
        }
        // postfix decrement
        myuint<1> operator--(int){
            myuint<1> old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }
        // (compound assignment) addition of rhs to this
        myuint<1>& operator+=(const myuint<1>& rhs){
            num += rhs.num;
            return *this;
        }
        // addition operator reuses compound assignment
        friend myuint<1> operator+(myuint<1> lhs, const myuint<1>& rhs){
            lhs+= rhs;
            return lhs;
        }
        // (compound assignment) subtraction of rhs from this
        myuint<1>& operator-=(const myuint<1>& rhs){
            num -= rhs.num;
            return *this;
        }
        // subtraction operator reuses compound assignment
        friend myuint<1> operator-(myuint<1> lhs, const myuint<1>& rhs){
            lhs-= rhs;
            return lhs;
        }
        // (compound assignment) right shift of this by rhs
        myuint<1>& operator>>=(const uint& rhs){
            num >>= rhs;
            return *this;
        }
        // right shift operator reuses compound assignment
        friend myuint<1> operator>>(myuint<1> lhs, const uint& rhs){
            lhs>>= rhs;
            return lhs;
        }
        // (compound assignment) left shift of this by rhs
        myuint<1>& operator<<=(const uint& rhs){
            num <<= rhs;
            return *this;
        }
        // left shift operator reuses compound assignment
        friend myuint<1> operator<<(myuint<1> lhs, const uint& rhs){
            lhs<<= rhs;
            return lhs;
        }
        // Multiplcation of rhs to lhs
        myuint<1>& operator*=(const myuint<1>& rhs){
            num *= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<1> operator*(myuint<1> lhs, const myuint<1>& rhs){
            lhs *=rhs;
            return lhs;
        }
        // (compound assignment) division of lhs by rhs
        myuint<1>& operator/=(myuint<1> rhs){
            num /= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<1> operator/(myuint<1> lhs, myuint<1> rhs){
            lhs /=rhs;
            return lhs;
        }
        // modulus function, gives answer to lhs mod rhs
        myuint<1>& operator%=(myuint<1> rhs){
            num %= rhs.num;
            return *this;
        }
        // modulus operator reuses compound assignment
        friend myuint<1> operator%(myuint<1> lhs, myuint<1> rhs){
            lhs %=rhs;
            return lhs;
        }
        //Comparison operators
        inline friend bool operator< (const myuint<1>& lhs, const myuint<1>& rhs){ 
            return lhs.num < rhs.num;
        }
        inline friend bool operator> (const myuint<1>& lhs, const myuint<1>& rhs){ 
            return rhs < lhs; 
        }
        inline friend bool operator<=(const myuint<1>& lhs, const myuint<1>& rhs){ 
            return !(lhs > rhs); 
        }
        inline friend bool operator>=(const myuint<1>& lhs, const myuint<1>& rhs){ 
            return !(lhs < rhs); 
        }
        inline friend bool operator==(const myuint<1>& lhs, const myuint<1>& rhs){ 
            return lhs.num == rhs.num;
        }
        inline friend bool operator!=(const myuint<1>& lhs, const myuint<1>& rhs){ 
            return !(lhs==rhs); 
        }
        //Write to stream (convert to base 10)
        friend std::ostream& operator<<(std::ostream& os, const myuint<1>& bigint){
            os << bigint.num;
            return os;
        }
        //Quick function that returns if value is equal to 0
        bool is_zero(){
            return num==0;
        }
        //Gets base 10 representation of uint
        std::string to_string() const {
            return std::to_string(num);
        }
};
template <>
class myuint<2>{
    private:
        uint8_t num =0;
    public:
        //Default constructor - creates structure with value 0
        myuint(){
        }
        //Unsigned Integer constructor
        myuint(uint8_t num){
            this->num = num;
        }
        /* String constructor (for base 10) 
           Accepts a string representation of an integer
           Behaviour when number too large is undefined
        */
        myuint(std::string s){    
            num = std::stoul(s);
        }
        // Copy assignment
        myuint<2>& operator=(const myuint<2>& other){
            // Guard self assignment
            if (this == &other){
                return *this;
            }
            num = other.num;
            return *this;
        }
        // prefix increment
        myuint<2>& operator++(){
            num++;
            return *this; // return new value by reference
        }
        // postfix increment
        myuint<2> operator++(int){
            myuint<2> old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }   
        // prefix decrement
        myuint<2>& operator--(){
            num--;
            return *this; // return new value by reference
        }
        // postfix decrement
        myuint<2> operator--(int){
            myuint<2> old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }
        // (compound assignment) addition of rhs to this
        myuint<2>& operator+=(const myuint<2>& rhs){
            num += rhs.num;
            return *this;
        }
        // addition operator reuses compound assignment
        friend myuint<2> operator+(myuint<2> lhs, const myuint<2>& rhs){
            lhs+= rhs;
            return lhs;
        }
        // (compound assignment) subtraction of rhs from this
        myuint<2>& operator-=(const myuint<2>& rhs){
            num -= rhs.num;
            return *this;
        }
        // subtraction operator reuses compound assignment
        friend myuint<2> operator-(myuint<2> lhs, const myuint<2>& rhs){
            lhs-= rhs;
            return lhs;
        }
        // (compound assignment) right shift of this by rhs
        myuint<2>& operator>>=(const uint& rhs){
            num >>= rhs;
            return *this;
        }
        // right shift operator reuses compound assignment
        friend myuint<2> operator>>(myuint<2> lhs, const uint& rhs){
            lhs>>= rhs;
            return lhs;
        }
        // (compound assignment) left shift of this by rhs
        myuint<2>& operator<<=(const uint& rhs){
            num <<= rhs;
            return *this;
        }
        // left shift operator reuses compound assignment
        friend myuint<2> operator<<(myuint<2> lhs, const uint& rhs){
            lhs<<= rhs;
            return lhs;
        }
        // Multiplcation of rhs to lhs
        myuint<2>& operator*=(const myuint<2>& rhs){
            num *= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<2> operator*(myuint<2> lhs, const myuint<2>& rhs){
            lhs *=rhs;
            return lhs;
        }
        // (compound assignment) division of lhs by rhs
        myuint<2>& operator/=(myuint<2> rhs){
            num /= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<2> operator/(myuint<2> lhs, myuint<2> rhs){
            lhs /=rhs;
            return lhs;
        }
        // modulus function, gives answer to lhs mod rhs
        myuint<2>& operator%=(myuint<2> rhs){
            num %= rhs.num;
            return *this;
        }
        // modulus operator reuses compound assignment
        friend myuint<2> operator%(myuint<2> lhs, myuint<2> rhs){
            lhs %=rhs;
            return lhs;
        }
        //Comparison operators
        inline friend bool operator< (const myuint<2>& lhs, const myuint<2>& rhs){ 
            return lhs.num < rhs.num;
        }
        inline friend bool operator> (const myuint<2>& lhs, const myuint<2>& rhs){ 
            return rhs < lhs; 
        }
        inline friend bool operator<=(const myuint<2>& lhs, const myuint<2>& rhs){ 
            return !(lhs > rhs); 
        }
        inline friend bool operator>=(const myuint<2>& lhs, const myuint<2>& rhs){ 
            return !(lhs < rhs); 
        }
        inline friend bool operator==(const myuint<2>& lhs, const myuint<2>& rhs){ 
            return lhs.num == rhs.num;
        }
        inline friend bool operator!=(const myuint<2>& lhs, const myuint<2>& rhs){ 
            return !(lhs==rhs); 
        }
        //Write to stream (convert to base 10)
        friend std::ostream& operator<<(std::ostream& os, const myuint<2>& bigint){
            os << bigint.num;
            return os;
        }
        //Quick function that returns if value is equal to 0
        bool is_zero(){
            return num==0;
        }
        //Gets base 10 representation of uint
        std::string to_string() const {
            return std::to_string(num);
        }
};
template <>
class myuint<4>{
    private:
        uint8_t num =0;
    public:
        //Default constructor - creates structure with value 0
        myuint(){
        }
        //Unsigned Integer constructor
        myuint(uint8_t num){
            this->num = num;
        }
        /* String constructor (for base 10) 
           Accepts a string representation of an integer
           Behaviour when number too large is undefined
        */
        myuint(std::string s){    
            num = std::stoul(s);
        }
        // Copy assignment
        myuint<4>& operator=(const myuint<4>& other){
            // Guard self assignment
            if (this == &other){
                return *this;
            }
            num = other.num;
            return *this;
        }
        // prefix increment
        myuint<4>& operator++(){
            num++;
            return *this; // return new value by reference
        }
        // postfix increment
        myuint<4> operator++(int){
            myuint<4> old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }   
        // prefix decrement
        myuint<4>& operator--(){
            num--;
            return *this; // return new value by reference
        }
        // postfix decrement
        myuint<4> operator--(int){
            myuint<4> old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }
        // (compound assignment) addition of rhs to this
        myuint<4>& operator+=(const myuint<4>& rhs){
            num += rhs.num;
            return *this;
        }
        // addition operator reuses compound assignment
        friend myuint<4> operator+(myuint<4> lhs, const myuint<4>& rhs){
            lhs+= rhs;
            return lhs;
        }
        // (compound assignment) subtraction of rhs from this
        myuint<4>& operator-=(const myuint<4>& rhs){
            num -= rhs.num;
            return *this;
        }
        // subtraction operator reuses compound assignment
        friend myuint<4> operator-(myuint<4> lhs, const myuint<4>& rhs){
            lhs-= rhs;
            return lhs;
        }
        // (compound assignment) right shift of this by rhs
        myuint<4>& operator>>=(const uint& rhs){
            num >>= rhs;
            return *this;
        }
        // right shift operator reuses compound assignment
        friend myuint<4> operator>>(myuint<4> lhs, const uint& rhs){
            lhs>>= rhs;
            return lhs;
        }
        // (compound assignment) left shift of this by rhs
        myuint<4>& operator<<=(const uint& rhs){
            num <<= rhs;
            return *this;
        }
        // left shift operator reuses compound assignment
        friend myuint<4> operator<<(myuint<4> lhs, const uint& rhs){
            lhs<<= rhs;
            return lhs;
        }
        // Multiplcation of rhs to lhs
        myuint<4>& operator*=(const myuint<4>& rhs){
            num *= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<4> operator*(myuint<4> lhs, const myuint<4>& rhs){
            lhs *=rhs;
            return lhs;
        }
        // (compound assignment) division of lhs by rhs
        myuint<4>& operator/=(myuint<4> rhs){
            num /= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<4> operator/(myuint<4> lhs, myuint<4> rhs){
            lhs /=rhs;
            return lhs;
        }
        // modulus function, gives answer to lhs mod rhs
        myuint<4>& operator%=(myuint<4> rhs){
            num %= rhs.num;
            return *this;
        }
        // modulus operator reuses compound assignment
        friend myuint<4> operator%(myuint<4> lhs, myuint<4> rhs){
            lhs %=rhs;
            return lhs;
        }
        //Comparison operators
        inline friend bool operator< (const myuint<4>& lhs, const myuint<4>& rhs){ 
            return lhs.num < rhs.num;
        }
        inline friend bool operator> (const myuint<4>& lhs, const myuint<4>& rhs){ 
            return rhs < lhs; 
        }
        inline friend bool operator<=(const myuint<4>& lhs, const myuint<4>& rhs){ 
            return !(lhs > rhs); 
        }
        inline friend bool operator>=(const myuint<4>& lhs, const myuint<4>& rhs){ 
            return !(lhs < rhs); 
        }
        inline friend bool operator==(const myuint<4>& lhs, const myuint<4>& rhs){ 
            return lhs.num == rhs.num;
        }
        inline friend bool operator!=(const myuint<4>& lhs, const myuint<4>& rhs){ 
            return !(lhs==rhs); 
        }
        //Write to stream (convert to base 10)
        friend std::ostream& operator<<(std::ostream& os, const myuint<4>& bigint){
            os << bigint.num;
            return os;
        }
        //Quick function that returns if value is equal to 0
        bool is_zero(){
            return num==0;
        }
        //Gets base 10 representation of uint
        std::string to_string() const {
            return std::to_string(num);
        }
};
template <>
class myuint<8>{
    private:
        uint8_t num =0;
    public:
        //Default constructor - creates structure with value 0
        myuint(){
        }
        //Unsigned Integer constructor
        myuint(uint8_t num){
            this->num = num;
        }
        /* String constructor (for base 10) 
           Accepts a string representation of an integer
           Behaviour when number too large is undefined
        */
        myuint(std::string s){    
            num = std::stoul(s);
        }
        // Copy assignment
        myuint<8>& operator=(const myuint<8>& other){
            // Guard self assignment
            if (this == &other){
                return *this;
            }
            num = other.num;
            return *this;
        }
        // prefix increment
        myuint<8>& operator++(){
            num++;
            return *this; // return new value by reference
        }
        // postfix increment
        myuint<8> operator++(int){
            myuint<8> old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }   
        // prefix decrement
        myuint<8>& operator--(){
            num--;
            return *this; // return new value by reference
        }
        // postfix decrement
        myuint<8> operator--(int){
            myuint<8> old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }
        // (compound assignment) addition of rhs to this
        myuint<8>& operator+=(const myuint<8>& rhs){
            num += rhs.num;
            return *this;
        }
        // addition operator reuses compound assignment
        friend myuint<8> operator+(myuint<8> lhs, const myuint<8>& rhs){
            lhs+= rhs;
            return lhs;
        }
        // (compound assignment) subtraction of rhs from this
        myuint<8>& operator-=(const myuint<8>& rhs){
            num -= rhs.num;
            return *this;
        }
        // subtraction operator reuses compound assignment
        friend myuint<8> operator-(myuint<8> lhs, const myuint<8>& rhs){
            lhs-= rhs;
            return lhs;
        }
        // (compound assignment) right shift of this by rhs
        myuint<8>& operator>>=(const uint& rhs){
            num >>= rhs;
            return *this;
        }
        // right shift operator reuses compound assignment
        friend myuint<8> operator>>(myuint<8> lhs, const uint& rhs){
            lhs>>= rhs;
            return lhs;
        }
        // (compound assignment) left shift of this by rhs
        myuint<8>& operator<<=(const uint& rhs){
            num <<= rhs;
            return *this;
        }
        // left shift operator reuses compound assignment
        friend myuint<8> operator<<(myuint<8> lhs, const uint& rhs){
            lhs<<= rhs;
            return lhs;
        }
        // Multiplcation of rhs to lhs
        myuint<8>& operator*=(const myuint<8>& rhs){
            num *= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<8> operator*(myuint<8> lhs, const myuint<8>& rhs){
            lhs *=rhs;
            return lhs;
        }
        // (compound assignment) division of lhs by rhs
        myuint<8>& operator/=(myuint<8> rhs){
            num /= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<8> operator/(myuint<8> lhs, myuint<8> rhs){
            lhs /=rhs;
            return lhs;
        }
        // modulus function, gives answer to lhs mod rhs
        myuint<8>& operator%=(myuint<8> rhs){
            num %= rhs.num;
            return *this;
        }
        // modulus operator reuses compound assignment
        friend myuint<8> operator%(myuint<8> lhs, myuint<8> rhs){
            lhs %=rhs;
            return lhs;
        }
        //Comparison operators
        inline friend bool operator< (const myuint<8>& lhs, const myuint<8>& rhs){ 
            return lhs.num < rhs.num;
        }
        inline friend bool operator> (const myuint<8>& lhs, const myuint<8>& rhs){ 
            return rhs < lhs; 
        }
        inline friend bool operator<=(const myuint<8>& lhs, const myuint<8>& rhs){ 
            return !(lhs > rhs); 
        }
        inline friend bool operator>=(const myuint<8>& lhs, const myuint<8>& rhs){ 
            return !(lhs < rhs); 
        }
        inline friend bool operator==(const myuint<8>& lhs, const myuint<8>& rhs){ 
            return lhs.num == rhs.num;
        }
        inline friend bool operator!=(const myuint<8>& lhs, const myuint<8>& rhs){ 
            return !(lhs==rhs); 
        }
        //Write to stream (convert to base 10)
        friend std::ostream& operator<<(std::ostream& os, const myuint<8>& bigint){
            os << bigint.num;
            return os;
        }
        //Quick function that returns if value is equal to 0
        bool is_zero(){
            return num==0;
        }
        //Gets base 10 representation of uint
        std::string to_string() const {
            return std::to_string(num);
        }
};
template <>
class myuint<16>{
    private:
        uint16_t num =0;
    public:
        //Default constructor - creates structure with value 0
        myuint(){
        }
        //Unsigned Integer constructor
        myuint(uint16_t num){
            this->num = num;
        }
        /* String constructor (for base 10) 
           Accepts a string representation of an integer
           Behaviour when number too large is undefined
        */
        myuint(std::string s){    
            num = std::stoul(s);
        }
        // Copy assignment
        myuint<16>& operator=(const myuint<16>& other){
            // Guard self assignment
            if (this == &other){
                return *this;
            }
            num = other.num;
            return *this;
        }
        // prefix increment
        myuint<16>& operator++(){
            num++;
            return *this; // return new value by reference
        }
        // postfix increment
        myuint<16> operator++(int){
            myuint<16> old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }   
        // prefix decrement
        myuint<16>& operator--(){
            num--;
            return *this; // return new value by reference
        }
        // postfix decrement
        myuint<16> operator--(int){
            myuint<16> old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }
        // (compound assignment) addition of rhs to this
        myuint<16>& operator+=(const myuint<16>& rhs){
            num += rhs.num;
            return *this;
        }
        // addition operator reuses compound assignment
        friend myuint<16> operator+(myuint<16> lhs, const myuint<16>& rhs){
            lhs+= rhs;
            return lhs;
        }
        // (compound assignment) subtraction of rhs from this
        myuint<16>& operator-=(const myuint<16>& rhs){
            num -= rhs.num;
            return *this;
        }
        // subtraction operator reuses compound assignment
        friend myuint<16> operator-(myuint<16> lhs, const myuint<16>& rhs){
            lhs-= rhs;
            return lhs;
        }
        // (compound assignment) right shift of this by rhs
        myuint<16>& operator>>=(const uint& rhs){
            num >>= rhs;
            return *this;
        }
        // right shift operator reuses compound assignment
        friend myuint<16> operator>>(myuint<16> lhs, const uint& rhs){
            lhs>>= rhs;
            return lhs;
        }
        // (compound assignment) left shift of this by rhs
        myuint<16>& operator<<=(const uint& rhs){
            num <<= rhs;
            return *this;
        }
        // left shift operator reuses compound assignment
        friend myuint<16> operator<<(myuint<16> lhs, const uint& rhs){
            lhs<<= rhs;
            return lhs;
        }
        // Multiplcation of rhs to lhs
        myuint<16>& operator*=(const myuint<16>& rhs){
            num *= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<16> operator*(myuint<16> lhs, const myuint<16>& rhs){
            lhs *=rhs;
            return lhs;
        }
        // (compound assignment) division of lhs by rhs
        myuint<16>& operator/=(myuint<16> rhs){
            num /= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<16> operator/(myuint<16> lhs, myuint<16> rhs){
            lhs /=rhs;
            return lhs;
        }
        // modulus function, gives answer to lhs mod rhs
        myuint<16>& operator%=(myuint<16> rhs){
            num %= rhs.num;
            return *this;
        }
        // modulus operator reuses compound assignment
        friend myuint<16> operator%(myuint<16> lhs, myuint<16> rhs){
            lhs %=rhs;
            return lhs;
        }
        //Comparison operators
        inline friend bool operator< (const myuint<16>& lhs, const myuint<16>& rhs){ 
            return lhs.num < rhs.num;
        }
        inline friend bool operator> (const myuint<16>& lhs, const myuint<16>& rhs){ 
            return rhs < lhs; 
        }
        inline friend bool operator<=(const myuint<16>& lhs, const myuint<16>& rhs){ 
            return !(lhs > rhs); 
        }
        inline friend bool operator>=(const myuint<16>& lhs, const myuint<16>& rhs){ 
            return !(lhs < rhs); 
        }
        inline friend bool operator==(const myuint<16>& lhs, const myuint<16>& rhs){ 
            return lhs.num == rhs.num;
        }
        inline friend bool operator!=(const myuint<16>& lhs, const myuint<16>& rhs){ 
            return !(lhs==rhs); 
        }
        //Write to stream (convert to base 10)
        friend std::ostream& operator<<(std::ostream& os, const myuint<16>& bigint){
            os << bigint.num;
            return os;
        }
        //Quick function that returns if value is equal to 0
        bool is_zero(){
            return num==0;
        }
        //Gets base 10 representation of uint
        std::string to_string() const {
            return std::to_string(num);
        }
};
template <>
class myuint<32>{
    private:
        uint32_t num =0;
    public:
        //Default constructor - creates structure with value 0
        myuint(){
        }
        //Unsigned Integer constructor
        myuint(uint32_t num){
            this->num = num;
        }
        /* String constructor (for base 10) 
           Accepts a string representation of an integer
           Behaviour when number too large is undefined
        */
        myuint(std::string s){    
            num = std::stoul(s);
        }
        // Copy assignment
        myuint<32>& operator=(const myuint<32>& other){
            // Guard self assignment
            if (this == &other){
                return *this;
            }
            num = other.num;
            return *this;
        }
        // prefix increment
        myuint<32>& operator++(){
            num++;
            return *this; // return new value by reference
        }
        // postfix increment
        myuint<32> operator++(int){
            myuint<32> old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }   
        // prefix decrement
        myuint<32>& operator--(){
            num--;
            return *this; // return new value by reference
        }
        // postfix decrement
        myuint<32> operator--(int){
            myuint<32> old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }
        // (compound assignment) addition of rhs to this
        myuint<32>& operator+=(const myuint<32>& rhs){
            num += rhs.num;
            return *this;
        }
        // addition operator reuses compound assignment
        friend myuint<32> operator+(myuint<32> lhs, const myuint<32>& rhs){
            lhs+= rhs;
            return lhs;
        }
        // (compound assignment) subtraction of rhs from this
        myuint<32>& operator-=(const myuint<32>& rhs){
            num -= rhs.num;
            return *this;
        }
        // subtraction operator reuses compound assignment
        friend myuint<32> operator-(myuint<32> lhs, const myuint<32>& rhs){
            lhs-= rhs;
            return lhs;
        }
        // (compound assignment) right shift of this by rhs
        myuint<32>& operator>>=(const uint& rhs){
            num >>= rhs;
            return *this;
        }
        // right shift operator reuses compound assignment
        friend myuint<32> operator>>(myuint<32> lhs, const uint& rhs){
            lhs>>= rhs;
            return lhs;
        }
        // (compound assignment) left shift of this by rhs
        myuint<32>& operator<<=(const uint& rhs){
            num <<= rhs;
            return *this;
        }
        // left shift operator reuses compound assignment
        friend myuint<32> operator<<(myuint<32> lhs, const uint& rhs){
            lhs<<= rhs;
            return lhs;
        }
        // Multiplcation of rhs to lhs
        myuint<32>& operator*=(const myuint<32>& rhs){
            num *= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<32> operator*(myuint<32> lhs, const myuint<32>& rhs){
            lhs *=rhs;
            return lhs;
        }
        // (compound assignment) division of lhs by rhs
        myuint<32>& operator/=(myuint<32> rhs){
            num /= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<32> operator/(myuint<32> lhs, myuint<32> rhs){
            lhs /=rhs;
            return lhs;
        }
        // modulus function, gives answer to lhs mod rhs
        myuint<32>& operator%=(myuint<32> rhs){
            num %= rhs.num;
            return *this;
        }
        // modulus operator reuses compound assignment
        friend myuint<32> operator%(myuint<32> lhs, myuint<32> rhs){
            lhs %=rhs;
            return lhs;
        }
        //Comparison operators
        inline friend bool operator< (const myuint<32>& lhs, const myuint<32>& rhs){ 
            return lhs.num < rhs.num;
        }
        inline friend bool operator> (const myuint<32>& lhs, const myuint<32>& rhs){ 
            return rhs < lhs; 
        }
        inline friend bool operator<=(const myuint<32>& lhs, const myuint<32>& rhs){ 
            return !(lhs > rhs); 
        }
        inline friend bool operator>=(const myuint<32>& lhs, const myuint<32>& rhs){ 
            return !(lhs < rhs); 
        }
        inline friend bool operator==(const myuint<32>& lhs, const myuint<32>& rhs){ 
            return lhs.num == rhs.num;
        }
        inline friend bool operator!=(const myuint<32>& lhs, const myuint<32>& rhs){ 
            return !(lhs==rhs); 
        }
        //Write to stream (convert to base 10)
        friend std::ostream& operator<<(std::ostream& os, const myuint<32>& bigint){
            os << bigint.num;
            return os;
        }
        //Quick function that returns if value is equal to 0
        bool is_zero(){
            return num==0;
        }
        //Gets base 10 representation of uint
        std::string to_string() const {
            return std::to_string(num);
        }
};
template <>
class myuint<64>{
    private:
        uint64_t num =0;
    public:
        //Default constructor - creates structure with value 0
        myuint(){
        }
        //Unsigned Integer constructor
        myuint(uint64_t num){
            this->num = num;
        }
        /* String constructor (for base 10) 
           Accepts a string representation of an integer
           Behaviour when number too large is undefined
        */
        myuint(std::string s){    
            num = std::stoul(s);
        }
        // Copy assignment
        myuint<64>& operator=(const myuint<64>& other){
            // Guard self assignment
            if (this == &other){
                return *this;
            }
            num = other.num;
            return *this;
        }
        // prefix increment
        myuint<64>& operator++(){
            num++;
            return *this; // return new value by reference
        }
        // postfix increment
        myuint<64> operator++(int){
            myuint<64> old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }   
        // prefix decrement
        myuint<64>& operator--(){
            num--;
            return *this; // return new value by reference
        }
        // postfix decrement
        myuint<64> operator--(int){
            myuint<64> old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }
        // (compound assignment) addition of rhs to this
        myuint<64>& operator+=(const myuint<64>& rhs){
            num += rhs.num;
            return *this;
        }
        // addition operator reuses compound assignment
        friend myuint<64> operator+(myuint<64> lhs, const myuint<64>& rhs){
            lhs+= rhs;
            return lhs;
        }
        // (compound assignment) subtraction of rhs from this
        myuint<64>& operator-=(const myuint<64>& rhs){
            num -= rhs.num;
            return *this;
        }
        // subtraction operator reuses compound assignment
        friend myuint<64> operator-(myuint<64> lhs, const myuint<64>& rhs){
            lhs-= rhs;
            return lhs;
        }
        // (compound assignment) right shift of this by rhs
        myuint<64>& operator>>=(const uint& rhs){
            num >>= rhs;
            return *this;
        }
        // right shift operator reuses compound assignment
        friend myuint<64> operator>>(myuint<64> lhs, const uint& rhs){
            lhs>>= rhs;
            return lhs;
        }
        // (compound assignment) left shift of this by rhs
        myuint<64>& operator<<=(const uint& rhs){
            num <<= rhs;
            return *this;
        }
        // left shift operator reuses compound assignment
        friend myuint<64> operator<<(myuint<64> lhs, const uint& rhs){
            lhs<<= rhs;
            return lhs;
        }
        // Multiplcation of rhs to lhs
        myuint<64>& operator*=(const myuint<64>& rhs){
            num *= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<64> operator*(myuint<64> lhs, const myuint<64>& rhs){
            lhs *=rhs;
            return lhs;
        }
        // (compound assignment) division of lhs by rhs
        myuint<64>& operator/=(myuint<64> rhs){
            num /= rhs.num;
            return *this;
        }
        // multiplication operator reuses compound assignment
        friend myuint<64> operator/(myuint<64> lhs, myuint<64> rhs){
            lhs /=rhs;
            return lhs;
        }
        // modulus function, gives answer to lhs mod rhs
        myuint<64>& operator%=(myuint<64> rhs){
            num %= rhs.num;
            return *this;
        }
        // modulus operator reuses compound assignment
        friend myuint<64> operator%(myuint<64> lhs, myuint<64> rhs){
            lhs %=rhs;
            return lhs;
        }
        //Comparison operators
        inline friend bool operator< (const myuint<64>& lhs, const myuint<64>& rhs){ 
            return lhs.num < rhs.num;
        }
        inline friend bool operator> (const myuint<64>& lhs, const myuint<64>& rhs){ 
            return rhs < lhs; 
        }
        inline friend bool operator<=(const myuint<64>& lhs, const myuint<64>& rhs){ 
            return !(lhs > rhs); 
        }
        inline friend bool operator>=(const myuint<64>& lhs, const myuint<64>& rhs){ 
            return !(lhs < rhs); 
        }
        inline friend bool operator==(const myuint<64>& lhs, const myuint<64>& rhs){ 
            return lhs.num == rhs.num;
        }
        inline friend bool operator!=(const myuint<64>& lhs, const myuint<64>& rhs){ 
            return !(lhs==rhs); 
        }
        //Write to stream (convert to base 10)
        friend std::ostream& operator<<(std::ostream& os, const myuint<64>& bigint){
            os << bigint.num;
            return os;
        }
        //Quick function that returns if value is equal to 0
        bool is_zero(){
            return num==0;
        }
        //Gets base 10 representation of uint
        std::string to_string() const {
            return std::to_string(num);
        }
};

