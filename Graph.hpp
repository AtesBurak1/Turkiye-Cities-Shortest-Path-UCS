#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "enum.hpp"
#define CITIES 81

class Graph{
  public:
    Graph(std::string, std::string); // create a V-vertex graph with no edges
     // read a graph from input stream in if necessary
    std::vector <std::string> neighbors(std::string x); //lists/vector or array of all vertices y such that there is an edge from the vertex x to the vertex y;
    int verticies() ;//number of vertices
    int edges(); //number of edges
    int get_edge_value(std::string, std::string); // returns the value associated with the edge (x, y);
    void set_edge_value(std::string, std::string, int); // sets the value associated with the edge (x, y);
    void UFS(std::string, std::string);
    void k_nearest(std::string,int);
  private:
    int V; // number of vertices
    int E; // number of edges
    std::map <std::string, std::vector <std::string>> adj; // choice of adjacency lists (linked list, matrix or hash table or whatever you consider more suitable
    int distance_matrix[81][81];
    void print_path(std::vector <std::string>&);
    void print_k_city(std::vector <std::string>&);
    std::vector <std::string> uniform_cost_search(std::string, std::string,int &, int*);
    std::vector <std::string> k_nearest_search(std::string, int*, int, int);
    

};





















#endif