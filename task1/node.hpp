#pragma once
#include <vector>
#include <unordered_set>
#include <set>
#include <memory>

//Node class - defines a template of a node
//Stores a single node in the graph along with any data it contains
template <class T>
class Node{
    private:   
        std::set<std::shared_ptr<Node<T>>> children;  //node (partly) owns all of its children
        std::set<std::weak_ptr<Node<T>>> parents;     //node contains weak reference to its parents
        T data; //data stored in node
    public:
        Node(T);
        ~Node();
        T get_data();
        void set_data(T);
        void add_parent(std::shared_ptr<Node<T>>);
        void add_child(std::shared_ptr<Node<T>>);
        
};

//Constructor - creates a node with data specified in parameters
template<class T>
Node<T>::Node(T data){
    this->data = data;
}

//Destructor - should set free all shared pointers to children before deleting node
template<class T>
Node<T>::~Node(){
    // Goes through all shared_ptrs (pointing to children of Node)
    // Using iterator pattern
    //for (auto itr = children.begin(); itr != children.end(); ++itr) {
        
    //}
}

//Method to get data of type T from an individual node
template<class T>
T Node<T>::get_data(){
    return data;
}

//Method to set data of type T in node
template<class T>
void Node<T>::set_data(T data){
    this->data = data;
}

//Method to add parent
template<class T>
void Node<T>::add_parent(std::shared_ptr<Node<T>> parent){
    //Creating a new weak pointer holding reference to parent
    std::weak_ptr<Node<T>> parent_pointer = parent;
    parents.insert(parent);
}

//Method to add child
template<class T>
void Node<T>::add_child(std::shared_ptr<Node<T>> child){
    //Creating a new shared pointer pointing to the child
    std::shared_ptr<Node<T>> child_pointer = child;
    children.insert(child);
}

// Edge class
template<class T>
class Edge{
    private:
        //Pointers are shared as a different edge may also have ownership of parent/child
        std::shared_ptr<Node<T>> parent;
        std::shared_ptr<Node<T>> child;
    public:
        Edge(std::shared_ptr<Node<T>>, std::shared_ptr<Node<T>>);
};

// Edge constructor
template<class T>
Edge<T>::Edge(std::shared_ptr<Node<T>> parent, std::shared_ptr<Node<T>> child){
    this->parent = parent;
    this->child = child;
}