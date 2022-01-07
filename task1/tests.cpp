#include "minunit.h"
#include "dag.hpp"
#include <memory>
#include <iostream>
#include <iomanip>

MU_CUSTOM_TEST_START(test_create_node)
    auto n = std::make_shared<Node<int>>(2);
    
    mu_custom_check(n!=NULL, "Node creation failed", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(test_get_data)
    auto n1 = std::make_shared<Node<int>>(2);
    int n1_val = n1->get_data();

    mu_custom_check(n1_val==2, "Node creation failed, value must not be null", 1, 2);

    auto n2 = std::make_shared<Node<int>>(5);
    int n2_val = n2->get_data();
    
    mu_custom_check(n2_val==5, "Node creation failed, value must not be null", 2, 2);

MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(test_node_ownership)
    auto n1 = std::make_shared<Node<int>>(2);
    auto n2 = std::make_shared<Node<int>>(5);
    
    mu_custom_check(1==1, "Node creation failed, value must not be null", 1, 1);
MU_CUSTOM_TEST_END

MU_TEST_SUITE(node_test_suite) {
	MU_RUN_TEST(test_create_node);
	MU_RUN_TEST(test_node_ownership);
}

int main(){
    MU_RUN_SUITE(node_test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
    return 0;
}