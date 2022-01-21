#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "node.hpp"

// An template of a directed, acyclic graph which nodes all of
// uniform type T. Each value of T must be unique, and the < operator 
// must be defined on T (for std::set to work)
template <class T>
class DAG{
    private:   
        //Stores all nodes in DAG which are not pointed to by any other node
        //has to be shared for child to contain weak_ptr reference to root
        std::set<std::shared_ptr<Node<T>>> roots;

        /* Method used to initialise DAG recursively
           Should only be accessed internally (by DAG object)
        */
        bool init_node(std::shared_ptr<Node<T>> node, const std::vector<Edge<T>> &edges, 
                       std::map<T, std::shared_ptr<Node<T>>> nodes,
                       std::map<std::shared_ptr<Node<T>>, bool> &visited,
                       std::map<std::shared_ptr<Node<T>>, bool> &initialised){

            //Node already initialised and no cycles found in the subgraph
            //Hence subgraph is valid (and already initialised) so does not need to be traversed
            if(initialised[node]){
                return true;
            }

            // Case when node already visited - means that there is a cycle
            if(visited[node]){
                return false;
            }
            // Sets visited to true before initialising children
            // in case children eventually point to node
            visited[node] = true;

            //Initialise all children here
            for(auto edge : edges){
                if(edge.get_parent() ==node->get_data()){
                    //Finding shared_ptr to child in mapping
                    std::shared_ptr<Node<T>> child = nodes[edge.get_child()];

                    //Trying to initialise child before taking ownership
                    //Otherwise there may arise cyclic ownership (memory leak)
                    bool valid = init_node(child, edges, nodes, visited, initialised);

                    if(!valid)return false;

                    //Gives parent ownership of child
                    node->add_child(child);
                    //Stores (weak) reference of parent in child
                    child->add_parent(node);
                }
            }

            // Sets visited as false as program is no longer traversing node's children
            visited[node] = false;
            // Node has already been initialised so does not need to be traversed again
            initialised[node] = true;
            return true;
        }

    public:
        //Default constructor - creates empty DAG
        DAG(){}

        /* Constructor of DAG
        Takes in a list (vector) of directed edges and if valid creates the DAG corresponding
        to these edges. 
        If invalid (ie there are cycles) DAG is initialised with roots being empty. 
        */
        DAG(std::vector<Edge<T>> &edges){
            //Checking the graph contains no loops (node pointing to itself)
            for(auto edge : edges){
                if(edge.get_parent()==edge.get_child())return;
            }

            //Finding unique values of root nodes
            std::set<T> root_values;
            for(auto edge : edges){
                root_values.insert(edge.get_parent());
            }
            for(auto edge : edges){
                root_values.erase(edge.get_child());
            }

            //If DAG has no roots then graph is cyclic (or empty) ie edges are invalid
            if(root_values.empty()){
                return;
            }

            //Finding all unique non root values
            std::set<T> child_values;
            for(auto edge : edges){
                child_values.insert(edge.get_parent());
                child_values.insert(edge.get_child());
            }
            for(auto root : root_values){
                child_values.erase(root);
            }

            std::map<std::shared_ptr<Node<T>>, bool> visited;
            std::map<std::shared_ptr<Node<T>>, bool> initialised;

            //Creating root nodes and storing in object variable roots
            for(auto root_val : root_values){
                auto root = std::make_shared<Node<T>>(root_val);
                roots.insert(root);

                //Marking node as not visited, not initialised
                visited.insert(std::pair<std::shared_ptr<Node<T>>, bool>(root, false));
                initialised.insert(std::pair<std::shared_ptr<Node<T>>, bool>(root, false));
            }
            
            //Creating non-root nodes and storing in mapping children
            std::map<T, std::shared_ptr<Node<T>>> children;
            for(auto child_val : child_values){
                auto child = std::make_shared<Node<T>>(child_val);
                children.insert(std::pair<T, std::shared_ptr<Node<T>>>(child_val, child));

                //Marking node as not visited, not initialised
                visited.insert(std::pair<std::shared_ptr<Node<T>>, bool>(child, false));
                initialised.insert(std::pair<std::shared_ptr<Node<T>>, bool>(child, false));
            }

            bool has_cycles=false;
            for(auto root : roots){
                if(!init_node(root, edges, children, visited, initialised)){   
                    //case when init_node fails ie there is a cycle in edges
                    has_cycles=true;
                    break;  //no point in initialising rest of graph
                }
            }

            //Checks that each node has been initialised
            //if not, there exists a loop unaccessible by roots
            for(auto const& pair : initialised){
                if(!pair.second)has_cycles=true;
            }

            if(has_cycles){
                //deletes roots -- should remove all ownership of nodes
                roots.clear();  
            }else{
                //DAG takes FULL ownership of nodes
                //hence ownership of vector edges must be removed
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

        bool is_empty(){
            return roots.empty();
        }

        /* Method to remove a node from DAG
           All children pointed to by said node will become unallocated unless
           another node is pointing to them.
           If node found, function returns number of times it is references 
           (ie. number of parents it had, or 1 in case of root nodes).
           If node not found, function returns 0.
        */
        int remove_node(T data){
            auto match = [&data](auto const& x) { return (x->get_data()) == data; };
            auto count = std::erase_if(roots, match);

            // If node is a root it is only reference once
            // hence it makes sense to return without checking rest of graph
            if(count >0)return count;

            for(auto root : roots){
                count += root->remove_node(data);
            }
            return count;
        }

        /* Method to get edges of a DAG
           Calls Node.get_edges() recursively in order to traverse DAG
        */
        std::vector<Edge<T>> get_edges() const{
            std::set<std::shared_ptr<Node<T>>> visited_nodes;
            std::vector<Edge<T>> edges;

            for(auto root : roots){
                //No need to add root to visited_nodes, since graph acyclic
                root->get_edges(visited_nodes, edges);
            }

            return edges;
        }

        

};



