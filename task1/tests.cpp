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

// DAG Tests
// Create DAG Tests
MU_CUSTOM_TEST_START(int_create_valid_2node_DAG)
    std::vector<Edge<int>> v;
    v.push_back(Edge<int>(4, 3));

    DAG<int> dag(v);
    mu_custom_check(v.empty(), "Vector should be cleared of contents- ownership now belongs to DAG", 1, 2);
    mu_custom_check(!dag.is_empty(), "DAG should not be empty- initialisation should be successful", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(int_create_valid_6node_DAG)
    std::vector<Edge<int>> v;
    v.push_back(Edge<int>(4, 3));
    v.push_back(Edge<int>(4, 5));
    v.push_back(Edge<int>(5, 9));
    v.push_back(Edge<int>(10, 9));
    v.push_back(Edge<int>(10, 1));
    v.push_back(Edge<int>(9, 1));

    DAG<int> dag(v);
    mu_custom_check(v.empty(), "Vector should be cleared of contents- ownership now belongs to DAG", 1, 2);
    mu_custom_check(!dag.is_empty(), "DAG should not be empty- initialisation should be successful", 2, 2);
MU_CUSTOM_TEST_END


MU_CUSTOM_TEST_START(int_create_valid_20node_DAG)
    std::vector<Edge<int>> v;
    //Creating most unbalanced tree
    for(int i=1; i<20; i++){
        v.push_back(Edge<int>(i, i+1));
    }

    DAG<int> dag(v);
    mu_custom_check(v.empty(), "Vector should be cleared of contents- ownership now belongs to DAG", 1, 2);
    mu_custom_check(!dag.is_empty(), "DAG should not be empty- initialisation should be successful", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(string_create_valid_6node_DAG)
    std::vector<Edge<std::string>> v;
    v.push_back(Edge<std::string>("Hello", "there"));
    v.push_back(Edge<std::string>("Hello", "how are"));
    v.push_back(Edge<std::string>("how are", "you"));
    v.push_back(Edge<std::string>("Hello", "nice to see"));
    v.push_back(Edge<std::string>("nice to see", "you"));
    v.push_back(Edge<std::string>("you", "my friend"));

    DAG<std::string> dag(v);
    mu_custom_check(v.empty(), "Vector should be cleared of contents- ownership now belongs to DAG", 1, 2);
    mu_custom_check(!dag.is_empty(), "DAG should not be empty- initialisation should be successful", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(int_create_invalid_DAG_cycle)
    std::vector<Edge<int>> v1;
    //Cycle between 4->3->5->4->3... disconnected from rest of graph
    v1.push_back(Edge<int>(4, 3));  
    v1.push_back(Edge<int>(3, 5));
    v1.push_back(Edge<int>(5, 4));
    v1.push_back(Edge<int>(10, 11));

    DAG<int> dag1(v1);
    mu_custom_check(!v1.empty(), "Vector should not be cleared of contents as DAG invalid", 1, 10);
    mu_custom_check(dag1.is_empty(), "DAG should be empty as edges contain cycles", 2, 10);

    std::vector<Edge<int>> v2;
    //Cycle between 4->3->4->3... disconnected from rest of graph
    v2.push_back(Edge<int>(4, 3));  
    v2.push_back(Edge<int>(3, 4));
    v2.push_back(Edge<int>(11, 12));
    v2.push_back(Edge<int>(10, 4));
    v2.push_back(Edge<int>(10, 2));

    DAG<int> dag2(v2);
    mu_custom_check(!v2.empty(), "Vector should not be cleared of contents as DAG invalid", 3, 10);
    mu_custom_check(dag2.is_empty(), "DAG should be empty as edges contain cycles", 4, 10);

    std::vector<Edge<int>> v3;
    //Cycle between 4->3-> ... connected to rest of graph by root 10
    v3.push_back(Edge<int>(4, 3));  
    v3.push_back(Edge<int>(3, 4));
    v3.push_back(Edge<int>(11, 12));
    v3.push_back(Edge<int>(10, 11));
    v3.push_back(Edge<int>(10, 4));

    DAG<int> dag3(v3);
    mu_custom_check(!v3.empty(), "Vector should not be cleared of contents as DAG invalid", 5, 10);
    mu_custom_check(dag3.is_empty(), "DAG should be empty as edges contain cycles", 6, 10);

    std::vector<Edge<int>> v4;
    //Cycle between 4->6->1->2->4 ... connected to rest of graph by root 10
    v4.push_back(Edge<int>(6, 1));
    v4.push_back(Edge<int>(1, 2));
    v4.push_back(Edge<int>(4, 6));  
    v4.push_back(Edge<int>(2, 4));
    v4.push_back(Edge<int>(10, 4));
    v4.push_back(Edge<int>(10, 11));

    DAG<int> dag4(v4);
    mu_custom_check(!v4.empty(), "Vector should not be cleared of contents as DAG invalid", 7, 10);
    mu_custom_check(dag4.is_empty(), "DAG should be empty as edges contain cycles", 8, 10);

    std::vector<Edge<int>> v5;
    //Cycle between 1->2->3->4->5->6->7->8->9->1-> ... 
    v5.push_back(Edge<int>(3, 4));
    v5.push_back(Edge<int>(7, 8));
    v5.push_back(Edge<int>(9, 1));
    v5.push_back(Edge<int>(1, 2));
    v5.push_back(Edge<int>(6, 7));
    v5.push_back(Edge<int>(4, 5));  
    v5.push_back(Edge<int>(2, 3));
    v5.push_back(Edge<int>(5, 6));
    v5.push_back(Edge<int>(8, 9));

    DAG<int> dag5(v5);
    mu_custom_check(!v5.empty(), "Vector should not be cleared of contents as DAG invalid", 9, 10);
    mu_custom_check(dag5.is_empty(), "DAG should be empty as edges contain cycles", 10, 10);


MU_CUSTOM_TEST_END

//A loop is an instance of a node pointing to itself
MU_CUSTOM_TEST_START(int_create_invalid_DAG_loop)
    std::vector<Edge<int>> v1;
    //Graph with 1 node, with loop 1->1
    v1.push_back(Edge<int>(1, 1));  

    DAG<int> dag1(v1);
    mu_custom_check(!v1.empty(), "Vector should not be cleared of contents as DAG invalid", 1, 4);
    mu_custom_check(dag1.is_empty(), "DAG should be empty as edges contain a loop", 2, 4);

    std::vector<Edge<int>> v2;
    //Loop with node 2->2 in otherwise valid graph
    v2.push_back(Edge<int>(4, 3));  
    v2.push_back(Edge<int>(3, 11));
    v2.push_back(Edge<int>(11, 12));
    v2.push_back(Edge<int>(2, 2));
    v2.push_back(Edge<int>(10, 2));

    DAG<int> dag2(v2);
    mu_custom_check(!v2.empty(), "Vector should not be cleared of contents as DAG invalid", 3, 4);
    mu_custom_check(dag2.is_empty(), "DAG should be empty as edges contain a loop", 4, 4);
MU_CUSTOM_TEST_END

//Remove Node test
MU_CUSTOM_TEST_START(int_remove_node_root_shared_children)
    std::vector<Edge<int>> v;
    v.push_back(Edge<int>(10, 3));
    v.push_back(Edge<int>(4, 3));   //3 belongs to 4 and 10 - node will not be deleted, but edge will
    v.push_back(Edge<int>(4, 2));   //2 belongs only to 4 - node will be deleted
    v.push_back(Edge<int>(2, 7));   //7 belongs to 2 - node will be deleted
    v.push_back(Edge<int>(1, 10));
    v.push_back(Edge<int>(3, 15));  //15 belongs to 3 - will not be deleted since 3 not deleted

    std::vector<Edge<int>> expected;
    expected.push_back(Edge<int>(10, 3));
    expected.push_back(Edge<int>(1, 10));
    expected.push_back(Edge<int>(3, 15));  //15 belongs to 3 - will not be deleted

    DAG<int> dag(v);
    int count = dag.remove_node(4);
    auto returned_edges = dag.get_edges();

    mu_custom_check(returned_edges.size() ==3, "3 edges in DAG expected after delete node", 1, 5);
    mu_custom_check(count==1, "Since root node removed, count expected to be 1", 2, 5);

    //Checking that each edge is present
    int i=3;
    for(auto edge : expected){
        bool found = false;
        for(auto ret_edge : returned_edges){
            if(ret_edge == edge)found=true;
        }
        mu_custom_check(found, "Edges not deleted should still be present in DAG", i++, 5);
    }
MU_CUSTOM_TEST_END  

MU_CUSTOM_TEST_START(int_remove_node_root_unique_children)
    std::vector<Edge<int>> v;
    v.push_back(Edge<int>(10, 3));  //10 is deleted, 3 is deleted
    v.push_back(Edge<int>(10, 2));  //10 is deleted, 2 is deleted
    v.push_back(Edge<int>(1, 10));  //10 belongs uniquely to 1, will be deleted
    v.push_back(Edge<int>(1, 7));   //7 belongs uniquely to 1, will be deleted

    DAG<int> dag(v);
    int count = dag.remove_node(1);

    mu_custom_check(dag.is_empty(), "DAG expected to be empty, as all nodes end up deleted", 1, 2);
    mu_custom_check(count==1, "Since root node removed, count expected to be 1", 2, 2);

MU_CUSTOM_TEST_END  

MU_CUSTOM_TEST_START(int_remove_node_child_one_parent)
    std::vector<Edge<int>> v;
    v.push_back(Edge<int>(10, 3));  
    v.push_back(Edge<int>(3, 2));   //3 is removed, 2 also removed
    v.push_back(Edge<int>(10, 5));  
    v.push_back(Edge<int>(7, 5));  

    std::vector<Edge<int>> expected;
    expected.push_back(Edge<int>(10, 5));  
    expected.push_back(Edge<int>(7, 5));  

    DAG<int> dag(v);
    int count = dag.remove_node(3);
    auto returned_edges = dag.get_edges();

    mu_custom_check(returned_edges.size() ==2, "2 edges in DAG expected after delete node", 1, 4);
    mu_custom_check(count==1, "3 has one parent, hence count==1", 2, 4);

    //Checking that each edge is present
    int i=3;
    for(auto edge : expected){
        bool found = false;
        for(auto ret_edge : returned_edges){
            if(ret_edge == edge)found=true;
        }
        mu_custom_check(found, "Edges not deleted should still be present in DAG", i++, 4);
    }
MU_CUSTOM_TEST_END  

MU_CUSTOM_TEST_START(int_remove_node_child_two_parents)
    std::vector<Edge<int>> v;
    v.push_back(Edge<int>(10, 3));  
    v.push_back(Edge<int>(3, 2));   //3 removed, 2 removed
    v.push_back(Edge<int>(3, 4));   //3 removed, so edge removed (4 not removed as is owned by 7)
    v.push_back(Edge<int>(2, 4));   //2 removed, so edge removed (4 not removed as is owned by 7)
    v.push_back(Edge<int>(10, 5));  
    v.push_back(Edge<int>(7, 3));  
    v.push_back(Edge<int>(7, 4));  

    std::vector<Edge<int>> expected;
    expected.push_back(Edge<int>(10, 5));  
    expected.push_back(Edge<int>(7, 4));  

    DAG<int> dag(v);
    int count = dag.remove_node(3);
    auto returned_edges = dag.get_edges();

    mu_custom_check(returned_edges.size() ==2, "2 edges in DAG expected after delete node", 1, 4);
    mu_custom_check(count==2, "3 has two parents, hence count==2", 2, 4);

    //Checking that each edge is present
    int i=3;
    for(auto edge : expected){
        bool found = false;
        for(auto ret_edge : returned_edges){
            if(ret_edge == edge)found=true;
        }
        mu_custom_check(found, "Edges not deleted should still be present in DAG", i++, 4);
    }
MU_CUSTOM_TEST_END  

MU_CUSTOM_TEST_START(int_remove_two_nodes)
    std::vector<Edge<int>> v;
    v.push_back(Edge<int>(1, 2));  
    v.push_back(Edge<int>(1, 3));  
    v.push_back(Edge<int>(3, 2));  
    v.push_back(Edge<int>(3, 8));   
    v.push_back(Edge<int>(3, 6));  
    v.push_back(Edge<int>(8, 9));   
    v.push_back(Edge<int>(1, 5));  
    v.push_back(Edge<int>(7, 5));  
    v.push_back(Edge<int>(7, 10));  
    v.push_back(Edge<int>(5, 6));  

    std::vector<Edge<int>> expected_rm3;
    expected_rm3.push_back(Edge<int>(1, 2));  
    expected_rm3.push_back(Edge<int>(1, 5));  
    expected_rm3.push_back(Edge<int>(7, 5));  
    expected_rm3.push_back(Edge<int>(5, 6));  
    expected_rm3.push_back(Edge<int>(7, 10));  

    std::vector<Edge<int>> expected_rm5;
    expected_rm5.push_back(Edge<int>(1, 2));  
    expected_rm5.push_back(Edge<int>(7, 10));  

    DAG<int> dag(v);
    int count = dag.remove_node(3);
    auto returned_edges = dag.get_edges();

    int i=1;
    mu_custom_check(returned_edges.size() ==5, "5 edges in DAG expected after delete node 3", i++, 11);
    mu_custom_check(count==1, "3 had one parent, hence count==1", i++, 11);

    //Checking that each edge is present
    for(auto edge : expected_rm3){
        bool found = false;
        for(auto ret_edge : returned_edges){
            if(ret_edge == edge)found=true;
        }
        mu_custom_check(found, "Edges not deleted should still be present in DAG", i++, 11);
    }

    count = dag.remove_node(5);
    returned_edges = dag.get_edges();

    mu_custom_check(returned_edges.size() ==2, "2 edges in DAG expected after delete node 5", i++, 11);
    mu_custom_check(count==2, "5 had two parent, hence count==2", i++, 11);

    //Checking that each edge is present
    for(auto edge : expected_rm5){
        bool found = false;
        for(auto ret_edge : returned_edges){
            if(ret_edge == edge)found=true;
        }
        mu_custom_check(found, "Edges not deleted should still be present in DAG", i++, 11);
    }
MU_CUSTOM_TEST_END 

MU_CUSTOM_TEST_START(int_remove_node_not_present)
    std::vector<Edge<int>> edges;
    edges.push_back(Edge<int>(4, 3));
    edges.push_back(Edge<int>(4, 5));
    edges.push_back(Edge<int>(5, 9));
    edges.push_back(Edge<int>(10, 9));
    edges.push_back(Edge<int>(10, 1));
    edges.push_back(Edge<int>(9, 1));

    auto edges_copy = edges;

    DAG<int> dag(edges);
    auto returned_edges = dag.get_edges();
    int count = dag.remove_node(20);

    mu_custom_check(count ==0, "Node not present to be removed, so count is 0", 1, 7);

    //Checking that each edge is present
    int i=2;
    for(auto edge : edges_copy){
        bool found = false;
        for(auto ret_edge : returned_edges){
            if(ret_edge == edge)found=true;
        }
        mu_custom_check(found, "Edges should match edges initialised - as no node deleted", i++, 7);
    }
MU_CUSTOM_TEST_END

//Get edges test
MU_CUSTOM_TEST_START(int_get_edges_2node_DAG)
    std::vector<Edge<int>> edges;
    Edge<int> e1(4, 3);
    edges.push_back(e1);

    DAG<int> dag(edges);
    auto returned_edges = dag.get_edges();

    mu_custom_check(returned_edges.size() ==1, "1 edge in DAG created", 1, 2);
    mu_custom_check(*returned_edges.begin() ==e1, "Edges should match edges initialised", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(int_get_edges_6node_DAG)
    std::vector<Edge<int>> edges;
    edges.push_back(Edge<int>(4, 3));
    edges.push_back(Edge<int>(4, 5));
    edges.push_back(Edge<int>(5, 9));
    edges.push_back(Edge<int>(10, 9));
    edges.push_back(Edge<int>(10, 1));
    edges.push_back(Edge<int>(9, 1));

    auto edges_copy = edges;

    DAG<int> dag(edges);
    auto returned_edges = dag.get_edges();

    mu_custom_check(returned_edges.size() ==6, "6 edges in DAG created", 1, 7);

    //Checking that each edge is present
    int i=2;
    for(auto edge : edges_copy){
        bool found = false;
        for(auto ret_edge : returned_edges){
            if(ret_edge == edge)found=true;
        }
        mu_custom_check(found, "Edges should match edges initialised", i++, 7);
    }
MU_CUSTOM_TEST_END


MU_CUSTOM_TEST_START(int_get_edges_20node_DAG)
    std::vector<Edge<int>> edges;
    //Creating most unbalanced tree
    for(int i=1; i<20; i++){
        edges.push_back(Edge<int>(i, i+1));
    }
    auto edges_copy = edges;
    DAG<int> dag(edges);
    auto returned_edges = dag.get_edges();

    mu_custom_check(returned_edges.size() ==19, "19 edges in DAG created", 1, 20);

    //Checking that each edge is present
    int i=2;
    for(auto edge : edges_copy){
        bool found = false;
        for(auto ret_edge : returned_edges){
            if(ret_edge == edge)found=true;
        }
        mu_custom_check(found, "Edges should match edges initialised", i++, 20);
    }
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(string_get_edges_6node_DAG)
    std::vector<Edge<std::string>> v;
    v.push_back(Edge<std::string>("Hello", "there"));
    v.push_back(Edge<std::string>("Hello", "how are"));
    v.push_back(Edge<std::string>("how are", "you"));
    v.push_back(Edge<std::string>("Hello", "nice to see"));
    v.push_back(Edge<std::string>("nice to see", "you"));
    v.push_back(Edge<std::string>("you", "my friend"));
    auto edges_copy = v;

    DAG<std::string> dag(v);
    auto returned_edges = dag.get_edges();

    mu_custom_check(returned_edges.size() ==6, "6 edges in DAG created", 1, 7);
    //Checking that each edge is present
    int i=2;
    for(auto edge : edges_copy){
        bool found = false;
        for(auto ret_edge : returned_edges){
            if(ret_edge == edge)found=true;
        }
        mu_custom_check(found, "Edges should match edges initialised", i++, 7);
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

MU_TEST_SUITE(create_DAG){
	MU_RUN_TEST(int_create_valid_2node_DAG);
	MU_RUN_TEST(int_create_valid_6node_DAG);
	MU_RUN_TEST(int_create_valid_20node_DAG);
    MU_RUN_TEST(int_create_invalid_DAG_cycle);
    MU_RUN_TEST(int_create_invalid_DAG_loop);
    MU_RUN_TEST(string_create_valid_6node_DAG);
}

MU_TEST_SUITE(get_edges){
    MU_RUN_TEST(int_get_edges_2node_DAG);
    MU_RUN_TEST(int_get_edges_6node_DAG);
    MU_RUN_TEST(int_get_edges_20node_DAG);
    MU_RUN_TEST(string_get_edges_6node_DAG);
}

MU_TEST_SUITE(remove_node){
    MU_RUN_TEST(int_remove_node_root_shared_children);
    MU_RUN_TEST(int_remove_node_root_unique_children);
    MU_RUN_TEST(int_remove_node_child_one_parent);
    MU_RUN_TEST(int_remove_node_child_two_parents);
    MU_RUN_TEST(int_remove_two_nodes);
    MU_RUN_TEST(int_remove_node_not_present);
}

int main(){
    //NODE tests
    MU_RUN_SUITE(create_node);
    MU_RUN_SUITE(get_data);
    MU_RUN_SUITE(add_contains_parent);
    MU_RUN_SUITE(add_contains_child);

    //DAG tests
    MU_RUN_SUITE(create_DAG);
    MU_RUN_SUITE(remove_node);
    MU_RUN_SUITE(get_edges);

    MU_REPORT();
    return MU_EXIT_CODE;
    return 0;
}