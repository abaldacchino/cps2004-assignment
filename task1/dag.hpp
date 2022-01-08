#pragma once
#include <string>
#include <vector>
#include <memory>
#include "node.hpp"


// Defining Edge class - used to construct the DAG
template<class T>
class Edge{
    private:
        //Pointers are shared as a different edge may also have ownership of parent/child
        std::shared_ptr<Node<T>> parent;
        std::shared_ptr<Node<T>> child;
    public:
        Edge(std::shared_ptr<Node<T>>, std::shared_ptr<Node<T>>){
            this->parent = parent;
            this->child = child;
        }
};

// An template of a directed, acyclic graph which nodes all of
// uniform type T
template <class T>
class DAG{
    private:   
        //Stores all nodes in DAG which are not pointed to by any other node
        std::set<std::unique_ptr<Node<T>>> roots;
    public:
        DAG(std::vector<Edge<T>>);
        ~DAG();
};

/* Destructor of DAG
   Deletes all the roots of the graph, which should call their destructors,
   in turn freeing the memory of all the DAG
*/
template<class T>
DAG<T>::~DAG(){
    for(auto root : roots){
        root.reset();  
    }
}

/* Constructor of DAG
   Takes in a list (vector) of directed edges and if valid creates the DAG corresponding
   to these edges. Ownership is transferred from list to DAG.
   If invalid (ie there are cycles) DAG is initialised with roots as null.
   Ownership remains list.
*/
template<class T>
DAG<T>::DAG(std::vector<Edge<T>> edges){
    //Finding all root nodes - storing in a set
    std::set<std::shared_ptr<Node<T>>> roots_set;
    for(auto edge : edges){
        roots_set.insert(edge.parent);
    }
}
