#pragma once
#include <vector>
#include <set>
#include <memory>
#include <iostream>

// Defining Edge class - used to construct the DAG
template<class T>
class Edge{
    private:
        T parent;
        T child;
    public:
        Edge(T parent, T child){
            this->parent = parent;
            this->child = child;
        }

        T get_parent() const{
            return parent;
        }
        
        T get_child() const{
            return child;
        }

        bool operator== (const Edge &rhs) const{
            return (parent==rhs.get_parent() && child==rhs.get_child());
        }

        bool operator!=(const Edge &rhs) const {
            return !(*this == rhs);
        }
};


//Node class - defines a template of a node
//Stores a single node in the graph along with any data it contains
//T is not assumed to be unique

/*  Notes:
    No enforcement specific in this class that if a node n1 is a child of n2, then n2 is a parent
    of n1... This detail needs to be enforced in the graph itself.
    This is done to avoid this to shared_pointer conversion
*/
template <class T>
class Node{
    private:   
        std::set<std::shared_ptr<Node<T>>> children;  //node (partly) owns all of its children
        std::vector<std::weak_ptr<Node<T>>> parents;     //node contains weak reference to its parents
        T data; //data stored in node
    public:
        //Default Constrctor - creates a node with empty root set
        Node(){}

        //Constructor - creates a node with data specified in parameters
        Node(T data){
            this->data = data;
        }

        //Destructor - should set free all shared pointers to children before deleting node
        ~Node(){
            for(auto child_ptr : children){
                child_ptr.reset();
            }
        }

        //Method to get data of type T from an individual node
        T get_data() const{
            return data;
        }

        //Method to set data of type T in node
        void set_data(const T data){
            this->data = data;
        }

        //Method to add parent
        void add_parent(std::shared_ptr<Node<T>> parent){
            //Creating a new weak pointer holding reference to parent
            std::weak_ptr<Node<T>> parent_pointer = parent;
            parents.push_back(parent_pointer);
        }

        //Method to add child
        void add_child(std::shared_ptr<Node<T>> child){
            children.insert(child);
        }

        //Returns true if parameter node is a parent, false otherwise
        bool contains_parent(const std::shared_ptr<Node<T>> parent) const{
            for(auto parent_iter : parents){
                //Gets reference from weak_ptr to compare to parent parameter
                auto shared_rep = parent_iter.lock();
                //Checking if pointer matches and is valid (shared_rep will be false if say parent deleted)
                if(shared_rep == parent && shared_rep)return true;
            }
            return false;
        }
        
        //Returns true if parameter node is a child, false otherwise
        bool contains_child(const std::shared_ptr<Node<T>> child) const{
            return children.contains(child);
        }

        int remove_node(T data){
            auto match = [&data](auto const& x) { return (x->get_data()) == data; };
            auto count = std::erase_if(children, match);

            //Traverses children to delete the node if present
            for(auto child : children){
                count += child->remove_node(data);
            }
            return count;
        }

        void get_edges(std::set<std::shared_ptr<Node<T>>> &visited_nodes, std::vector<Edge<T>> &edges) const{
            for(auto child : children){
                Edge<T> e(data, child->get_data());
                edges.push_back(e);

                //If node has not been visited before must be traversed to get edges
                if(visited_nodes.find(child) == visited_nodes.end()){
                    child->get_edges(visited_nodes, edges);
                    visited_nodes.insert(child);
                }
            }
        }
};


