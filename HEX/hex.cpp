#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <queue>

// #define _DEBUG_

using namespace std;
const int SIZE = 9;

enum class Color{Black, White, Empty};

class Hex {

public:
  friend ostream& operator<<(ostream &out, const Hex input){
    int m = 0;
	out << "board size:" << SIZE << "*" << SIZE << endl;
	for(int i = 0; i != SIZE; ++i){
	  if (i == 0){
	    for (int j = 0; j != SIZE; ++j){   // first line
	      out << "  " << "/" << " " << "\\" << " ";
	      if ( j % SIZE == SIZE - 1 )  // end of line
		out << endl;
	    }
	    for (int j = 0; j != SIZE; ++j){      // second line
	      out << " " << "/" << "   " << "\\";
	      if ( j % SIZE == SIZE - 1 )  // end of line
		out << endl;
	    }
	  }  // end if (i == 0)
	  for (int j = 0; j!= SIZE; ++j){   // third line
	    out << "|" << "     ";	      
	    if ( j % SIZE == SIZE - 1 ){  // end of line
	      out << "|" << endl;
	      for (int k = 0; k != i; ++k)
		out << "   ";
	    }
	  }
	  for (int j = 0; j != SIZE; ++j){    // 4th line
	    out << "|" << "  ";
	    switch(input.color[m++]){
	    case Color::Black:
	      out << "B";
	      break;
	    case Color::White:
	      out << "W";
	      break;
	    case Color::Empty:
	      out << "E";
	      break;
	    default:
	      break;
	    }
	    out << "  ";
	    if ( j % SIZE == SIZE - 1 ){  // end of line
	      out << "|" << endl;
	      for (int k = 0; k != i; ++k)
		out << "   ";
	    }
	  }
	  for (int j = 0; j != SIZE; ++j){   // 5th line
	    out << " " << "\\" << "   " << "/";
	    if ( j % SIZE == SIZE - 1  ){  // end of line
	      if ( i == SIZE - 1){
		out << endl;
		for (int k = 0; k != i; ++k )
		  out << "   ";
		continue;
	      }
	      out << " " << "\\";
	      out << endl;
	      for (int k = 0; k != i; ++k)
		out << "   ";
	    }
	  }
	  for (int j = 0; j != SIZE; ++j){   // 6th (last) line
	    out << "  " << "\\" << " " << "/" << " ";
	    if ( j % SIZE == SIZE - 1  ){  // end of line
	      if ( i == SIZE - 1)
		continue;
	      out << "  " << "\\";
	      out << endl;
	      for (int k = 0; k != i; ++k)
		out << "   ";
	    }
	  }
	  out << "   ";  // end of a row in the game board
	}
	return out;
  };
  
  Hex(void);
  void setBoard();
  void wins();  
  
private:
  bool upjudge(int node);
  bool leftjudge(int node);
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
		else if (i % SIZE == SIZE - 1  && i != SIZE*SIZE-1){
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

bool Hex::upjudge(int node){
  int v;
  queue<int> Q;
  visitorInfo[node] = true;

#ifdef _DEBUG_  
  cout << node;
#endif

  // start from node 0
  Q.push(node);
  while(!Q.empty()){
    v = Q.front();
    Q.pop();
    visitorInfo[v] = true;
    for( auto w : adjList[v]){
      if( visitorInfo[w] == false && color[w] == color[node]){
	visitorInfo[w] = true;  //use "VISITING" to identify "candidates" for visits

#ifdef _DEBUG_
	cout << "->" << w;
#endif
	if((w  > SIZE*(SIZE-1) && w < SIZE*SIZE)){

#ifdef _DEBUG_
	  cout << endl;
#endif
	  cout << "node " << w << " reached" << endl;

	  return true;
	}
	Q.push(w);
      }
    }
  }
  return false;
}

bool Hex::leftjudge(int node){
  int v;
  queue<int> Q;
  visitorInfo[node] = true;

#ifdef _DEBUG_
  cout << node;
#endif

  // start from node 0
  Q.push(node);
  while(!Q.empty()){
    v = Q.front();
    Q.pop();
    visitorInfo[v] = true;
    for( auto w : adjList[v]){
      if( visitorInfo[w] == false && color[w] == color[node]){
	visitorInfo[w] = true;  //use "VISITING" to identify "candidates" for visits

#ifdef _DEBUG_
	cout << "->" << w;
#endif
	if(w % SIZE == SIZE - 1 ){

#ifdef _DEBUG_
	  cout << endl;
#endif
	  cout << "node " << w << " reached" << endl;
	  return true;
	}
	Q.push(w);
      }
    }
  }
  return false; 
}

void Hex::wins(){
  for (int i = 0; i!= SIZE*SIZE-1;++i){
    if (i >=0 && i <= SIZE-1){        // if the node is in the upper side
      visitorInfo.resize(SIZE*SIZE, 0);
      if(upjudge(i)){
	if (color[i] == Color::White){
	  cout << "White " << i << " wins"  << endl;
	  return;
	}
	else {
	  cout << "Black " << i << " wins"  << endl;
	  return;
	}
      }
    }
    if (i % SIZE == 0){    // if the node is in the left side
      visitorInfo.resize(SIZE*SIZE, 0);
      if(leftjudge(i)){
	if (color[i] == Color::White){
	  cout << "White " << i << " wins"  << endl;
	  return;
	}
	else {
	  cout << "Black " << i << " wins" << endl;
	  return;
	}
      }
    }
  }
}

int main(){
  Hex board;
  board.setBoard();
  cout << board << endl;
  board.wins();
  return 0;
}
