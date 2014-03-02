#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <queue>
#include <ctime>

// #define _DEBUG_

// #define _DEBUGAI_

using namespace std;
const int SIZE = 5;

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
  bool move (int x, int y, Color c);
  void AI(int &x, int &y, int difficulty = 1000);  // computer AI, will generate a move
private:
  bool upjudge (int node);
  bool leftjudge (int node);
  void indexToCoordin (int index, int &row, int &col);
  int coordinToIndex (int x, int y);

  vector<bool> visitorInfo;
  vector<Color> color;
  vector<list<int> > adjList;
};

int Hex::coordinToIndex(int x, int y){
  return ((x-1) * SIZE + y -1);
}

bool Hex::move (int x, int y, Color c){

  int loc = coordinToIndex(x, y);
  if (color[loc] != Color::Empty)
    return false;
  if (c == Color::White){
    color[loc] = Color::White;
  }
  else if (c == Color::Black)
    color[loc] = Color::Black;

  return true;
}

void Hex::indexToCoordin (int index, int &row, int &col){
  for (row = 1; row != SIZE*SIZE; ++row){
    if (index <= SIZE - 1){
      col = index + 1;
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

#ifdef _DEBUG_
  for (int i = 0; i != SIZE*SIZE; ++i){   // check the correctness of the adjacency list
    cout << i << " -> ";
    for ( auto elem : adjList[i])
      cout << elem << " -> ";
    cout << endl;
  }
#endif
}

void Hex::setBoard(){
  fill_n(color.begin(), (SIZE*SIZE), Color::Empty);	
}

bool Hex::upjudge(int node){
  int v;
  queue<int> Q;
  visitorInfo[node] = true;

  // start from node
  Q.push(node);
  while(!Q.empty()){
    v = Q.front();
    Q.pop();
    visitorInfo[v] = true;
    
#ifdef _DEBUG_
    cout << "reached node: " << v << endl;
    cout << "neighbors: ";
#endif

    for( auto w : adjList[v]){
      
#ifdef _DEBUG_
 
      if (color[w] == Color::White)
	cout << w << " with color: White" << "visited info: " << visitorInfo[w] << endl;
      else if (color[w] == Color::Black)
	cout << w << " with color: Black" << "visited info: " << visitorInfo[w] << endl;
      else cout << w << " with color: Empty" << "visited info: " << visitorInfo[w] << endl;

#endif

      if( visitorInfo[w] == false && (color[w] == color[node])){
	visitorInfo[w] = true;  // use "VISITING" to identify "candidates" for visits

#ifdef _DEBUG_
	cout << " next visited: " << w;
#endif
	if((w >= SIZE*(SIZE-1) && w < SIZE*SIZE)){  // reached the bottom line

#ifdef _DEBUG_
	  cout << endl;
#endif

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
    visitorInfo.assign(SIZE*SIZE, false);
    if(color[i] == Color::White && upjudge(i)){
      return true;	
    }
  }
  for (int i = 0; i <= SIZE*(SIZE - 1); i += SIZE){    // if the node is in the left side
    visitorInfo.assign(SIZE*SIZE, false);
    if(color[i] == Color::Black && leftjudge(i)){
      return true;
    }
  }
  return false;
}

void Hex::AI(int &opt_x, int &opt_y, int difficulty){  // AI takes the black side
  int move, simul;
  double max;
  srand(time(NULL));

  vector<Color> backup(color);      // store the current board info.
  vector<int> scores(SIZE*SIZE, 0);
  vector<int> times(SIZE*SIZE, 0);
  
  for(int i = 0; i != difficulty; ++i){
    while(true){    // loop until get an legal move
      move = rand() % (SIZE*SIZE);
      if (color[move] == Color::Empty){
		color[move] = Color::Black;
		times[move]++;

#ifdef _DEBUGAI_
		cout << "move chosen " << move << endl;
#endif
		break;
      }
    }

    if (wins()){
      color = backup;
      indexToCoordin(move, opt_x, opt_y);
      return;
    }
	
    while(true){  // simulation begins for one move

#ifdef _DEBUGAI_
	  cout << "simulation move: " << simul << endl;

#endif

      while(true){
		simul = rand() % (SIZE*SIZE);

#ifdef _DEBUGAI_
		cout << "stuck in simul: " << simul << endl;
#endif

		if (color[simul] == Color::Empty){
		  color[simul] = Color::White;
		  break;
		}
      } // White move ends
      if (wins()){
		//   	scores[move]--;  // this move loses one score
		color = backup;
		break;
      }
      while(true){
		simul = rand() % (SIZE*SIZE);

#ifdef _DEBUGAI_
		cout << "stuck in simul: " << simul << endl;
#endif

		if (color[simul] == Color::Empty){
		  color[simul] = Color::Black;
		  break;
		}
      }  // Black move ends
      if (wins()){
		scores[move]++;  // this move gets one score
		color = backup;
		break;
      }

    }  // end one simulation for one move
    	
  }  // end for
  
  int i = 0;

  while(true){
 
#ifdef _DEBUGAI_
	cout << "initialize the max " << i << endl;
	
#endif
	
	if (times[i] != 0){
	  max = scores[i]/times[i];
	  move = i;
	  break;
	}
	++i;
  }
  
  for (int i = 0; i != SIZE*SIZE; ++i){
	if (times[i] != 0)
	  if ( static_cast<double>(scores[i])/static_cast<double>(times[i]) > max){   // search for the optimal move
		max = static_cast<double>(scores[i])/static_cast<double>(times[i]);
		move = i;
	  }
  }
  
  indexToCoordin(move, opt_x, opt_y);
  color = backup;
  return;
  
}

int main(){
  Hex game;
  int x, y;
  game.setBoard();
  
  cout << game << endl;
  while (true){

    cout << "White side plays: please input the location" << endl;
    while (true){
      cin >> x >> y;
      if(game.move(x, y, Color::White))   // white moves first
	break;
      else cout << "illegal movement, please try agagin" << endl;
    }
    cout << game << endl;
    if (game.wins()) {
      cout << "White wins!" << endl;
      break;
    }

    cout << "computer plays..." << endl;
	game.AI(x, y, 100);
	game.move(x, y, Color::Black);
    cout << game << endl;
    if (game.wins()) {
      cout << "computer wins!" << endl;
      break;
    }
  }
  cout << "game is over, press any key to exit" << endl;
  cin >> x;
  exit(0);
  return 0;
}
