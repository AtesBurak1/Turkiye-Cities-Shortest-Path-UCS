#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include "enum.hpp"

#define CITIES 81
#define prio_queue std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> 

class Graph{
  private:
    int V; // number of vertices
    int E; // number of edges
    std::map <std::string, std::vector <std::string>> adj; // choice of adjacency lists (linked list, matrix or hash table or whatever you consider more suitable
    std::map <int, std::map <int,int>> distances;
    std::vector <std::string> create_ucs_path(std::map<std::string, std::string>&, std::string, std::string);
    void read_adj_file(std::string &);
    void read_dist_file(std::string &);

    void print_path(std::vector <std::string>&);
    void print_k_city(std::vector <std::pair<std::string, int>>&);
    void add_to_queue(std::string&, std::map<std::string, int>&, std::map<std::string, std::string>&, prio_queue &); // add queue for shortest path
    void add_to_queue(std::string&, std::map<std::string, int>&, prio_queue &); // add queue for k nearest
    std::vector <std::string> uniform_cost_search(std::string, std::string,int &, int*);
    std::vector <std::pair<std::string, int>> k_nearest_search(std::string, int*, int, int);

  public:
    Graph(std::string, std::string); // create a V-vertex graph with no edges
     // read a graph from input stream in if necessary
    std::vector <std::string> neighbors(std::string x); //lists/vector or array of all vertices y such that there is an edge from the vertex x to the vertex y;
    int verticies() ;//number of vertices
    int edges(); //number of edges
    int get_edge_value(std::string, std::string); // returns the value associated with the edge (x, y);
    void set_edge_value(std::string, std::string, int); // sets the value associated with the edge (x, y);
    void UCS(std::string, std::string);
    void k_nearest(std::string,int);

};

#endif