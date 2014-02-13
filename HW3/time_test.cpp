#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <queue>
#include <functional>
#include <ctime>
#include <cstdlib>

using namespace std;

class graph
{
  // operator<< overloading of the graph
  friend ostream& operator<<(ostream &out, const graph &input){
    out << "graph size is " << input.size << endl;
    for(int i = 0 ; i < input.size; ++i){
      out << "\n";
	for(auto elemIt = input.adjList[i].begin(); elemIt != input.adjList[i].end(); ++elemIt){
	  out << "Node " << i << " has the distance of " << elemIt->distance << " to Node " << elemIt->vertex << endl;
	}
    }
    return out;
  }

public:

  // constructor for adjacent list representation
  graph(int inputSize,int lowerbound, int upperbound, float density): size(inputSize){

    // random generator for density
    default_random_engine generator;
    uniform_real_distribution<double> probability(0.0, 1.0);
    auto probNumbers = bind(probability, generator);

    // random generator for distance
    default_random_engine distGenerator;
    uniform_int_distribution<int> distance(lowerbound, upperbound);
    auto distNumbers = bind(distance, distGenerator);

    adjList.resize(size);

    for(int i = 0; i < size; ++i){
      for(int j = 0 ; j < size; ++j){

	// check if if it is self loop or the distance between i and j is already defined
	if(i != j && probNumbers() < density && getDistance(i, j) == numeric_limits<int>::max())
	    {
	      Node temp;
	      temp.vertex = j;
	      temp.distance = distNumbers();
	      adjList[i].push_back(temp);
	      
	      // since it is undirected graph, will add extra edge accordingly
	      temp.vertex = i;
	      adjList[j].push_back(temp);
	    }
      }
    }
  }
    
  void DijkstraSearch(int s, int t){
    priority_queue<Node,vector<Node>,comparator> minHeap;
    Node temp;
    int nodeId;

    // vector to store the distance from the source
    // default to be a large number    
    vector<int> shortDistance(size, (numeric_limits<int>::max())/2);
   
    // vector to store show if the node is visited
    vector<bool> visitedNode(size, false);
    
    // start from the source
    temp.vertex = s;
    temp.distance = 0;
    minHeap.push(temp);
    shortDistance[s] = 0;
    
    for(int i = 0; i < size; ++i){
      
      // find an unvisited vertex with minimum distance each time
      do{
	// if all vertex are visited return the distance at t
	if(minHeap.empty()){
	   shortestD = shortDistance[t];
	   return;
	}
	temp = minHeap.top();
	minHeap.pop();
	nodeId = temp.vertex;
      }while(visitedNode[nodeId] == true);
      
      visitedNode[nodeId] = true;

      // when the target node is reached return the distance
      if(nodeId == t){
	shortestD =  shortDistance[t];
	return;
      }
      // update the minimum distance of the successor node    
      for(auto iter = adjList[nodeId].begin(); iter != adjList[nodeId].end(); ++iter){
	if(shortDistance[iter->vertex]  > shortDistance[nodeId] + getDistance(nodeId, iter->vertex)){
	  shortDistance[iter->vertex] = shortDistance[nodeId] + getDistance(nodeId,iter->vertex);
	  temp.distance = shortDistance[iter->vertex];
	  temp.vertex = iter->vertex;
	  minHeap.push(temp);
	}
      }

    }
  }

