#include "Graph.hpp"

void Graph::read_adj_file(std::string &adj_name)
{
  std::ifstream infile(adj_name);
  std::string line;

  while (std::getline(infile, line))
  {
    std::vector<std::string> vertices;
    size_t pos = 0;
    std::string token;
    std::string initialCity;
    bool initalCityFlag = true;
    
    while ((pos = line.find(',')) != std::string::npos)
    {
      token = line.substr(0, pos);
      if(initalCityFlag)
      {
        initialCity = token;
        initalCityFlag = false;
      }
      else
      {
        vertices.push_back(token);
      }
      line.erase(0, pos + 1);
    }

    vertices.push_back(line); // Adding the last vertex without erasing
    if (!vertices.empty()) 
    {
      this->adj[initialCity] = vertices;
    }
  }
}

void Graph::read_dist_file(std::string &distance_name)
{
  std::ifstream infile_distance(distance_name);
  std::string line_distance;
  int row = 0;
  while (std::getline(infile_distance, line_distance)) 
  {
    std::istringstream iss(line_distance);
    std::string value;
    size_t col = 0;
    size_t skips = 0;
    while (std::getline(iss, value, ';')) 
    {
      if(skips < 2)
      { // skip the first 2 columns in every line
        skips++;
        col++;
        continue;
      }
      // Convert the string distance value to integer
      if(find(this->adj[num_to_city(row)].begin(), this->adj[num_to_city(row)].end(), num_to_city(col - 2)) != this->adj[num_to_city(row)].end())
      {
        this->distances[row][col-2] = std::stoi(value);
      }
      col++;
    }
    row++;
  }
}

Graph::Graph(std::string txt_name, std::string distance_name)
{
  this->read_adj_file(txt_name);
  this->read_dist_file(distance_name);
}

std::vector <std::string> Graph::neighbors(std::string vertex)
{
  return this->adj[vertex];
}

int Graph::get_edge_value(std::string vertex_src, std::string vertex_dst)
{
  
  auto it = find(this->adj[vertex_src].begin(), this->adj[vertex_src].end(), vertex_dst);
  if(it != adj[vertex_src].end()) // if an edge is found between cities
    return this->distances[ getCityEnum(vertex_src) ][ getCityEnum(vertex_dst) ];
  else
    return 0;
}

void Graph::set_edge_value(std::string vertex_src, std::string vertex_dst, int weight)
{
  auto it = find(this->adj[vertex_src].begin(), this->adj[vertex_src].end(), vertex_dst);

  if(it != adj[vertex_src].end()) // if an edge is found between cities
  {
    this->distances[ getCityEnum(vertex_src) ][ getCityEnum(vertex_dst) ] = weight;
    // do not forget that edges are double sided!
    this->distances[ getCityEnum(vertex_dst) ][ getCityEnum(vertex_src) ] = weight;
    std::cout << "Successfullys rearranged! "<< vertex_src << "<-->" << vertex_dst << "= " << weight << std::endl;

  }
  else
  {
    std::cout << "There is no direct edge between these two cities!" << std::endl;
  }
}

int Graph::verticies()
{
  return CITIES; 
}

int Graph::edges()
{
  int sum {0};
  for(auto city : this->adj)
  {
    sum = sum + city.second.size();
  }
  // divide by two edges are double sided!
  sum = sum / 2;
  return sum;
}

// For debugging
void print_pq(    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq)
{
  while (!pq.empty()) { // print current queue for debug
        int currentDistance = pq.top().first;
        std::string c = pq.top().second;
        std::cout << currentDistance << " " << c << std::endl;
        pq.pop();
  }
  std::cout << std::endl;

}
// UCS functions
void Graph::add_to_queue(std::string &currentCity, std::map <std::string, int> &dist, std::map <std::string, std::string> &prev, prio_queue &pq)
{
  for (const auto& neighbor : this->adj[currentCity]) 
  { // visit every neighbor of current city
    int weight = this->distances[ getCityEnum(currentCity) ][ getCityEnum(neighbor) ] + dist[currentCity];
    if (weight < dist[neighbor])
    { 
      //If current cost is lower than before update the prev list, and distance
      prev[neighbor] = currentCity;
      dist[neighbor] = weight;
      pq.push({weight, neighbor});
    }
  }
}

std::vector <std::string> Graph::create_ucs_path(std::map < std::string, std::string> &prev, std::string dst, std::string src)
{
  // shortest path found !
  std::vector<std::string> path;
  std::string current = dst;
  while (current != src) 
  {
    path.push_back(current);
    current = prev[current];
  }
  path.push_back(src);
  reverse(path.begin(), path.end()); // reverse the path because of previous list
  return path;
}

