all: Dijkstra_bidirect time_test

Dijkstra_bidirect: Dijkstra_bidirect.cpp
	g++ -std=c++11 -o Dijkstra_bidirect Dijkstra_bidirect.cpp

time_test:
	g++ -std=c++11 -o time_test time_test.cpp
