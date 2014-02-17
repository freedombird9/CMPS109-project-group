#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <queue>

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
  void judge();  
  
private:
  vector<bool> visitorInfo;
  vector<Color> color;
  vector<list<int> > adjList;
};

Hex::Hex(){
        visitorInfo.resize(SIZE*SIZE);
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

void Hex::judge(){
  int v;
  queue<int> Q;
  visitorInfo[0] = true;
 
  // start from node 0
  Q.push( 0 );
  while(!Q.empty()){
    v = Q.front();
    Q.pop();
    visitorInfo[v] = true;
    for( auto w : adjList[v]){
      if( visitorInfo[w] == false && color[w] == color[0]){
	visitorInfo[w] = true;  //use "VISITING" to identify "candidates" for visits

	if((w  > SIZE*(SIZE-1) && w < SIZE*SIZE -1) || (w % 10 == 0))  {
	    if(color[0] == Color::Black)
	      cout << "Black group wins!" << endl;
	    else
	      cout << "White group wins!" << endl;
	    return;
	  }
	  Q.push(w);
      }
    }
  }
  if(color[0] == Color::Black)
    cout << "White group wins!" << endl;
  else
    cout << "Black group wins!" << endl;
}

int main(){
	Hex board;
	board.setBoard();
	cout << board << endl;
	board.judge();
	return 0;
}