void Graph::print_path(std::vector <std::string> &path)
{
  if(path.size() > 0)
  {    
    std::cout << "Path : ";
    // print the path
    for (size_t i {0}; i < path.size() ; i++)
    {
      if(i < path.size()-1)
        std::cout << path[i] << "->";
      else
        std::cout << path[i] << std::endl;
    }
  }
  else
  {
    std::cout << "No path is found check Start/End city name it should start with camel case!" << std::endl;
  }
}

void Graph::UCS(std::string src, std::string dst)
{
  int cost {0};
  int visited[81] = {0}; // initilaze visited list
  std::vector <std::string> path = this->uniform_cost_search(src, dst, cost, visited);
  this->print_path(path);
  std::cout << "Total Cost: " << cost << std::endl; 

}

std::vector< std::string> Graph::uniform_cost_search(std::string src, std::string dst, int &cost, int *visited) {

  std::map<std::string, int> dist; // To store distances
  std::map<std::string, std::string> prev; // To store previous node in the shortest path
  // Priority queue for (distance, city) query (low distance will be in front of the queue)
  prio_queue pq;

  //Set initial distance to max (for find shortest one)
  for (const auto& node : this->adj)
  {
    dist[node.first] = 9999999;
  }

  dist[src] = 0;
  pq.push({0, src}); // inital state

  while (!pq.empty()) 
  {   
    // loop until queue is empty
    std::string currentCity;
    int currentDistance = pq.top().first; // current distance
    currentCity = pq.top().second; // current city
    
    pq.pop(); // visited status
    visited[getCityEnum(currentCity)] = 1; // mark as visited

    if (currentCity == dst) 
    { // shortest path found !
      std::vector<std::string> path = this->create_ucs_path(prev, dst, src);
      cost = dist[currentCity]; // total cost
      return path;
    }

    if (currentDistance > dist[currentCity]) 
    {
      continue; // already founded shorter path to this city
    }
    this->add_to_queue(currentCity, dist, prev, pq);

  }
  return {}; // path is not find 
}

// k-nearest functions
void Graph::add_to_queue(std::string &currentCity, std::map <std::string, int> &dist, prio_queue &pq)
{
  for (const auto& neighbor : adj[currentCity]) 
  { // visit every neighbor of current city
    int weight = this->distances[ getCityEnum(currentCity) ][ getCityEnum(neighbor) ] + dist[currentCity];
    if (weight < dist[neighbor])
    { 
      dist[neighbor] = weight;
      pq.push({weight, neighbor});
    }
  }
}

void Graph::print_k_city(std::vector <std::pair<std::string, int>> &visited_cities)
{
  if(visited_cities.size() > 0)
  {  
    std::cout << "Cities : ";
    // print the path
    for (size_t i {0}; i < visited_cities.size() ; i++)
    {
        std::cout << visited_cities[i].first << "("<< visited_cities[i].second  << ")" << " ";
    }
    std::cout << std::endl;
  }
  else
  {
    std::cout << "No city is found check Start/End city name it should start with camel case!" << std::endl;
  }
}

void Graph::k_nearest(std::string src, int k)
{  
  if(k <= 79)
  {
    int count {0};
    int visited[CITIES] = {0}; // initilaze visited list
    std::vector <std::pair<std::string, int>> visited_cities = this->k_nearest_search(src, visited, count, k);
    this->print_k_city(visited_cities);
  }
  else
  {
    std::cout << "Entered number is false or greated than 79!" << std::endl;
  }
}

std::vector <std::pair<std::string, int>> Graph::k_nearest_search(std::string src, int *visited, int count, int k)
{
  std::map<std::string, int> dist; // To store distances
  std::vector <std::pair<std::string, int>> visited_cities;
  // Priority queue for (distance, city) query (low distance will be in front of the queue)
  std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq;
  //Set initial distance to max 
  for (const auto& node : this->adj)
  {
      dist[node.first] = 9999999;
  }
  dist[src] = 0;
  pq.push({0, src}); // inital state
  while (!pq.empty()) 
  {   // loop until queue is empty
    //print_pq(pq); debug
    std::string currentCity;
    int currentDistance = pq.top().first; // current distance
    currentCity = pq.top().second; // current city
    
    pq.pop(); 
    if (currentDistance > dist[currentCity])
    {
      continue; // already founded shorter path to this city
    }

    if(currentCity != src && visited[getCityEnum(currentCity)] == 0)
    { 
      visited[getCityEnum(currentCity)] = 1; // mark as visited
      std::pair pr = std::make_pair(currentCity, currentDistance);
      visited_cities.push_back(pr);
      count++; // count the cities when we pop!
    }

    if (count == k)
    { // shortest path found !
      return visited_cities;
    }
    this->add_to_queue(currentCity, dist, pq);
  }
  return {}; // no cities 

}