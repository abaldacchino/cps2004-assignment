#include "minunit.h"
#include "bigint.hpp"

#define MYUINT64_MAX 18446744073709551615
#define UINT128_MAX 340282366920938463463374607431768211455
#define UINT256_MAX 115792089237316195423570985008687907853269984665640564039457584007913129639935
#define UINT512_MAX 134078079299425970995740249982058461274793658205923933777235614437217640300735\
46976801874298166903427690031858186486050853753882811946569946433649006084095
#define UINT1024_MAX 17976931348623159077293051907890247336179769789423065727343008115773267580550\
09631327084773224075360211201138798713933576587897688144166224928474306394741243777678934248654852\
76302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947\
245938479716304835356329624224137215
#define UINT2048_MAX 32317006071311007300714876688669951960444102669715484032130345427524655138867\
89089319720141152291346368871796092189801949411955915049092109508815238644828312063087736730099609\
17501977503896521067960576383840675682767922186426197561618380943384761704705816458520363050428875\
75891541065808607552399123930385521914333389668342420684974786564569494856176035326322058077805659\
33102619270846031415025859286417711672594360371846185735759835115230164590440369761323328723122712\
56847108202097251571017269313234696785425806566979350459972683529986382155251663894373355436021354\
33229604645318478604952148193555853611059596230655
                     
//CONSTN is storable in uint<n> but not in uint<n-1>
#define CONST128 184467440737095516152342341
#define CONST256 11579208923731619542357098500868790785326998466564056
#define CONST512 134078079299425970995740249982058461274793658205923933777235614437217640300735469\
7680187429816690342769003185818648605085375388281194
#define CONST1024 17976931348623159077293051907890247336179769789423065727343008115773267580550096\
31327084773224075360211201138798713933576587897688144166224928474306394741243777678934248654852763\
0221960124609411945308
#define CONST2048 32317006071311007300714876688669951960444102669715484032130345427524655138867890\
89319720141152291346368871796092189801949411955915049092109508815238644828312063087736730099609175\
01977503896521067960576383840675682767922186426197561618380943384761704705816458520363050428875758\
91541065808607552399123930385521914333389668342420684974786564569494856176035326322058

#define ADDCONST512CONST256 1340780792994259709957402499820584612747936582059239337772356144372176\
403007354709259396353548309885126101686687439390412373854845250
#define SUBCONST512CONST256 1340780792994259709957402499820584612747936582059239337772356144372176\
403007354686100978506085070800411904684949857819758376921717138
#define MULCONST512CONST256 1552518092300708935148979488462502555256886017116696556978780610497988\
69181868072902298595367206052023540991432914023289101656629461906549395471559292821402495795445894\
31314660741162864
#define DIVCONST512CONST256 1157920892373161954235709850086879078532699846656405680789151680158262\
59279872000
#define MODCONST512CONST256 1631721757299904655907449194

#define RSHIFTCONST512BY192 2135987035920910082395021706169552114602704522356652769947041607822219\
72578
#define RSHIFTCONST512BY201 4171849679533027504677776769862406473833407270227837441302815640277772\
90
#define LSHIFTCONST512BY192 8416217442477397611585583812608205864880543684517078175197249444909971\
4468753293153818664580441415219631727501698851483408310916002899621587\
88498062005326542348994408680045514506724467277824
#define LSHIFTCONST512BY201 4309103330548427577131818912055401402818838366472744025700991715793905\
3808001686094755156265186004592451444480869811959505055188993484606252\
99711007746727189682685137244183303427442927246245888

#define STRX(s) STR(s)
#define STR(s) #s

MU_CUSTOM_TEST_START(create_no_param)
    myuint<1> n1;
    mu_custom_check(n1.is_zero(), "Default should be initialised as 0", 1, 4);

    myuint<64> n2;
    mu_custom_check(n2.is_zero(), "Default should be initialised as 0", 2, 4);

    myuint<512> n3;
    mu_custom_check(n3.is_zero(), "Default should be initialised as 0", 3, 4);

    myuint<2048> n4;
    mu_custom_check(n4.is_zero(), "Default should be initialised as 0", 4, 4);

MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(create_int_param)
    myuint<1> n1(0);
    mu_custom_check((n1.to_string().compare("0"))==0, "myuint value should match value in constructor", 1, 4);

    myuint<64> n2(100598);
    mu_custom_check((n2.to_string().compare("100598"))==0, "myuint value should match value in constructor", 2, 4);

    myuint<512> n3(280009);
    mu_custom_check((n3.to_string().compare("280009"))==0, "myuint value should match value in constructor", 3, 4);

    myuint<2048> n4(UINT64_MAX);
    mu_custom_check((n4.to_string().compare(STRX(MYUINT64_MAX)))==0, "myuint value should match value in constructor", 4, 4);

MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(create_string_param)
    myuint<1> n1("0");
    mu_custom_check((n1.to_string().compare("0"))==0, "myuint value should match value in constructor", 1, 5);

    myuint<64> n2("100598");
    mu_custom_check((n2.to_string().compare("100598"))==0, "myuint value should match value in constructor", 2, 5);

    myuint<512> n3("280009");
    mu_custom_check((n3.to_string().compare("280009"))==0, "myuint value should match value in constructor", 3, 5);

    myuint<2048> n4(STRX(MYUINT64_MAX));
    mu_custom_check((n4.to_string().compare(STRX(MYUINT64_MAX)))==0, "myuint value should match value in constructor", 4, 5);

    myuint<2048> n5(STRX(CONST2048));
    mu_custom_check((n5.to_string().compare(STRX(CONST2048)))==0, "myuint value should match value in constructor", 5, 5);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(prefix_increment)
    myuint<32> n1("231");
    myuint<32> res1 = n1++;

    mu_custom_check((n1.to_string().compare("232"))==0, "value should be incremented", 1, 6);
    mu_custom_check((res1.to_string().compare("231"))==0, "postfix, so result should not be incremented", 2, 6);

    //increment should carry
    myuint<256> n2("18446744073709551615");
    myuint<256> res2 = n2++;

    mu_custom_check((n2.to_string().compare("18446744073709551616"))==0, 
                    "value should be incremented", 3, 6);
    mu_custom_check((res2.to_string().compare("18446744073709551615"))==0, 
                    "postfix, so result should not be incremented", 4, 6);

    //increment should carry
    myuint<1024> n3("115792089237316195423570985008687907853269984665640564039457584007913129639935");
    myuint<1024> res3 = n3++;

    mu_custom_check((n3.to_string().compare("115792089237316195423570985008687907853269984665640564039457584007913129639936"))==0, 
                    "value should be incremented", 5, 6);
    mu_custom_check((res3.to_string().compare("115792089237316195423570985008687907853269984665640564039457584007913129639935"))==0, 
                    "postfix, so result should not be incremented", 6, 6);
    
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(postfix_increment)
myuint<32> n1("231");
    myuint<32> res1 = ++n1;

    mu_custom_check((n1.to_string().compare("232"))==0, "value should be incremented", 1, 6);
    mu_custom_check((res1.to_string().compare("232"))==0, "prefix, so result should be incremented", 2, 6);

    //increment should carry
    myuint<256> n2("18446744073709551615");
    myuint<256> res2 = ++n2;

    mu_custom_check((n2.to_string().compare("18446744073709551616"))==0, 
                    "value should be incremented", 3, 6);
    mu_custom_check((res2.to_string().compare("18446744073709551616"))==0, 
                    "prefix, so result should be incremented", 4, 6);

    //increment should carry
    myuint<1024> n3("115792089237316195423570985008687907853269984665640564039457584007913129639935");
    myuint<1024> res3 = ++n3;

    mu_custom_check((n3.to_string().compare("115792089237316195423570985008687907853269984665640564039457584007913129639936"))==0, 
                    "value should be incremented", 5, 6);
    mu_custom_check((res3.to_string().compare("115792089237316195423570985008687907853269984665640564039457584007913129639936"))==0, 
                    "prefix, so result should be incremented", 6, 6);
    
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(prefix_decrement)
    myuint<32> n1("232");
    myuint<32> res1 = n1--;

    mu_custom_check((n1.to_string().compare("231"))==0, "value should be decremented", 1, 6);
    mu_custom_check((res1.to_string().compare("232"))==0, "postfix, so result should not be decremented", 2, 6);

    //decrement should carry
    myuint<256> n2("18446744073709551616");
    myuint<256> res2 = n2--;

    mu_custom_check((n2.to_string().compare("18446744073709551615"))==0, 
                    "value should be decremented", 3, 6);
    mu_custom_check((res2.to_string().compare("18446744073709551616"))==0, 
                    "postfix, so result should not be decremented", 4, 6);

    //decrement should carry
    myuint<1024> n3("115792089237316195423570985008687907853269984665640564039457584007913129639936");
    myuint<1024> res3 = n3--;

    mu_custom_check((n3.to_string().compare("115792089237316195423570985008687907853269984665640564039457584007913129639935"))==0, 
                    "value should be decremented", 5, 6);
    mu_custom_check((res3.to_string().compare("115792089237316195423570985008687907853269984665640564039457584007913129639936"))==0, 
                    "postfix, so result should not be decremented", 6, 6);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(postfix_decrement)
    myuint<32> n1("232");
    myuint<32> res1 = --n1;

    mu_custom_check((n1.to_string().compare("231"))==0, "value should be decremented", 1, 6);
    mu_custom_check((res1.to_string().compare("231"))==0, "prefix, so result should be decremented", 2, 6);

    //decrement should carry
    myuint<256> n2("18446744073709551616");
    myuint<256> res2 = --n2;

    mu_custom_check((n2.to_string().compare("18446744073709551615"))==0, 
                    "value should be decremented", 3, 6);
    mu_custom_check((res2.to_string().compare("18446744073709551615"))==0, 
                    "prefix, so result should be decremented", 4, 6);

    //decrement should carry
    myuint<1024> n3("115792089237316195423570985008687907853269984665640564039457584007913129639936");
    myuint<1024> res3 = --n3;

    mu_custom_check((n3.to_string().compare("115792089237316195423570985008687907853269984665640564039457584007913129639935"))==0, 
                    "value should be decremented", 5, 6);
    mu_custom_check((res3.to_string().compare("115792089237316195423570985008687907853269984665640564039457584007913129639935"))==0, 
                    "prefix, so result should be decremented", 6, 6);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(addition)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    myuint<512> c = a + b;

    mu_custom_check((c.to_string().compare(STRX(ADDCONST512CONST256)))==0, \
    "Addition of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_addition)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    a+=b;

    mu_custom_check((a.to_string().compare(STRX(ADDCONST512CONST256)))==0, \
    "Addition of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(subtraction)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    myuint<512> c = a - b;

    mu_custom_check((c.to_string().compare(STRX(SUBCONST512CONST256)))==0, \
    "Subtraction of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_subtraction)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    a-=b;

    mu_custom_check((a.to_string().compare(STRX(SUBCONST512CONST256)))==0, \
    "Subtraction of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(multiplication)
    myuint<2048> a1("1844674407370955");
    myuint<2048> b1("220029988888882");
    myuint<2048> c1 = a1 * b1;

    mu_custom_check((c1.to_string().compare("405883689357436216771249222310"))==0, \
    "Multiplication of two numbers should result in correct expression", 1, 2);

    myuint<2048> a2(STRX(CONST512));
    myuint<2048> b2(STRX(CONST256));
    myuint<2048> c2 = a2 * b2;

    mu_custom_check((c2.to_string().compare(STRX(MULCONST512CONST256)))==0, \
    "Multiplication of two large numbers should result in correct expression", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_multiplication)
    myuint<2048> a1("1844674407370955");
    myuint<2048> b1("220029988888882");
    a1 *= b1;

    mu_custom_check((a1.to_string().compare("405883689357436216771249222310"))==0, \
    "Multiplication of two numbers should result in correct expression", 1, 2);

    myuint<2048> a2(STRX(CONST512));
    myuint<2048> b2(STRX(CONST256));
    a2*=b2;
    mu_custom_check((a2.to_string().compare(STRX(MULCONST512CONST256)))==0, \
    "Multiplication of two large numbers should result in correct expression", 1, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(division)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    myuint<512> c = a / b;

    mu_custom_check((c.to_string().compare(STRX(DIVCONST512CONST256)))==0, \
    "Division of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_division)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    a/=b;

    mu_custom_check((a.to_string().compare(STRX(DIVCONST512CONST256)))==0, \
    "Division of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(modulus)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    myuint<512> c = a % b;

    mu_custom_check((c.to_string().compare(STRX(MODCONST512CONST256)))==0, \
    "Modulus of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_modulus)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    a%=b;

    mu_custom_check((a.to_string().compare(STRX(MODCONST512CONST256)))==0, \
    "Modulus of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(rshift)
    myuint<512> a(STRX(CONST512));
    myuint<512> result1 = a >>192; //right shift by multiple of 64

    mu_custom_check((result1.to_string().compare(STRX(RSHIFTCONST512BY192)))==0, \
    "Right shift by multiple of 64 should result in correct expression", 1, 3);
    
    myuint<512> result2 = a >>201; //right shift by non multiple of 64 (rem!=0)
    mu_custom_check((result2.to_string().compare(STRX(RSHIFTCONST512BY201)))==0, \
    "Right shift by non multiple of 64 should result in correct expression", 2, 3);

    myuint<512> result3 = a >>0; //right shift by 0
    mu_custom_check((result3.to_string().compare(STRX(CONST512)))==0, \
    "Right shift by 0 results in the same result", 3, 3);

MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_rshift)
    myuint<512> a1(STRX(CONST512));
    myuint<512> a2(STRX(CONST512));
    myuint<512> a3(STRX(CONST512));
    a1 >>= 192; //right shift by multiple of 64

    mu_custom_check((a1.to_string().compare(STRX(RSHIFTCONST512BY192)))==0, \
    "Right shift by multiple of 64 should result in correct expression", 1, 3);
    
    a2 >>=201; //right shift by non multiple of 64 (rem!=0)
    mu_custom_check((a2.to_string().compare(STRX(RSHIFTCONST512BY201)))==0, \
    "Right shift by non multiple of 64 should result in correct expression", 2, 3);

    a3 >>=0; //right shift by 0
    mu_custom_check((a3.to_string().compare(STRX(CONST512)))==0, \
    "Right shift by 0 results in the same result", 3, 3);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(lshift)
    myuint<2048> a(STRX(CONST512));
    myuint<2048> result1 = a <<192; //left shift by multiple of 64

    mu_custom_check((result1.to_string().compare(STRX(LSHIFTCONST512BY192)))==0, \
    "Left shift by multiple of 64 should result in correct expression", 1, 3);
    
    myuint<2048> result2 = a <<201; //right shift by non multiple of 64 (rem!=0)
    mu_custom_check((result2.to_string().compare(STRX(LSHIFTCONST512BY201)))==0, \
    "Left shift by non multiple of 64 should result in correct expression", 2, 3);

    myuint<2048> result3 = a <<0; //right shift by 0
    mu_custom_check((result3.to_string().compare(STRX(CONST512)))==0, \
    "Left shift by 0 results in the same result", 3, 3);

MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_lshift)
    myuint<2048> a1(STRX(CONST512));
    myuint<2048> a2(STRX(CONST512));
    myuint<2048> a3(STRX(CONST512));
    a1 <<= 192; //right shift by multiple of 64

    mu_custom_check((a1.to_string().compare(STRX(LSHIFTCONST512BY192)))==0, \
    "Right shift by multiple of 64 should result in correct expression", 1, 3);
    
    a2 <<=201; //right shift by non multiple of 64 (rem!=0)
    mu_custom_check((a2.to_string().compare(STRX(LSHIFTCONST512BY201)))==0, \
    "Right shift by non multiple of 64 should result in correct expression", 2, 3);

    a3 <<=0; //right shift by 0
    mu_custom_check((a3.to_string().compare(STRX(CONST512)))==0, \
    "Right shift by 0 results in the same result", 3, 3);
MU_CUSTOM_TEST_END

MU_TEST_SUITE(create_myuint) {
	MU_RUN_TEST(create_no_param);
	MU_RUN_TEST(create_int_param);
	MU_RUN_TEST(create_string_param);
}

MU_TEST_SUITE(test_unary) {
	MU_RUN_TEST(prefix_increment);
	MU_RUN_TEST(postfix_increment);
	MU_RUN_TEST(prefix_decrement);
	MU_RUN_TEST(postfix_decrement);
}

MU_TEST_SUITE(test_binary) {
	MU_RUN_TEST(addition);
	MU_RUN_TEST(compound_assignment_addition);
	MU_RUN_TEST(subtraction);
	MU_RUN_TEST(compound_assignment_subtraction);
    MU_RUN_TEST(multiplication);
    MU_RUN_TEST(compound_assignment_multiplication);
	MU_RUN_TEST(division);
	MU_RUN_TEST(compound_assignment_division);
	MU_RUN_TEST(modulus);
	MU_RUN_TEST(compound_assignment_modulus);
	MU_RUN_TEST(rshift);
	MU_RUN_TEST(compound_assignment_rshift);
	MU_RUN_TEST(lshift);
	MU_RUN_TEST(compound_assignment_lshift);
}

int main(){
    MU_RUN_SUITE(create_myuint);
    MU_RUN_SUITE(test_unary);
    MU_RUN_SUITE(test_binary);
    MU_REPORT();
    return MU_EXIT_CODE;
    return 0;
}