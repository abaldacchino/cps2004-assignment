#include "minunit.h"
#include "bigint.hpp"

#define MYUINT64_MAX 18446744073709551615
#define UINT128_MAX 340282366920938463463374607431768211455
#define UINT256_MAX 115792089237316195423570985008687907853269984665640564039457584007913129639935
#define UINT512_MAX 13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084095
#define UINT1024_MAX 179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137215
#define UINT2048_MAX 32317006071311007300714876688669951960444102669715484032130345427524655138867890893197201411522913463688717960921898019494119559150490921095088152386448283120630877367300996091750197750389652106796057638384067568276792218642619756161838094338476170470581645852036305042887575891541065808607552399123930385521914333389668342420684974786564569494856176035326322058077805659331026192708460314150258592864177116725943603718461857357598351152301645904403697613233287231227125684710820209725157101726931323469678542580656697935045997268352998638215525166389437335543602135433229604645318478604952148193555853611059596230655
                     
//CONSTN is storable in uint<n> but not in uint<n-1>
#define CONST128 184467440737095516152342341
#define CONST256 11579208923731619542357098500868790785326998466564056
#define CONST512 1340780792994259709957402499820584612747936582059239337772356144372176403007354697680187429816690342769003185818648605085375388281194
#define CONST1024 17976931348623159077293051907890247336179769789423065727343008115773267580550096313270847732240753602112011387987139335765878976881441662249284743063947412437776789342486548527630221960124609411945308
#define CONST2048 32317006071311007300714876688669951960444102669715484032130345427524655138867890893197201411522913463688717960921898019494119559150490921095088152386448283120630877367300996091750197750389652106796057638384067568276792218642619756161838094338476170470581645852036305042887575891541065808607552399123930385521914333389668342420684974786564569494856176035326322058

#define ADDCONST512CONST256 1340780792994259709957402499820584612747936582059239337772356144372176\
403007354709259396353548309885126101686687439390412373854845250
#define SUBCONST512CONST256 1340780792994259709957402499820584612747936582059239337772356144372176\
403007354686100978506085070800411904684949857819758376921717138
#define MULCONST512CONST256 15525180923007089351489794884625025552568860171166965569787806104979886918186807290229859536720605202354099143291402328910165662946190654939547155929282140249579544589431314660741162864
#define DIVCONST512CONST256 1157920892373161954235709850086879078532699846656405680789151680158262\
59279872000


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

    //increment should carry
    myuint<256> n2("18446744073709551616");
    myuint<256> res2 = n2--;

    mu_custom_check((n2.to_string().compare("18446744073709551615"))==0, 
                    "value should be decremented", 3, 6);
    mu_custom_check((res2.to_string().compare("18446744073709551616"))==0, 
                    "postfix, so result should not be decremented", 4, 6);

    //increment should carry
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

    //increment should carry
    myuint<256> n2("18446744073709551616");
    myuint<256> res2 = --n2;

    mu_custom_check((n2.to_string().compare("18446744073709551615"))==0, 
                    "value should be decremented", 3, 6);
    mu_custom_check((res2.to_string().compare("18446744073709551615"))==0, 
                    "prefix, so result should be decremented", 4, 6);

    //increment should carry
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

    mu_custom_check((c.to_string().compare(STRX(ADDCONST512CONST256)))==0, "Addition of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_addition)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    a+=b;

    mu_custom_check((a.to_string().compare(STRX(ADDCONST512CONST256)))==0, "Addition of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(subtraction)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    myuint<512> c = a - b;

    mu_custom_check((c.to_string().compare(STRX(SUBCONST512CONST256)))==0, "Subtraction of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_subtraction)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    a-=b;

    mu_custom_check((a.to_string().compare(STRX(SUBCONST512CONST256)))==0, "Subtraction of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(multiplication)
    myuint<2048> a1("1844674407370955");
    myuint<2048> b1("220029988888882");
    myuint<2048> c1 = a1 * b1;

    mu_custom_check((c1.to_string().compare("405883689357436216771249222310"))==0, "Multiplication of two numbers should result in correct expression", 1, 2);

    myuint<2048> a2(STRX(CONST512));
    myuint<2048> b2(STRX(CONST256));
    myuint<2048> c2 = a2 * b2;

    mu_custom_check((c2.to_string().compare(STRX(MULCONST512CONST256)))==0, "Multiplication of two large numbers should result in correct expression", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_multiplication)
    myuint<2048> a1("1844674407370955");
    myuint<2048> b1("220029988888882");
    a1 *= b1;

    mu_custom_check((a1.to_string().compare("405883689357436216771249222310"))==0, "Multiplication of two numbers should result in correct expression", 1, 2);

    myuint<2048> a2(STRX(CONST512));
    myuint<2048> b2(STRX(CONST256));
    a2*=b2;
    mu_custom_check((a2.to_string().compare(STRX(MULCONST512CONST256)))==0, "Multiplication of two large numbers should result in correct expression", 1, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(division)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    myuint<512> c = a / b;

    mu_custom_check((c.to_string().compare(STRX(DIVCONST512CONST256)))==0, "Division of two numbers should result in correct expression", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(compound_assignment_division)
    myuint<512> a(STRX(CONST512));
    myuint<512> b(STRX(CONST256));
    a/=b;

    mu_custom_check((a.to_string().compare(STRX(DIVCONST512CONST256)))==0, "Division of two numbers should result in correct expression", 1, 1);
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
}

int main(){
    MU_RUN_SUITE(create_myuint);
    MU_RUN_SUITE(test_unary);
    MU_RUN_SUITE(test_binary);
    MU_REPORT();
    return MU_EXIT_CODE;
    return 0;
}