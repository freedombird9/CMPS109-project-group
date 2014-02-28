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
const int SIZE = 11;

enum class Color{Black, White, Empty};

class Hex {

public:
  friend ostream& operator<<(ostream &out, const Hex input){
    int m = 0;
	out << "board size:" << SIZE << "*" << SIZE << endl;
	for(int i = 0; i != SIZE; ++i){
	  if (i == 0){
	    for (int j = 0, col = 1; j != SIZE; ++j, ++col){   // first line
	      if (col - 10 < 0){
		if (j == 0) out << "   ";      // three white-space indent of the whole game board
		out << " " << col << "/" << " " << "\\" << " ";   // display the column number
	      }
	      else{
		if (j == 0) out << "   ";
		out << col << "/" << " " << "\\" << " ";
	      }
	      if ( j % SIZE == SIZE - 1 )  // end of line
		out << endl;
	    }
	    for (int j = 0; j != SIZE; ++j){      // second line
	      if (j == 0) out << "   ";      // three white-space indent of the whole game board
	      out << " " << "/" << "   " << "\\";
	      if ( j % SIZE == SIZE - 1 )  // end of line
		out << endl;
	    }
	  }  // end if (i == 0)
	  for (int j = 0; j!= SIZE; ++j){   // third line
	    if (j == 0) out << "   ";     // three white-space indent of the whole game board
	    out << "|" << "     ";	      
	    if ( j % SIZE == SIZE - 1 ){  // end of line
	      out << "|" << endl;
	      for (int k = 0; k != i; ++k)
		out << "   ";
	    }
	  }
	  for (int j = 0; j != SIZE; ++j){    // 4th line
	    if (j == 0){
	      if ((i+1) - 10 < 0)        // display the row number
		out << "  " << i+1;
	      else out << " " << i+1;
	    }
	    out << "|" << "  ";
	    switch(input.color[m++]){
	    case Color::Black:
	      out << "B";
	      break;
	    case Color::White:
	      out << "W";
	      break;
	    case Color::Empty:
	      out << " ";
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
	    if (j == 0) out << "   ";
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
	    if (j == 0) out << "   ";
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
  void setBoard ();
  bool wins ();  
  void move (int x, int y, Color c);
private:
  bool upjudge (int node);
  bool leftjudge (int node);
  void indexToCoordin (int index);
  int coordinToIndex (int x, int y);

  vector<bool> visitorInfo;
  vector<Color> color;
  vector<list<int> > adjList;
};

int coordinToIndex(int x, int y){
  return ((x-1) * SIZE + y -1);
}

void Hex::move (int x, int y, Color c){

  int loc = coordinToIndex(x, y);
  if (c == Color::White){
    color[loc] = Color::White;
  }
  else if (c == Color::Black)
    color[loc] = Color::Black;

  return;
}

void Hex::indexToCoordin (int index){
  int col, row;
  for (row = 1; row != SIZE; ++row){
    if (index <= SIZE - 1){
      col = index + 1;
      cout << "( " << row << ", " << col << " )";
      return;
    }
    if (index - SIZE < SIZE -1){
      col = index - SIZE + 1;
      cout << "( " << row << ", " << col << " )";
      return;
    }
    else index -= SIZE;
  }
}


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
	fill_n(color.begin(), (SIZE*SIZE), Color::Empty);	
}

bool Hex::upjudge(int node){
  int v;
  queue<int> Q;
  visitorInfo[node] = true;

#ifdef _DEBUG_  
  cout << node;
#endif

  // start from node
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
	if((w  > SIZE*(SIZE-1) && w < SIZE*SIZE)){  // reached the bottom line

#ifdef _DEBUG_
	  cout << endl;
#endif
	  cout << "node ";
	  indexToCoordin(w);
	  cout << " reached" << endl;

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
	if(w % SIZE == SIZE - 1 ){    // reached the right border

#ifdef _DEBUG_
	  cout << endl;
#endif
	  cout << "node ";
	  indexToCoordin(w);
	  cout << " reached" << endl;
	  return true;
	}
	Q.push(w);
      }
    }
  }
  return false; 
}

bool Hex::wins(){
 
  for (int i = 0; i != SIZE; ++i){        // if the node is in the upper side, assume white should connect upper side and bottom side
    visitorInfo.resize(SIZE*SIZE, 0);
    if(color[i] == Color::White && upjudge(i)){
      cout << "White node";
      indexToCoordin(i);
      cout << "reached" << endl;
      return true;	
    }
  }
  for (int i = 0; i <= SIZE*(SIZE - 1); i += SIZE){    // if the node is in the left side
    visitorInfo.resize(SIZE*SIZE, 0);
      if(color[i] == Color::Black && leftjudge(i)){
	cout << "Black node";
	indexToCoordin(i);
	cout << " reached" << endl;
	return true;
      }
  }
  return false;
}

int main(){
  Hex game;
  int x, y;
  game.setBoard();
  
  cout << game << endl;
  while (!game.wins()){

    cout << "White side plays: please input the location" << endl;
    cin >> x >> y;
    game.move(x, y, Color::White);   // white moves first
    cout << game << endl;
    cout << "Black side plays: please input the location" << endl;
    cin >> x >> y;
    game.move(x, y, Color::Black);  // then black moves
    cout << game <<endl;
  }
  return 0;
}
