#ifndef HEX_H
#define HEX_H

#include <iostream>
#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>

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

#endif
