#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <queue>
#include <ctime>
#include "hex.h"

// #define _DEBUG_

// #define _DEBUGAI_

using namespace std;


int Hex::coordinToIndex(int x, int y){
  return ((x-1) * size + y -1);
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
  for (row = 1; row != size*size; ++row){
    if (index <= size - 1){
      col = index + 1;
      return;
    }
    else index -= size;
  }
}


Hex::Hex(int si):size(si){
  visitorInfo.resize(size*size);
  adjList.resize(size*size);
  color.resize(size*size, Color::Empty);
  for (int i = 0; i != size*size; ++i){
    
    if (i == 0) {
      adjList[i].push_back(i+1);
      adjList[i].push_back(i+size);
    }
    else if (i < size - 1){
      adjList[i].push_back(i+1);
      adjList[i].push_back(i-1);
      adjList[i].push_back(i+size);
      adjList[i].push_back(i+size-1);
    }
    else if (i == size - 1 ){
      adjList[i].push_back(i-1);
      adjList[i].push_back(i+size);
      adjList[i].push_back(i+size-1);
    }
    else if (i % size == 0 && i != size*(size-1)){
      adjList[i].push_back(i-size);
      adjList[i].push_back(i-size+1);
      adjList[i].push_back(i+1);
      adjList[i].push_back(i+size);
    }
    else if (i == size*(size-1)){
      adjList[i].push_back(i-size);
      adjList[i].push_back(i-size+1);
      adjList[i].push_back(i+1);
    }
    else if (i % size == size - 1  && i != size*size-1){
      adjList[i].push_back(i-size);
      adjList[i].push_back(i-1);
      adjList[i].push_back(i+size);
      adjList[i].push_back(i+size-1);
    }
    else if (i == size*size -1){
      adjList[i].push_back(i-1);
      adjList[i].push_back(i-size);
    }
    else if (i > size*(size-1) && i < size*size -1){
      adjList[i].push_back(i-1);
      adjList[i].push_back(i-size);
      adjList[i].push_back(i+1);
      adjList[i].push_back(i-size+1);
    }
    else {
      adjList[i].push_back(i+1);
      adjList[i].push_back(i-1);
      adjList[i].push_back(i-size);
      adjList[i].push_back(i+size);
      adjList[i].push_back(i-size+1);
      adjList[i].push_back(i+size-1);
    }
  }

#ifdef _DEBUG_
  for (int i = 0; i != size*size; ++i){   // check the correctness of the adjacency list
    cout << i << " -> ";
    for ( auto elem : adjList[i])
      cout << elem << " -> ";
    cout << endl;
  }
#endif
}

void Hex::setBoard(){
  fill_n(color.begin(), (size*size), Color::Empty);	
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
	if((w >= size*(size-1) && w < size*size)){  // reached the bottom line

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
	if(w % size == size - 1 ){    // reached the right border

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
 
  for (int i = 0; i != size; ++i){        // if the node is in the upper side, assume white should connect upper side and bottom side
    visitorInfo.assign(size*size, false);
    if(color[i] == Color::White && upjudge(i)){
      return true;	
    }
  }
  for (int i = 0; i <= size*(size - 1); i += size){    // if the node is in the left side
    visitorInfo.assign(size*size, false);
    if(color[i] == Color::Black && leftjudge(i)){
      return true;
    }
  }
  return false;
}

void Hex::AI(int &opt_x, int &opt_y, int difficulty){  // AI takes the black side
  int move, simul;
  double max;

  vector<Color> backup(color);      // store the current board info.
  vector<int> scores(size*size, 0);
  vector<int> times(size*size, 0);
  
  for(int i = 0; i != difficulty; ++i){
    while(true){    // loop until get an legal move
      move = rand() % (size*size);
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
		simul = rand() % (size*size);

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
		simul = rand() % (size*size);

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
  
  for (int i = 0; i != size*size; ++i){
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

/*
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
  }   */
