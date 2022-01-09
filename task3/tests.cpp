#include "minunit.h"

#include "bigint.hpp"

MU_CUSTOM_TEST_START(create_bigint)
    myuint<128>* m = new myuint<128>();
    myuint<128> n(2);
    mu_custom_check(m!=NULL, "Integer creation failure", 1, 1);
MU_CUSTOM_TEST_END

MU_TEST_SUITE(test_suite) {
	MU_RUN_TEST(create_bigint);
}

int main(){
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
    return 0;
}
