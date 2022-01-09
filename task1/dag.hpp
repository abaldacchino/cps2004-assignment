#pragma once
#include <string>
#include <vector>
#include <memory>
#include "node.hpp"

// An template of a directed, acyclic graph which nodes all of
// uniform type T
template <class T>
class DAG{
    private:   
        //Stores all nodes in DAG which are not pointed to by any other node
        std::set<std::shared_ptr<Node<T>>> roots;

        /* Method used to initialise DAG recursively
           Should only be accessed internally (by DAG object)
        */
        bool init_node(std::shared_ptr<Node<T>> node, std::vector<Edge<T>> edges){
            //Node already initialised and no cycles found in the subgraph
            //Hence subgraph is valid (and already initialised) so does not need to be traversed
            if(node->get_init()){
                return true;
            }

            // Case when node already visited - means that there is a cycle
            if(node->get_visited()){
                return false;
            }
            // Sets visited to true before initialising children
            // in case children eventually point to node
            node->set_visited(true);

            //Traverse all children here
            for(edge : edges){
                if(edge.get_parent() ==node){
                    //Trying to initialise child before taking ownership
                    //Otherwise there may arise cyclic ownership (memory leak)
                    bool valid = init_node(edge.get_child(), edges);

                    if(!valid)return false;

                    //Gives parent ownership of child
                    edge.get_parent()->add_child(edge.get_child());
                    //Stores (weak) reference of parent in child
                    edge.get_child()->add_parent(edge.get_parent());
                }
            }

            // Sets visited as false as program is no longer traversing node's children
            node->set_visited(false);
            // Node has already been initialised so does not need to be traversed again
            node->set_init(true);
        }

    public:
        //Default constructor - creates empty DAG
        DAG(){}

        /* Constructor of DAG
        Takes in a list (vector) of directed edges and if valid creates the DAG corresponding
        to these edges. Ownership is transferred from list to DAG.
        If invalid (ie there are cycles) DAG is initialised with roots being empty
        Ownership remains list.
        */
        DAG(std::vector<Edge<T>> edges){
            //Finding all root nodes - storing in roots
            roots = find_roots(edges);

            //If DAG has no roots then graph is cyclic (or empty) ie edges are invalid
            if(roots.empty()){
                return;
            }

            bool has_cycles=false;
            for(auto root : roots){
                if(!init_node(root)){   
                    //case when init_node fails ie there is a cycle in edges
                    has_cycles=true;
                    break;  //no point in initialising rest of graph
                }
            }

            if(has_cycles){
                //deletes roots -- should remove all ownership of nodes
                roots.clear();  
            }else{
                //otherwise graph has full ownership of nodes
                //ownership of list of edges is removed.
                edges.clear();
            }

        }

        /* Destructor of DAG
        Deletes all the roots of the graph, which should call their destructors,
        in turn freeing the memory of all the DAG
        */
        ~DAG(){
            //calls destructor of the roots - recursively freeing all allocated nodes
            roots.clear();  
        }

        //Method finds and returns all root notes (ie nodes with no parents) when given a list of edges
        std::set<std::shared_ptr<Node<T>>> find_roots(std::vector<Edge<T>> edges){
            std::set<std::shared_ptr<Node<T>>> roots_set;
            for(auto edge : edges){
                roots_set.insert(edge.get_parent());
            }

            //Removing all nodes not roots - ie exist as a child in an edge
            for(auto edge : edges){
                roots_set.erase(edge.get_child());
            }

            return roots_set;
        }

        

        bool delete_node_with_ptr(std::shared_ptr<Node<T>> node){
            std::shared_ptr<Node<T>> found_node = roots.find(node);

            if(found_node != roots.end()){
                roots.erase(found_node);
                return true;
            }

            for(auto edge : edges){

            }
        }

        bool delete_node_with_value(T data){
            for(auto edge : edges){
                
            }
        }



};



