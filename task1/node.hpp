#pragma once


//Defining a node
template <class T>
class Node{
    private:
        T data; //data stored in node
        vector<shared_ptr<Node>> children;  //node (partly) owns all of its children
        vector<weak_ptr<Node>> parents;     //node contains weak reference to its parents

    public:
        Node(T);
        ~Node();
        add_child(Node);
        add_parent(Node);
};

//Constructor - creates a node with data specified in parameters
template<class T>
Node<T>::Node(T data){
    this.data = data;
}

//Destructor - should set free all shared pointers to children before deleting node
template<class T>
Node<T>::~Node(){
    //Goes through all shared_ptrs (pointing to children of Node)
    for(auto child::children){
        // removes ownage of Node to its child
        child.reset();  
    }
}

template<class T>
Node<T>::add_child(Node node){

}

template<class T>
Node<T>::add_parent(Node node){

}

//Edge class
class Edge{
    private:
        void *parent;
        void *child;
    public:
        Edge(void *parent_data, void *child_data);
};