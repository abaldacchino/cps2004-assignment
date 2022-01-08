#include "minunit.h"
#include "dag.hpp"
#include <memory>
#include <iostream>
#include <iomanip>
#include <string>

// Create Node tests
MU_CUSTOM_TEST_START(int_create_node)
    auto n = std::make_shared<Node<int>>(2);
    
    mu_custom_check(n!=NULL, "Node creation failed", 1, 2);
    mu_custom_check(n.use_count()==1, "Node creation must only create 1 reference to node", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(double_create_node)
    auto n = std::make_shared<Node<double>>(8.8);
    
    mu_custom_check(n!=NULL, "Node creation failed", 1, 2);
    mu_custom_check(n.use_count()==1, "Node creation must only create 1 reference to node", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(string_create_node)
    auto n = std::make_shared<Node<std::string>>("Hello World!");
    
    mu_custom_check(n!=NULL, "Node creation failed", 1, 2);
    mu_custom_check(n.use_count()==1, "Node creation must only create 1 reference to node", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(array_create_node)
    std::array<int,5> ar{{1,3,2,3,4}};
    auto n = std::make_shared<Node<std::array<int,5>>>(ar);
    
    mu_custom_check(n!=NULL, "Node creation failed", 1, 2);
    mu_custom_check(n.use_count()==1, "Node creation must only create 1 reference to node", 2, 2);
MU_CUSTOM_TEST_END

// Get data tests
MU_CUSTOM_TEST_START(int_get_data)
    auto n1 = std::make_shared<Node<int>>(2);
    int n1_val = n1->get_data();

    mu_custom_check(n1_val==2, "get_data failed for type integer, data=2", 1, 2);

    auto n2 = std::make_shared<Node<int>>(5);
    int n2_val = n2->get_data();
    
    mu_custom_check(n2_val==5, "get_data failed for type integer, data=5", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(string_get_data)
    auto n3 = std::make_shared<Node<std::string>>("Hello world!");
    std::string n3_val = n3->get_data();

    mu_custom_check(n3_val.compare("Hello world!")==0, "get_data failed for type string, data=Hello world!", 1,1);

MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(array_get_data)
    std::array<int, 5> ar = {1,2,3,4,5};
    auto n4 = std::make_shared<Node<std::array<int,5>>>(ar);
    auto n4_val = n4->get_data();
    mu_custom_check(ar == n4_val, "get_data failed for type std::array<int,5>, {1,2,3,4,5}", 1,1);

MU_CUSTOM_TEST_END

// Add parent tests
MU_CUSTOM_TEST_START(int_add_one_parent)
    auto n1 = std::make_shared<Node<int>>(2);
    auto n2 = std::make_shared<Node<int>>(5);
    auto n3 = std::make_shared<Node<int>>(20);
    auto n4 = std::make_shared<Node<int>>(5);
    
    n1->add_parent(n2);
    
    mu_custom_check(n2.use_count()==1, "Use count of parent node should not increase after call", 1, 4);
    mu_custom_check(n1->contains_parent(n2), "contains_parent(n2) should succeed after adding n2", 2,4);
    mu_custom_check(!n1->contains_parent(n3), "contains_parent(n3) should not succeed as n3 not added", 3,4);
    mu_custom_check(!n1->contains_parent(n4), "contains_parent(n4) should not succeed as n4 not added", 4,4);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(int_add_two_parents)
    auto n1 = std::make_shared<Node<int>>(2);
    auto n2 = std::make_shared<Node<int>>(5);
    auto n3 = std::make_shared<Node<int>>(20);
    auto n4 = std::make_shared<Node<int>>(20);
    
    n1->add_parent(n2);
    n1->add_parent(n3);
    
    mu_custom_check(n2.use_count()==1, "Use count of parent node should not increase after call", 1, 4);
    mu_custom_check(n1->contains_parent(n2), "contains_parent(n2) should succeed after adding n2", 2,4);
    mu_custom_check(n1->contains_parent(n3), "contains_parent(n3) should succeed after adding n3", 3,4);
    mu_custom_check(!n1->contains_parent(n4), "contains_parent(n4) should not succeed as n4 not added", 4,4);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(int_add_ten_parents)
    int n=10;
    auto child = std::make_shared<Node<int>>();

    std::array<std::shared_ptr<Node<int>>,10> parents;
    for(int i=0; i<n; i++){
        parents[i] = std::make_shared<Node<int>>(i*10);
        child->add_parent(parents[i]);
    }
    
    for(int i=0;i<n;i++){
        mu_custom_check(parents[i].use_count()==1, "Use count of parent node should not increase after adding parent", (2*i)+1, n*2+1);
        mu_custom_check(child->contains_parent(parents[i]), "contains_parent(node) should succeed after adding node", (2*i)+2,n*2+1);
    }
    mu_custom_check(child.use_count()==1, "Use count of child node should not increase after adding parent", n*2+1, n*2+1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(int_add_one_child)
    auto n1 = std::make_shared<Node<int>>(1);
    auto n2 = std::make_shared<Node<int>>(2);
    auto n3 = std::make_shared<Node<int>>(2);
    n1->add_child(n2);

    mu_custom_check(n2.use_count()==2, "Use count of n2 should be 2 as n1 now owns n2", 1, 4);
    mu_custom_check(n1->contains_child(n2), "n2 is a child of n1 so contains_child(n2) should succeed", 2,4);
    mu_custom_check(!n1->contains_child(n3), "n3 not a child of n1 so contains_child(n3) should fail", 3,4);
    mu_custom_check(n1.use_count()==1, "Use count of n1 should remain 1", 4,4);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(int_add_ten_children)
    int n=10;
    auto parent = std::make_shared<Node<int>>();

    std::array<std::shared_ptr<Node<int>>,10> children;
    for(int i=0; i<n; i++){
        children[i] = std::make_shared<Node<int>>(i*10);
        parent->add_child(children[i]);
    }
    
    for(int i=0;i<n;i++){
        mu_custom_check(children[i].use_count()==2, "Use count should increase to 2 as node now owned by parent", (2*i)+1, n*2+1);
        mu_custom_check(parent->contains_child(children[i]), "contains_child(node) should succeed after adding child", (2*i)+2,n*2+1);
    }
    mu_custom_check(parent.use_count()==1, "Use count of parent should remain 1", n*2+1, n*2+1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(int_add_child_to_two_nodes)
    auto n1 = std::make_shared<Node<int>>(1);
    auto n2 = std::make_shared<Node<int>>(2);
    auto child = std::make_shared<Node<int>>(20);
    n1->add_child(child);
    n2->add_child(child);

    mu_custom_check(child.use_count()==3, "n1, n2 both have ownership of child so use_count=3", 1, 3);
    mu_custom_check(n1->contains_child(child), "child is a child of n1 so contains_child(child) should succeed", 2,3);
    mu_custom_check(n2->contains_child(child), "child is a child of n2 so contains_child(child) should succeed", 3,3);

MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(int_add_child_to_ten_nodes)
    int n=10;
    auto child = std::make_shared<Node<int>>();

    std::array<std::shared_ptr<Node<int>>,10> parents;
    for(int i=0; i<n; i++){
        parents[i] = std::make_shared<Node<int>>(i*10);
        parents[i]->add_child(child);
        mu_custom_check(child.use_count()==2+i, "Use count should increase with each child added", (2*i)+1, n*2);
        mu_custom_check(parents[i]->contains_child(child), "contains_child should return true", (2*i)+2, n*2);
    }
    
MU_CUSTOM_TEST_END

MU_TEST_SUITE(create_node){
	MU_RUN_TEST(int_create_node);
    MU_RUN_TEST(double_create_node);
    MU_RUN_TEST(string_create_node);
    MU_RUN_TEST(array_create_node);
}

MU_TEST_SUITE(get_data){
    MU_RUN_TEST(int_get_data);
    MU_RUN_TEST(string_get_data);
    MU_RUN_TEST(array_get_data);
}

MU_TEST_SUITE(add_contains_parent){
	MU_RUN_TEST(int_add_one_parent);
	MU_RUN_TEST(int_add_two_parents);
	MU_RUN_TEST(int_add_ten_parents);
}

MU_TEST_SUITE(add_contains_child){
	MU_RUN_TEST(int_add_one_child);
    MU_RUN_TEST(int_add_ten_children);
    MU_RUN_TEST(int_add_child_to_two_nodes);
    MU_RUN_TEST(int_add_child_to_ten_nodes);
}

int main(){
    // //testing pointer behaviour
    // auto m = std::make_shared<int>(10);
    // auto k = m;
    // std::cout << m << std::endl;
    // std::cout << *m << std::endl;
    // std::weak_ptr<int> w = m;

    // m.reset();
    // k.reset();
    // auto self = w.lock();
    // if(self){
    //     std::cout << self << std::endl;
    //     std::cout << *self << std::endl;
    // }
    
    //  //   std::cout << "k"<<k << std::endl;
    //   //  std::cout << "k"<<*k << std::endl;

    //NODE tests
    MU_RUN_SUITE(create_node);
    MU_RUN_SUITE(get_data);
    MU_RUN_SUITE(add_contains_parent);
    MU_RUN_SUITE(add_contains_child);

    MU_REPORT();
    return MU_EXIT_CODE;
    return 0;
}