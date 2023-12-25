#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include "Graph.hpp"

void display_menu()
{
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "1. Choose a city" << std::endl;
    std::cout << "2. Chosen city" << std::endl;
    std::cout << "3. Nearest K city (max 79)" << std::endl;
    std::cout << "4. Shortest path between two cities" << std::endl << "5. Number of cities" << std::endl;
    std::cout << "6. Number of edges" << std::endl;
    std::cout << "7. Get edge weight between two cities" << std::endl;
    std::cout << "8. Set edge weight between two cities" << std::endl;
    std::cout << "9. Exit" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

void check(int &weight, std::string &src, std::string &dst)
{
  if(weight == 0)
  {
    std::cout << "There is no direct edge between: " << src << " <--> " << dst << std::endl;
  }
  else
  {
    std::cout << "Edge weight between: " << src << " <--> " << dst << " is: "<< weight << std::endl;
  }
}
void start_program(Graph &graph)
{
  int choice {0};
  std::string src = "";

  while(choice != 9)
  {
    std::string dst = "";
    int weight;
    int k;

    display_menu();

    std::cout << "Choice: ";
    std::cin >> choice;
    if(choice == 1)
    {
      std::cout << "Choose your city: ";
      std::cin >> src;
    }
    else if(choice == 2)
    {
      std::cout << "Chosen city: ";
      std::cout << src << std::endl;
    }
    else if(choice == 3)
    {
      std::cout << "Enter K (max 79): ";
      std::cin >> k; 
      graph.k_nearest(src,k);
    }
    else if(choice == 4)
    {
      if(src == "")
      {
        std::cout << "Start: ";
        std::cin >> src;
      }
      std::cout << "End: ";
      std::cin >> dst;
      graph.UFS(src,dst);
    }
    else if(choice == 5)
    {
      std::cout << graph.verticies() << std::endl;
    }
    else if(choice == 6)
    {
      std::cout << graph.edges() << std::endl;
    }
    else if(choice == 7)
    {
      if(src == "")
      {
        std::cout << "Start: ";
        std::cin >> src;
      }
      std::cout << "End: ";
      std::cin >> dst;
      int weight = graph.get_edge_value(src,dst);

      check(weight,src,dst);
    }
    else if(choice == 8)
    {
      if(src == "")
      {
        std::cout << "Start: ";
        std::cin >> src;
      }

      std::cout << "End: ";
      std::cin >> dst;

      std::cout << "New weight: ";
      std::cin >> weight;

      graph.set_edge_value(src,dst,weight);
    }
  }
}
int main()
{
  std::string f_name = "adjacent_cities.txt";
  std::string distance_name = "CityDistances.txt";
  Graph graph(f_name,distance_name);  
  start_program(graph);
  std::cout << "Bye Bye!";


  return 0;
}