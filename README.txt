To run this program, one just input "make" 
100 graphs with 1000 nodes have been tested with Dijkstra and Bidirection algorithm.The distance between nodes is randomly 
distributed between 1 to 200. 
For density = 0.1, the Bidirection algorithm is about 5 times as fast as Dijkstra. 
For denstty = 0.01, the Bidirection algorithm is less than 2 times as fast as Dijkstra. 
Since the density is very low, about 10 nodes are connected for density = 0.01, the difference between two algorithms is very
small. From this assignment, I have learned how to convert the STL default priority_queue to a minHeap, and use it to implement
Dijkstra and Bidirection algorithm. And use <random> library to generate random values. And BiDirection algorithm is very effecient
algorithm compared to original Dijkstra algorithm. 
