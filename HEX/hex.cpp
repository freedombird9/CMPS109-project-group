#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
const int SIZE = 11;

enum class Color{Black, White, Empty};

class Hex {

public:
  friend ostream& operator<<(ostream &out, const Hex input){
	out << "board size:" << SIZE << "*" << SIZE << endl;
	for(int i = 0; i != SIZE*SIZE; ++i){
	  switch(input.color[i]){
	  case Color::Black:
	    out << "\\" << "Black";
	    break;
	  case Color::White:
	    out << "\\" << "White";
	    break;
	  case Color::Empty:
	    out << "\\" << "Empty";
	    break;
	  default:
	    break;
	  }
	  if(i % SIZE == 10){
	    out << endl;
	    for (int j = 0; j != i/SIZE+1; ++j)
	      out << " ";
	  }
	}
	return out;
  };
  
  Hex(void);
  void setBoard();
  
  
private:
  
  vector<Color> color;
  vector<list<int> > adjList;
};

Hex::Hex(){
	adjList.resize(SIZE*SIZE);
	color.resize(SIZE*SIZE, Color::Empty);
	for (int i = 0; i != SIZE*SIZE; ++i){
	  
		if (i == 0) {
		  adjList[i].push_back(i+1);
		  adjList[i].push_back(i+SIZE);
		}
		else if (i < SIZE - 1){
			adjList[i].push_back(i+1);
			adjList[i].push_back(i-1);
			adjList[i].push_back(i+SIZE);
			adjList[i].push_back(i+SIZE-1);
		}
		else if (i == SIZE - 1 ){
			adjList[i].push_back(i-1);
			adjList[i].push_back(i+SIZE);
			adjList[i].push_back(i+SIZE-1);
		}
		else if (i % SIZE == 0 && i != SIZE*(SIZE-1)){
			adjList[i].push_back(i-SIZE);
			adjList[i].push_back(i-SIZE+1);
			adjList[i].push_back(i+1);
			adjList[i].push_back(i+SIZE);
		}
		else if (i == SIZE*(SIZE-1)){
			adjList[i].push_back(i-SIZE);
			adjList[i].push_back(i-SIZE+1);
			adjList[i].push_back(i+1);
		}
		else if (i % SIZE == 10 && i != SIZE*SIZE-1){
			adjList[i].push_back(i-SIZE);
			adjList[i].push_back(i-1);
			adjList[i].push_back(i+SIZE);
			adjList[i].push_back(i+SIZE-1);
		}
		else if (i == SIZE*SIZE -1){
			adjList[i].push_back(i-1);
			adjList[i].push_back(i-SIZE);
		}
		else if (i > SIZE*(SIZE-1) && i < SIZE*SIZE -1){
			adjList[i].push_back(i-1);
			adjList[i].push_back(i-SIZE);
			adjList[i].push_back(i+1);
			adjList[i].push_back(i-SIZE+1);
		}
		else {
			adjList[i].push_back(i+1);
			adjList[i].push_back(i-1);
			adjList[i].push_back(i-SIZE);
			adjList[i].push_back(i+SIZE);
			adjList[i].push_back(i-SIZE+1);
			adjList[i].push_back(i+SIZE-1);
		}
	}
}

void Hex::setBoard(){
	fill_n(color.begin(), (SIZE*SIZE), Color::White);
	fill_n(color.begin(), (SIZE*SIZE)/2 +1,Color::Black);
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	shuffle(color.begin(), color.end(), default_random_engine(seed));	
}


int main(){
	Hex board;
	board.setBoard();
	cout << board << endl;
	return 0;
}
