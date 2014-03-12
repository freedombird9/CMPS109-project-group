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

using namespace std;

// overloading << 
ostream& operator<<(ostream &out, const Hex& input){
  int m = 0;
  out << "board size:" << input.size << "*" << input.size << endl;
  for(int i = 0; i != input.size; ++i){
    if (i == 0){
      for (int j = 0, col = 1; j != input.size; ++j, ++col){   // first line
	if (col - 10 < 0){
	  if (j == 0) out << "   ";      // three white-space indent of the whole game board
	  out << " " << col << "/" << " " << "\\" << " ";   // display the column number
	}
	else{
	  if (j == 0) out << "   ";
	  out << col << "/" << " " << "\\" << " ";
	}
	if ( j % input.size == input.size - 1 )  // end of line
	  out << endl;
      }
      for (int j = 0; j != input.size; ++j){      // second line
	if (j == 0) out << "   ";      // three white-space indent of the whole game board
	out << " " << "/" << "   " << "\\";
	if ( j % input.size == input.size - 1 )  // end of line
	  out << endl;
      }
    }  // end if (i == 0)
    for (int j = 0; j!= input.size; ++j){   // third line
      if (j == 0) out << "   ";     // three white-space indent of the whole game board
      out << "|" << "     ";	      
      if ( j % input.size == input.size - 1 ){  // end of line
	out << "|" << endl;
	for (int k = 0; k != i; ++k)
	  out << "   ";
      }
    }
    for (int j = 0; j != input.size; ++j){    // 4th line
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
      if ( j % input.size == input.size - 1 ){  // end of line
	out << "|" << endl;
	for (int k = 0; k != i; ++k)
	  out << "   ";
      }
    }
    for (int j = 0; j != input.size; ++j){   // 5th line
      if (j == 0) out << "   ";
      out << " " << "\\" << "   " << "/";
      if ( j % input.size == input.size - 1  ){  // end of line
	if ( i == input.size - 1){
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
    for (int j = 0; j != input.size; ++j){   // 6th (last) line
      if (j == 0) out << "   ";
      out << "  " << "\\" << " " << "/" << " ";
      if ( j % input.size == input.size - 1  ){  // end of line
	if ( i == input.size - 1)
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

Hex::Hex(int s): size(s ){  
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

    for( auto w : adjList[v]){

      if( visitorInfo[w] == false && (color[w] == color[node])){
	visitorInfo[w] = true;  // use "VISITING" to identify "candidates" for visits

	if((w >= size*(size-1) && w < size*size)){  // reached the bottom line
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

  // start from node 0
  Q.push(node);
  while(!Q.empty()){
    v = Q.front();
    Q.pop();
    visitorInfo[v] = true;
    for( auto w : adjList[v]){
      if( visitorInfo[w] == false && color[w] == color[node]){
	visitorInfo[w] = true;  //use "VISITING" to identify "candidates" for visits
	if(w % size == size - 1 ){    // reached the right border
	  return true;
	}
	Q.push(w);
      }
    }
  }
  return false; 
}

bool Hex::wins(){
 
  // if the node is in the upper side, assume white should connect upper side and bottom side
  for (int i = 0; i != size; ++i){       
    visitorInfo.assign(size*size, false);
    if(color[i] == Color::White && upjudge(i)){
      return true;	
    }
  }
  // if the node is in the left side
  for (int i = 0; i <= size*(size - 1); i += size){    
    visitorInfo.assign(size*size, false);
    if(color[i] == Color::Black && leftjudge(i)){
      return true;
    }
  }
  return false;
}

// AI takes the black side
void Hex::AI(int &opt_x, int &opt_y, int difficulty){  
  int move, simul;
  double max;
  srand(time(NULL));

  vector<Color> backup(color);      // store the current board info.
  vector<int> scores(size*size, 0);
  vector<int> times(size*size, 0);
  
  for(int i = 0; i != difficulty; ++i){
    while(true){    
      // loop until get an legal move
      move = rand() % (size*size);
      if (color[move] == Color::Empty){
	color[move] = Color::Black;
	times[move]++;
	break;
      }
    }

    if (wins()){
      color = backup;
      indexToCoordin(move, opt_x, opt_y);
      return;
    }
    
    // simulation begins for one move
    while(true)
      {  
      while(true)
	{
	  simul = rand() % (size*size);
	  
	  if (color[simul] == Color::Empty)
	    {
	      color[simul] = Color::White;
	      break;
	    }
      } // White move ends
    
      if (wins()){
	color = backup;
	break;
      }
      while(true){
	simul = rand() % (size*size);

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

  while(true)
    {
      if (times[i] != 0)
	{
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