  void biDirection(int s, int t){
    
    // minHeap to store the forward direction
    priority_queue<Node,vector<Node>,comparator> minHeapF;

    // minHeap to store the backward direction
    priority_queue<Node,vector<Node>, comparator> minHeapB;

    // signal to check if the bi-direction intersects
    int stop = 0;    

    Node back, forward;
    int forwardId, backId;

    // vectors to store the shortest distance and the visiting status of each node in forward direction
    vector<int>  forwardDistance(size, (numeric_limits<int>::max())/2);
    vector<bool> forwardVisited(size, false);

    // vectors to store the shortest distance and the visiting status of each node in backward direction
    vector<int>  backDistance(size,(numeric_limits<int>::max())/2);
    vector<bool> backVisited(size, false);

    // initialize the forward direction
    forward.vertex = s;
    forward.distance = 0;
    minHeapF.push(forward);
    forwardDistance[s] = 0;
   
    // initialize the backward direction
    back.vertex = t;
    back.distance = 0;
    minHeapB.push(back);
    backDistance[t] = 0;

    for(int i = 0; i < size; ++i){
      
      // find an unvisited vertex with minimum distance in forward direction
      do{
	forward = minHeapF.top();
	minHeapF.pop();
	forwardId = forward.vertex;
      }while(forwardVisited[forwardId] == true);
      
      forwardVisited[forwardId] = true;

      // find an unvisited vertex with minimum distance in backward direction
      do{
	back = minHeapB.top();
	minHeapB.pop();
	backId = back.vertex;
      }while(backVisited[backId] == true);
      
      backVisited[backId] = true;

      // update the minimum distance of the successor node in forward direction   
      for(auto iter = adjList[forwardId].begin(); iter != adjList[forwardId].end(); ++iter){
	if(forwardDistance[iter->vertex]  > forwardDistance[forwardId] + getDistance(forwardId, iter->vertex)){
	  forwardDistance[iter->vertex] = forwardDistance[forwardId] + getDistance(forwardId,iter->vertex);
	  forward.distance = forwardDistance[iter->vertex];
	  forward.vertex = iter->vertex;
	  minHeapF.push(forward);
	}
      }

      // update the minimum distance of the successor node in backward direction   
      for(auto iter = adjList[backId].begin(); iter != adjList[backId].end(); ++iter){
	if(backDistance[iter->vertex]  > backDistance[backId] + getDistance(backId, iter->vertex)){
	  backDistance[iter->vertex] = backDistance[backId] + getDistance(backId,iter->vertex);
	  back.distance = backDistance[iter->vertex];
	  back.vertex = iter->vertex;
	  minHeapB.push(back);
	}
      }

      // if there is one node visited by both direction get out of the loop
      for(int j = 0; j != size; ++j){
	if(forwardVisited[j] && backVisited[j]){
	  stop = 1;
	}
      }

      if(stop == 1) break;
    }
    
    // loop through all the visited node in forward direction 
    // find the minimum distance from forward direction and backward direction
    int min = numeric_limits<int>::max();
    for(int i = 0; i < size; ++i){
      if(forwardVisited[i] == true){
	if(forwardDistance[i] + backDistance[i] < min)
	  min = forwardDistance[i] + backDistance[i]; 
      }
    }

    shortestD = min;
    return ;
  }

  // print out the shortest distance
  void shortestDistance(){
    cout << "The shortest distance is " << shortestD << endl;
  }   
  
private:
  class Node{
  public:
    int vertex;
    int distance;
  };
 
  // To use STL priority_queue as minHeap 
  // comparator should be redefined since STL priority_queue in STL is maxHeap
  struct comparator{
   bool operator() ( Node i, Node j){
     if(i.distance < j.distance)
       return false;
     else
       return true;
   }
  };

  // the variable to store the shortest distance
  int shortestD;

  int size;
  vector<list<Node> > adjList;
  
  // get the distance between vertex v1 and vertex w1
  int getDistance(int v1, int w1){
    for(auto element : adjList[v1])
      {
	if(element.vertex == w1)
	  return element.distance;
      }

    // if no node found return large value    
    return numeric_limits<int>::max();
  }
    
};

int main(){
  
  int graphSize = 1000;
  int source, target;
  double density = 0.1;

  // the variable to record the running time
  clock_t t;

 
  // The mean time for Dijkstra
  double mean1 = 0;

  // The mean time for Bidirection
  double mean2 = 0;

  for (int i = 0; i != 100; ++i) {
    
    // 1000 node size distance from 1 to 200, with 0.1 as density
    graph test(1000, 1, 200, density);
    source =rand() % graphSize ;
    
    // target and source can't be the same
    do {
      target = rand() % graphSize;
    } while (source == target);


    t = clock();
    test.DijkstraSearch(source, target);
    t = clock() - t;
    mean1 += t*1.0 /10000;
    
    t = clock();
    test.biDirection(source, target);
    t = clock() - t;
    mean2 += t*1.0/10000;
  }
  
  mean1 =  mean1/100;
  mean2 =  mean2/100;
  
  cout << "For density = " << density << endl;
  cout << "average time for Dijkstra:        " << mean1 << endl;
  cout << "average time for bidirectional:   " << mean2 << endl;


  mean1 = 0;
  mean2 = 0;

  density = 0.01;
  for (int i = 0; i != 100; ++i) {
    
    // 1000 node size distance from 1 to 200, with 0.01 as density
    graph test(1000, 1, 200, density);
    source = rand() % graphSize;
    
    // target and source can't be the same
    do {
      target = rand() % graphSize;
    } while (source == target);
    
    t = clock();
    test.DijkstraSearch(source, target);
    t = clock() - t;
    mean1 += t*1.0/1000;
    
    t = clock();
    test.biDirection(source, target);
    t = clock() - t;
    mean2 += t*1.0/1000;
  }
  
  mean1 =  mean1/100;
  mean2 =  mean2/100;
  
  cout << "For density = " << density << endl;
  cout << "average time for Dijkstra:        " << mean1 << endl;
  cout << "average time for bidirectional:   " << mean2 << endl;
  
  return 0;
}    
